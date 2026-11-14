#!/usr/bin/env python3
"""Prepare per-rep input workspaces for the Preguss baseline.

Preguss ingests RAW C files directly: its preprocessing stage runs
``frama-c <src> -rte -then -print`` itself to materialize RTE assertions
(see README.md section 2.1). So unlike the SpecSyn baseline there is NO
target-materialization step here — this script is deliberately a plain
copier. It exists only because Preguss writes outputs NEXT TO its input
(``<src>_rte_annotated_cil.c``) and into CWD-relative directories
(``output/``, ``results/``, ``logs/``, cache dir), so each (bench, model,
rep) gets an isolated workspace and our ``benchmarks/`` tree is never
mutated.

Layout produced (results tree is gitignored):

    scripts/experiments/results/baseline-preguss/<bench>/<model>/rep<k>/work/
        <benchfile>.c        copy of the registered bench source
        [<header>.h]         copy of the bench header, when registered

Bench name -> file resolution reuses scripts/experiments/lib/benchmarks.sh
(single source of truth) via a bash subprocess; no mapping is duplicated here.

Usage:
    python3 scripts/experiments/baseline-preguss/prepare_inputs.py
    python3 ... --benches student-enrollment jc-voronoi --reps 1
    python3 ... --model claude-sonnet-4-6 --reps 3 --force
"""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

# Repo root = three levels up from this file (scripts/experiments/baseline-preguss/).
REPO_ROOT = Path(__file__).resolve().parents[3]
BENCHMARKS_SH = REPO_ROOT / "scripts/experiments/lib/benchmarks.sh"
RESULTS_ROOT = REPO_ROOT / "scripts/experiments/results/baseline-preguss"

# Comparison surface: x509 submodule-1/submodule-2 + the contiki AES-CCM
# module + the util-libs corpus (the 14-bench RQ1 comparison set).
DEFAULT_BENCHES = [
    "submodule-1",
    "submodule-2",
    "contiki-aes-ccm",
    "ccan-time",
    "cgl",
    "char-utils",
    "hypatia",
    "jc-voronoi",
    "maratis-math",
    "punity",
    "sokol-shape",
    "stb-divide",
    "student-enrollment",
    "zpl-math",
]


def resolve_bench(name: str) -> tuple[Path, Path | None]:
    """Resolve bench name to (source, header|None) via lib/benchmarks.sh."""
    script = (
        f'source "{BENCHMARKS_SH}" && '
        f'src="$(bench_source {name})" || exit 3; '
        f'hdr="$(bench_header {name})" || exit 3; '
        f'printf "%s\\n%s\\n" "$src" "$hdr"'
    )
    proc = subprocess.run(
        ["bash", "-c", script], capture_output=True, text=True, cwd=REPO_ROOT
    )
    if proc.returncode != 0:
        raise KeyError(f"bench '{name}' not registered in {BENCHMARKS_SH}")
    src_rel, hdr_rel = (proc.stdout.split("\n") + [""])[:2]
    src = REPO_ROOT / src_rel
    if not src.is_file():
        raise FileNotFoundError(f"bench '{name}': source missing: {src}")
    hdr = None
    if hdr_rel.strip():
        hdr = REPO_ROOT / hdr_rel.strip()
        if not hdr.is_file():
            raise FileNotFoundError(f"bench '{name}': header missing: {hdr}")
    return src, hdr


def prepare_one(bench: str, model: str, rep: int, force: bool) -> Path:
    """Create one rep workspace; returns the work dir path."""
    src, hdr = resolve_bench(bench)
    rep_dir = RESULTS_ROOT / bench / model / f"rep{rep}"
    work = rep_dir / "work"
    work.mkdir(parents=True, exist_ok=True)

    targets = [(src, work / src.name)] + ([(hdr, work / hdr.name)] if hdr else [])
    for origin, dest in targets:
        if dest.exists() and not force:
            print(f"  keep   {dest.relative_to(REPO_ROOT)} (exists; --force to overwrite)")
            continue
        shutil.copy2(origin, dest)
        print(f"  copied {dest.relative_to(REPO_ROOT)}")
    return work


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--benches", nargs="*", default=DEFAULT_BENCHES,
                        help="bench names from lib/benchmarks.sh (default: the 16-bench comparison surface)")
    parser.add_argument("--model", default="claude-sonnet-4-6",
                        help="model tag used in the results path (default: claude-sonnet-4-6)")
    parser.add_argument("--reps", type=int, default=3, help="repetitions (default: 3)")
    parser.add_argument("--force", action="store_true", help="overwrite existing copies")
    args = parser.parse_args()

    failures = []
    for bench in args.benches:
        print(f"[{bench}]")
        for rep in range(1, args.reps + 1):
            try:
                prepare_one(bench, args.model, rep, args.force)
            except (KeyError, FileNotFoundError) as exc:
                print(f"  ERROR: {exc}", file=sys.stderr)
                failures.append(bench)
                break
    if failures:
        print(f"\nFAILED benches: {', '.join(failures)}", file=sys.stderr)
        return 1
    print(f"\nWorkspaces ready under {RESULTS_ROOT.relative_to(REPO_ROOT)}/")
    print("Next: run run_preguss.sh on a prepared bench (see this dir's README).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
