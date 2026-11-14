#!/usr/bin/env python3
"""Score rte_N assertions in spec-annotated C files via Frama-C WP `-report`.

The SpecSyn artifact ships an SV-COMP scorer (results/*/Live-FM-Bench/
verify_all.py) that counts report lines whose property name starts with
``Assertion 'target'``. No equivalent exists for the RTE corpus — this script
is that missing scorer for ``rte_N`` labels.

Pipeline (mirrors their verifier.py / verify_all.py exactly):

    frama-c -wp <file> -c11 -wp-timeout <N> [-wp-model <M>] [...] -then -report

and parses report lines of the form (empirically validated on frama-c 29.0):

    [  Valid  ] Assertion 'rte_1,mem_access' (file t.c, line 4)
    [    -    ] Assertion 'rte_2,signed_overflow' (file t.c, line 6)

An assertion counts as proven iff its status is exactly ``Valid`` (same rule
as their verify_all.py). Denominator = distinct rte_N labels present in the
source text, so asserts dropped by the kernel (annot-error) count as failures
("missing") instead of silently shrinking the total.

Presets:
    --preset specsyn   their scoring semantics: -wp-model Typed+var+int+real,
                       default prover (alt-ergo), default const handling.
    --preset verdict   our sound semantics: default Typed model,
                       -wp-prover z3,cvc5,alt-ergo, -wp-no-init-const
                       (matches src/verdict/tools/_wp_runner.py _wp_core()).

Usage:
    python score_rte.py results/specsyn/voronoi.c --csv scores.csv
    python score_rte.py result_dir/ --preset verdict --wp-timeout 10 \
        --detail-csv detail.csv
"""

from __future__ import annotations

import argparse
import csv
import os
import re
import subprocess
import sys
import time

RTE_SOURCE_RE = re.compile(r"/\*@\s*assert\s+(rte_\d+)\s*:")
# Anchored at column 0: pending-dependency continuation lines under
# [ Partial ] entries are indented and must not match.
REPORT_LINE_RE = re.compile(r"^\[(?P<status>[^\]]*)\]\s*(?P<prop>.*?)\s*\(file (?P<file>.*?), line (?P<line>\d+)\)")
FUNC_HEADER_RE = re.compile(r"^--- Properties of Function '(?P<func>.*)'")
PROP_RTE_RE = re.compile(r"^Assertion '(rte_\d+)")


def build_command(path: str, args: argparse.Namespace) -> list[str]:
    # annot-error=active: an ill-typed LLM annotation must drop only that
    # one contract (counted via its dependent asserts going missing), not
    # abort the kernel and zero the whole file (fc29 default is fatal;
    # hit on submodule-2: `prim ==` written for `*prim ==`, 2026-06-12).
    cmd = [args.frama_c, "-wp", path, "-c11",
           "-kernel-warn-key", "annot-error=active",
           "-wp-timeout", str(args.wp_timeout)]
    if args.wp_model:
        cmd += ["-wp-model", args.wp_model]
    if args.prover:
        cmd += ["-wp-prover", args.prover]
    if args.wp_par:
        cmd += ["-wp-par", str(args.wp_par)]
    if args.no_init_const:
        cmd += ["-wp-no-init-const"]
    cmd += args.extra_args
    cmd += ["-then", "-report"]
    return cmd


def score_file(path: str, args: argparse.Namespace) -> tuple[dict, list[dict]]:
    """Run WP+report on one file. Returns (summary_row, detail_rows)."""
    with open(path) as f:
        source = f.read()
    source_labels = sorted(set(RTE_SOURCE_RE.findall(source)), key=lambda s: int(s.split("_")[1]))

    cmd = build_command(path, args)
    t0 = time.time()
    try:
        proc = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=args.proc_timeout or None,
        )
        output = proc.stdout
        exit_code = proc.returncode
    except subprocess.TimeoutExpired as exc:
        output = (exc.stdout or b"").decode() if isinstance(exc.stdout, bytes) else (exc.stdout or "")
        exit_code = -1
    elapsed = time.time() - t0

    if args.keep_log:
        os.makedirs(args.keep_log, exist_ok=True)
        log_path = os.path.join(args.keep_log, os.path.basename(path) + ".report.log")
        with open(log_path, "w") as f:
            f.write(output)

    syntax_error = any("  syntax error:" in line for line in output.split("\n"))

    # Parse report: status per rte label (function context for the detail CSV).
    statuses: dict[str, dict] = {}
    current_func = ""
    for line in output.split("\n"):
        if m := FUNC_HEADER_RE.match(line):
            current_func = m.group("func")
            continue
        m = REPORT_LINE_RE.match(line)
        if not m:
            continue
        prop = m.group("prop").strip()
        rm = PROP_RTE_RE.match(prop)
        if not rm:
            continue
        label = rm.group(1)
        statuses[label] = {
            "status": m.group("status").strip(),
            "function": current_func,
            "line": int(m.group("line")),
        }

    detail_rows = []
    n_valid = 0
    for label in source_labels:
        entry = statuses.get(label)
        status = entry["status"] if entry else "Missing"
        if status == "Valid":
            n_valid += 1
        detail_rows.append(
            {
                "file": path,
                "rte_id": label,
                "function": entry["function"] if entry else "",
                "line": entry["line"] if entry else "",
                "status": status,
            }
        )

    total = len(source_labels)
    summary = {
        "file": path,
        "n_rte_source": total,
        "n_rte_report": len(statuses),
        "n_valid": n_valid,
        "ratio": round(n_valid / total, 4) if total else 0.0,
        "elapsed_s": round(elapsed, 1),
        "wp_model": args.wp_model or "(default Typed)",
        "wp_timeout": args.wp_timeout,
        "prover": args.prover or "(default alt-ergo)",
        "syntax_error": syntax_error,
        "exit_code": exit_code,
    }
    return summary, detail_rows


def collect_files(paths: list[str]) -> list[str]:
    files = []
    for p in paths:
        if os.path.isdir(p):
            for dirpath, _dirs, names in sorted(os.walk(p)):
                files.extend(os.path.join(dirpath, n) for n in sorted(names) if n.endswith(".c"))
        elif p.endswith(".c"):
            files.append(p)
        else:
            print(f"warning: skipping non-.c argument {p}", file=sys.stderr)
    return files


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("paths", nargs="+", help=".c files or directories to score")
    parser.add_argument("--preset", choices=["specsyn", "verdict"], default="specsyn",
                        help="parameter preset (default: specsyn = artifact scoring semantics)")
    parser.add_argument("--wp-timeout", type=int, default=10,
                        help="per-goal prover timeout, seconds (artifact scoring default: 10)")
    parser.add_argument("--wp-model", default=None,
                        help="-wp-model override (preset specsyn: Typed+var+int+real)")
    parser.add_argument("--prover", default=None,
                        help="-wp-prover (preset verdict: z3,cvc5,alt-ergo)")
    parser.add_argument("--wp-par", type=int, default=None,
                        help="-wp-par prover processes (default: frama-c default of 4)")
    parser.add_argument("--no-init-const", action="store_true",
                        help="pass -wp-no-init-const (on in preset verdict)")
    parser.add_argument("--frama-c", default="frama-c", help="frama-c binary")
    parser.add_argument("--extra-args", nargs="*", default=[],
                        help="extra frama-c arguments inserted before -then -report")
    parser.add_argument("--proc-timeout", type=int, default=0,
                        help="hard wall-clock limit per file, seconds (0 = none)")
    parser.add_argument("--csv", default=None, help="write per-file summary CSV here")
    parser.add_argument("--detail-csv", default=None, help="write per-assert detail CSV here")
    parser.add_argument("--keep-log", default=None, help="directory to store raw report logs")
    args = parser.parse_args()

    # Apply presets only where the user did not override explicitly.
    if args.preset == "specsyn":
        if args.wp_model is None:
            args.wp_model = "Typed+var+int+real"
    elif args.preset == "verdict":
        if args.prover is None:
            args.prover = "z3,cvc5,alt-ergo"
        args.no_init_const = True
        # wp_model stays None -> frama-c default (Typed), our sound model.

    files = collect_files(args.paths)
    if not files:
        sys.exit("error: no .c files found")

    summaries = []
    details = []
    for i, path in enumerate(files, 1):
        print(f"[{i}/{len(files)}] scoring {path} ...", flush=True)
        summary, detail = score_file(path, args)
        summaries.append(summary)
        details.extend(detail)
        print(
            f"    rte total={summary['n_rte_source']} valid={summary['n_valid']} "
            f"ratio={summary['ratio']} elapsed={summary['elapsed_s']}s"
            + (" [SYNTAX ERROR]" if summary["syntax_error"] else ""),
            flush=True,
        )

    if args.csv:
        with open(args.csv, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=list(summaries[0].keys()))
            writer.writeheader()
            writer.writerows(summaries)
        print(f"summary csv -> {args.csv}")
    if args.detail_csv and details:
        with open(args.detail_csv, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=list(details[0].keys()))
            writer.writeheader()
            writer.writerows(details)
        print(f"detail csv -> {args.detail_csv}")

    grand_total = sum(s["n_rte_source"] for s in summaries)
    grand_valid = sum(s["n_valid"] for s in summaries)
    print(f"TOTAL: {grand_valid}/{grand_total} rte assertions Valid")


if __name__ == "__main__":
    main()
