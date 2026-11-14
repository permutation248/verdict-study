#!/usr/bin/env python3
"""Prepare SpecSyn-pipeline input files from Verdict benchmarks.

For a registered benchmark (see scripts/experiments/lib/benchmarks.sh) this
script runs OUR preprocessing (frama-c -rte -> CIL -> rte_N renumbering) via
``verdict.preprocessing.pipeline.preprocess`` and writes the resulting
rte_N-annotated CIL file to ``inputs/<bench>/<stem>.c`` — the exact input
format the SpecSyn artifact consumes (see README.md, finding #1):

  * one self-contained .c file, no main() required;
  * verification targets are ordinary ACSL body assertions
    ``/*@ assert rte_N: <category>: <pred>; */`` — SpecSyn's generation is
    label-agnostic (it never parses assert labels), and the rte_N labels are
    what our scorer (score_rte.py) counts in `-then -report` output.

The script does NOT call any LLM and does NOT touch any remote server.
Requires: local frama-c on PATH (same requirement as `verdict solve`), and
must run inside the repo env (use `uv run`).

Usage:
    uv run python scripts/experiments/baseline-specsyn/prepare_inputs.py --list
    uv run python scripts/experiments/baseline-specsyn/prepare_inputs.py jc-voronoi
    uv run python scripts/experiments/baseline-specsyn/prepare_inputs.py submodule-1 --out-dir /tmp/x
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", ".."))
BENCHMARKS_SH = os.path.join(REPO_ROOT, "scripts", "experiments", "lib", "benchmarks.sh")

RTE_ASSERT_RE = re.compile(r"/\*@\s*assert\s+(rte_\d+)\s*:")

# Frama-C 29 pretty-prints ACSL with UTF-8 math symbols; the SpecSyn corpus
# (testcases/Preguss/*.c) is plain ASCII. Map every symbol Frama-C may emit.
UNICODE_TO_ACSL = {
    "≤": "<=",      # ≤
    "≥": ">=",      # ≥
    "≢": "!=",      # ≢ (printed for term disequality, e.g. division_by_zero)
    "≠": "!=",      # ≠
    "≡": "==",      # ≡
    "¬": "!",       # ¬
    "∧": "&&",      # ∧
    "∨": "||",      # ∨
    "⇒": "==>",     # ⇒
    "⇔": "<==>",    # ⇔
    "∀": "\\forall ",  # ∀
    "∃": "\\exists ",  # ∃
    "∈": "\\in",    # ∈
    "ℕ": "integer", # ℕ (approximation; not expected in RTE asserts)
    "ℤ": "integer", # ℤ
    "ℝ": "real",    # ℝ
    "⊥": "\\false", # ⊥
}

ACSL_BLOCK_RE = re.compile(r"/\*@.*?\*/", re.DOTALL)
QUOTED_INCLUDE_RE = re.compile(r'^(\s*#\s*include\s*)"([^"]+)"', re.MULTILINE)
# Frama-C's own builtin header rides along in some CIL prints; pycparser's
# fake_libc has no such header and nothing in an RTE-annotated bench calls
# Frama_C_* builtins, so the include is dropped (guarded below).
FC_BUILTIN_INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]__fc_builtin\.h[>"]\s*$', re.MULTILINE)


def normalize_for_specsyn(cil_text: str) -> str:
    """Normalize our annotated CIL to the SpecSyn corpus shape.

    1. UTF-8 ACSL operators -> ASCII (their corpus is ASCII; avoids any
       cpp/locale surprises in their pycparser/Frama-C round-trips).
    2. Collapse multi-line ``/*@ assert ... */`` blocks to a single line
       (their corpus has one assert per line; single-line blocks are also
       immune to their brace-counting segmenter and re-indenter).
    3. ``#include "x.h"`` -> ``#include <x.h>``: their decomp.py only
       registers angle-form includes as dependency nodes
       (regex ``#include[\\s]*<(.*)>``), and angle form resolves against
       pycparser's fake_libc_include via ``-I``.
    4. GCC/CIL keyword spellings -> C99 (``__inline`` -> ``inline``,
       ``__restrict`` -> ``restrict``): pycparser only knows the C99
       forms; Frama-C's CIL printer emits the GCC spellings.

    The input is guaranteed ACSL-free apart from RTE asserts (checked on all
    registered benches), so collapsing every ACSL block is safe.
    """
    cil_text = cil_text.replace("__inline", "inline").replace("__restrict", "restrict")
    for uni, ascii_ in UNICODE_TO_ACSL.items():
        cil_text = cil_text.replace(uni, ascii_)

    def _collapse(m: re.Match) -> str:
        return " ".join(m.group(0).split())

    cil_text = ACSL_BLOCK_RE.sub(_collapse, cil_text)
    cil_text = QUOTED_INCLUDE_RE.sub(r"\1<\2>", cil_text)
    return cil_text


def _bash_lookup(func: str, bench: str) -> str | None:
    """Resolve a bench attribute by sourcing the shell registry (no duplication)."""
    res = subprocess.run(
        ["bash", "-c", f'source "{BENCHMARKS_SH}" && {func} "{bench}"'],
        capture_output=True,
        text=True,
    )
    if res.returncode != 0:
        return None
    return res.stdout.strip()


def list_benches() -> list[str]:
    """Extract registered bench names from the bench_source() case arms."""
    names: list[str] = []
    in_func = False
    with open(BENCHMARKS_SH) as f:
        for line in f:
            if line.startswith("bench_source()"):
                in_func = True
                continue
            if in_func and line.startswith("}"):
                break
            if in_func:
                m = re.match(r"\s*([A-Za-z0-9_-]+)\)\s+echo", line)
                if m:
                    names.append(m.group(1))
    return names


def prepare(bench: str, out_dir: str, keep_main: bool = False) -> str:
    src_rel = _bash_lookup("bench_source", bench)
    if not src_rel:
        sys.exit(f"error: unknown bench '{bench}' (try --list)")
    src = os.path.join(REPO_ROOT, src_rel)
    if not os.path.exists(src):
        sys.exit(f"error: source file not found: {src}")

    # Our pipeline auto-copies .h files living next to the source, so the
    # registered header (bench_header) needs no special handling.
    sys.path.insert(0, os.path.join(REPO_ROOT, "src"))
    from verdict.preprocessing.pipeline import preprocess  # noqa: PLC0415

    print(f"[prepare] preprocessing {src_rel} (frama-c -rte -> CIL -> rte_N renumber)")
    project_ctx, ecg = preprocess(src)
    cil_file = project_ctx.source_file

    with open(cil_file) as f:
        cil_text = f.read()

    # 0. Drop EVA harness functions: main() plus any helper that references
    #    Frama_C_* builtins (e.g. rand_buf_or_null in x509-full-module).
    #    Cutting them removes all Frama_C_* references so __fc_builtin.h
    #    is not needed downstream. Extents come from the call-graph nodes
    #    against the RAW CIL (before any line edits); cut bottom-up so
    #    line numbers stay valid.
    harness_fns = []
    for name, node in project_ctx.call_graph_dict.items():
        if (name == "main" and not keep_main) or (
            hasattr(node, "extent") and node.extent
            and re.search(r"\bFrama_C_\w+", "\n".join(
                cil_text.split("\n")[node.extent[0] - 1 : node.extent[2]]))
        ):
            if getattr(node, "extent", None):
                harness_fns.append((name, node.extent))
    if harness_fns:
        harness_fns.sort(key=lambda x: -x[1][0])
        raw_lines = cil_text.split("\n")
        for name, (s_line, _, e_line, _) in harness_fns:
            del raw_lines[s_line - 1 : e_line]
            print(f"[prepare] dropped {name}() (CIL lines {s_line}-{e_line}, EVA harness)")
        cil_text = "\n".join(raw_lines)

    # SpecSyn-input adjustments (kept minimal — see README finding #1):
    # 1. Drop the "/* Generated by Frama-C */" banner: their corpus files do
    #    not carry it, and our own pipeline.preprocess() would otherwise treat
    #    a re-fed copy as already preprocessed. Harmless to cpp/pycparser, but
    #    removing it keeps the file shape identical to testcases/Preguss/*.c.
    lines = cil_text.split("\n")
    while lines and lines[0].strip().startswith("/* Generated by Frama-C"):
        lines.pop(0)
    cil_text = "\n".join(lines).lstrip("\n")

    # 1b. Drop frama-c's builtin include (no fake_libc equivalent); refuse
    # if the file actually references Frama_C_ builtins.
    if FC_BUILTIN_INCLUDE_RE.search(cil_text):
        without = FC_BUILTIN_INCLUDE_RE.sub("", cil_text)
        if re.search(r"\bFrama_C_\w+\s*\(", without):
            raise SystemExit(
                "input calls Frama_C_* builtins; cannot drop __fc_builtin.h"
            )
        cil_text = without

    # 2.-4. ASCII operators, single-line asserts, angle-form includes.
    cil_text = normalize_for_specsyn(cil_text)

    n_asserts = len(set(RTE_ASSERT_RE.findall(cil_text)))
    n_funcs = len(
        [
            name
            for name, node in project_ctx.call_graph_dict.items()
            if getattr(node, "is_definition", True)
        ]
    )

    stem = os.path.splitext(os.path.basename(src))[0]
    bench_dir = os.path.join(out_dir, bench)
    os.makedirs(bench_dir, exist_ok=True)
    out_path = os.path.join(bench_dir, f"{stem}.c")
    with open(out_path, "w") as f:
        f.write(cil_text)

    print(f"[prepare] bench={bench} functions={n_funcs} rte_asserts={n_asserts}")
    print(f"[prepare] wrote {os.path.relpath(out_path, REPO_ROOT)}")

    # Clean up the temp work_dir created by preprocess() (tempfile.mkdtemp
    # with prefix "verdict_").
    work_dir = project_ctx.work_dir
    if (
        work_dir
        and os.path.isdir(work_dir)
        and os.path.basename(work_dir).startswith("verdict_")
    ):
        shutil.rmtree(work_dir, ignore_errors=True)
    return out_path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("bench", nargs="*", help="registered bench name(s)")
    parser.add_argument("--list", action="store_true", help="list registered benches")
    parser.add_argument(
        "--out-dir",
        default=os.path.join(SCRIPT_DIR, "inputs"),
        help="output root (default: scripts/experiments/baseline-specsyn/inputs)",
    )
    parser.add_argument(
        "--keep-main", action="store_true",
        help="do NOT drop main() (still drops Frama_C_* helpers); used to "
             "reconstruct the harness main with its rte for re-scoring",
    )
    args = parser.parse_args()

    if args.list:
        for name in list_benches():
            src = _bash_lookup("bench_source", name) or "?"
            print(f"{name:24s} {src}")
        return

    if not args.bench:
        parser.error("give at least one bench name, or --list")

    for bench in args.bench:
        prepare(bench, args.out_dir, keep_main=args.keep_main)


if __name__ == "__main__":
    main()
