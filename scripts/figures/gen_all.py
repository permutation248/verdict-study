#!/usr/bin/env python3
"""Regenerate every paper table/figure from raw data in one command.

Runs all gen_*.py builders in a single process (so they share load_raw's
benchmark-metadata cache) under one commit/date stamp, leaving
the generated/ directory rebuilt deterministically and clean (only .tex/.csv/.pdf).

Usage:
    gen_all.py [out_dir] [--commit C] [--date D]
Defaults: out_dir = <this dir>/generated, commit = git short HEAD, date = today.
This is the single entry point behind the data-provenance pipeline: one command
turns the raw run artifacts under results/ into every artifact the paper inputs.
"""
from __future__ import annotations
import argparse
import datetime
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import gen_rq1                # noqa: E402
import gen_rq2                # noqa: E402
import gen_rq2c               # noqa: E402
import gen_rq3                # noqa: E402

# order: study setup (bench table) -> RQ1 -> RQ2 (a/b) -> RQ2c mechanism -> RQ3
BUILDERS = [
    ("rq1", gen_rq1.build),
    ("rq2 (rq2a + rq2b)", gen_rq2.build),
    ("rq2c", gen_rq2c.build),
    ("rq3", gen_rq3.build),
]


def _git_commit() -> str:
    try:
        return subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"], cwd=HERE, text=True,
            stderr=subprocess.DEVNULL).strip() or "HEAD"
    except Exception:
        return "HEAD"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("out_dir", nargs="?",
                    default=os.path.normpath(os.path.join(HERE, "generated")))
    ap.add_argument("--commit", default=None, help="source commit stamp (default: git HEAD)")
    ap.add_argument("--date", default=None, help="generation date stamp (default: today)")
    a = ap.parse_args()

    out = os.path.abspath(a.out_dir)
    os.makedirs(out, exist_ok=True)
    commit = a.commit or _git_commit()
    date = a.date or datetime.date.today().isoformat()

    print(f"== regenerating all paper artifacts\n   out    : {out}\n"
          f"   commit : {commit}\n   date   : {date}\n")
    failed = []
    for name, build in BUILDERS:
        print(f"-- {name}")
        try:
            build(out, commit, date)
        except Exception as exc:                       # report, keep going
            failed.append(name)
            print(f"   !! FAILED: {exc!r}")

    # defensive: strip any stray LaTeX build artifacts, then report final state
    for f in os.listdir(out):
        if f.startswith("_build_"):
            try:
                os.remove(os.path.join(out, f))
            except OSError:
                pass
    arts = sorted(f for f in os.listdir(out)
                  if f.rsplit(".", 1)[-1] in ("tex", "csv", "pdf"))
    pdfs = [f for f in arts if f.endswith(".pdf")]
    print(f"\n== {len(arts)} artifacts in {out} ({len(pdfs)} pdf)")
    for f in arts:
        print(f"   {f}")
    if failed:
        print(f"\n!! {len(failed)} builder(s) failed: {', '.join(failed)}")
        return 1
    print("\n== done")
    return 0


if __name__ == "__main__":
    sys.exit(main())
