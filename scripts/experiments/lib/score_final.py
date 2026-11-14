#!/usr/bin/env python3
"""Unified final judge: score ONE tool's annotated C file with Frama-C WP.

Scores the FINAL annotated output of any tool (Verdict final_annotated.c,
Preguss output .c, SpecSyn restored.annotated.c) under ONE uniform judge
configuration, so the numbers are comparable across tools:

    frama-c -wp <file> -wp-no-init-const -wp-model Typed+var+int+real
            -wp-par 8 -wp-timeout 30 -wp-prover alt-ergo,z3,cvc5
            -kernel-warn-key annot-error=active -then -report

Agreed evaluation rules (2026-06-12):
  1. evaluation semantics = Typed+var+int+real for every tool (SpecSyn's own
     published model; strictly charitable to baselines). `--preset sound`
     scores under the default Typed model instead (supplementary column).
  2. main() is INCLUDED: WP runs over the whole file (skip_main=False, no
     -wp-skip-fct) and every reported property counts toward the denominators,
     including those under "Properties of Function 'main'" — the harness's RTE
     asserts, the callsite obligations of its calls, and its own contract.
  3. uniform prover budget: -wp-par 8, -wp-timeout 30, alt-ergo,z3,cvc5.
  4. three failure categories, counted separately:
       rte       Assertion 'rte_N,<kind>' (the shared injected targets —
                 the only category with an identical denominator per bench)
       callsite  instances of callee preconditions at call points
       spec      everything else the tool wrote: post-conditions, assigns,
                 loop invariants/assigns/variants, its own asserts, ...
  5. ratios per category and pooled.

Property accounting:
  - proven iff consolidated status is exactly "Valid" (same rule as the
    SpecSyn artifact's verify_all.py).
  - "Considered valid" entries are hypotheses (e.g. a function's own
    requires at its definition), not goals: excluded from denominators.
  - "Extern" entries are contract clauses on bodiless (extern) functions:
    unverifiable assumptions the tool ships. Excluded from denominators,
    counted in the ledger as extern_contract_clauses.
  - rte denominator hardening: rte_N labels are also scanned from the
    source text; a label that never shows up in the report is counted as
    an unproven rte ("missing") with a warning, so kernel drops can only
    hurt, never silently shrink the total.
  - unverified-assumption ledger: `axiomatic` blocks and `admit` clauses in
    the source are counted and reported (the judge cannot check them).

Usage:
    score_final.py final_annotated.c --json out.json --detail-csv detail.csv
    score_final.py restored.annotated.c --preset sound --json sound.json
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import subprocess
import sys
import time

RTE_SOURCE_RE = re.compile(r"/\*@\s*assert\s+(rte_\d+)\s*:")
RTE_PROP_RE = re.compile(r"^Assertion 'rte_(\d+)")
# Anchored at column 0: pending-dependency continuation lines under
# [ Partial ] entries are indented and must not match.
REPORT_LINE_RE = re.compile(
    r"^\[(?P<status>[^\]]*)\]\s*(?P<prop>.*?)\s*\(file (?P<file>.*?), line (?P<line>\d+)\)")
FUNC_HEADER_RE = re.compile(r"^--- Properties of Function '(?P<func>.*)'")
CALLSITE_PROP_RE = re.compile(r"^Instance of '")
AXIOMATIC_RE = re.compile(r"\baxiomatic\b")
ADMIT_RE = re.compile(r"\badmit\b")


def build_command(path: str, args: argparse.Namespace, *,
                  skip_main: bool) -> list[str]:
    cmd = [args.frama_c, "-wp", path]
    cmd += ["-wp-no-init-const",
            "-kernel-warn-key", "annot-error=active",
            "-wp-timeout", str(args.wp_timeout),
            "-wp-par", str(args.wp_par),
            "-wp-prover", args.prover]
    if args.preset == "final":
        cmd += ["-wp-model", "Typed+var+int+real"]
    # sound preset: default Typed model, nothing to add
    cmd += ["-then", "-report"]
    return cmd


def classify(prop: str) -> str:
    if RTE_PROP_RE.match(prop):
        return "rte"
    if CALLSITE_PROP_RE.match(prop):
        return "callsite"
    return "spec"


def score_file(path: str, args: argparse.Namespace) -> dict:
    with open(path, "r", errors="replace") as fp:
        source = fp.read()
    source_rte = set(RTE_SOURCE_RE.findall(source))
    # The rte denominator belongs to the BENCH, not to whatever survives in
    # the tool's output: with --expect-rte-from the label set is scanned
    # from the original input file, so a tool that drops targets is charged
    # for them instead of silently shrinking the total.
    #
    # Counting note (2026-06-12): frama-c's pretty-printer WRAPS long
    # asserts across physical lines ("assert" and "rte_N:" then sit on
    # different lines). Label scans here use \s+ and are wrap-tolerant; any
    # line-anchored count (grep "assert rte_") undercounts wrapped asserts
    # — that artifact produced a false 91-vs-142 alarm once. All three
    # tools' artifacts carry the full label set.
    if args.expect_rte_from:
        with open(args.expect_rte_from, "r", errors="replace") as fp:
            source_rte = set(RTE_SOURCE_RE.findall(fp.read()))
    has_main = re.search(r"\b(int|void)\s+main\s*\(", source) is not None

    cmd = build_command(path, args, skip_main=False)
    t0 = time.time()
    proc = subprocess.run(cmd, capture_output=True, text=True,
                          timeout=args.proc_timeout or None)
    elapsed = time.time() - t0
    out = proc.stdout + "\n" + proc.stderr
    if args.keep_log:
        with open(args.keep_log, "w") as fp:
            fp.write(" ".join(cmd) + "\n\n" + out)

    rows: list[dict] = []
    seen_rte: set[str] = set()
    extern_clauses = 0
    func = None
    for line in out.splitlines():
        mh = FUNC_HEADER_RE.match(line)
        if mh:
            func = mh.group("func")
            continue
        m = REPORT_LINE_RE.match(line)
        if not m:
            continue
        status = m.group("status").strip()
        prop = m.group("prop").strip()
        if status == "Considered valid":   # hypothesis, not a goal
            continue
        if status == "Extern":
            # Contract clause on a bodiless (extern) function: nothing to
            # verify against, it is an ASSUMPTION the tool ships. Goes to
            # the unverified-assumption ledger, not the denominators.
            extern_clauses += 1
            continue
        cat = classify(prop)
        if cat == "rte":
            seen_rte.add("rte_" + RTE_PROP_RE.match(prop).group(1))
        rows.append({
            "function": func or "", "category": cat, "property": prop,
            "status": status, "valid": status == "Valid",
            "line": int(m.group("line")),
        })

    # rte labels expected but absent from the report were dropped by the
    # tool; they are counted as unproven ("missing") so a tool cannot shrink
    # its own denominator by emitting fewer targets than the bench defines.
    missing_rte = sorted(source_rte - seen_rte, key=lambda s: int(s[4:]))
    count_missing = True
    if count_missing:
        for label in missing_rte:
            rows.append({"function": "?", "category": "rte",
                         "property": f"Assertion '{label}' (missing from report)",
                         "status": "missing", "valid": False, "line": 0})

    summary: dict = {
        "file": path,
        "preset": args.preset,
        "wp_model": "Typed+var+int+real" if args.preset == "final" else "Typed (default)",
        "wp_timeout": args.wp_timeout, "wp_par": args.wp_par,
        "prover": args.prover,
        "frama_c_rc": proc.returncode,
        "elapsed_s": round(elapsed, 1),
        "axiomatic_blocks": len(AXIOMATIC_RE.findall(source)),
        "admit_clauses": len(ADMIT_RE.findall(source)),
        "extern_contract_clauses": extern_clauses,
        "rte_in_source": len(source_rte),
        "rte_missing_from_report": missing_rte,
        "rte_missing_counted": count_missing,
    }
    for cat in ("rte", "callsite", "spec"):
        sub = [r for r in rows if r["category"] == cat]
        ok = sum(r["valid"] for r in sub)
        summary[cat] = {"total": len(sub), "valid": ok,
                        "unproved": len(sub) - ok,
                        "ratio": round(ok / len(sub), 4) if sub else None}
    ok = sum(r["valid"] for r in rows)
    summary["all"] = {"total": len(rows), "valid": ok,
                      "unproved": len(rows) - ok,
                      "ratio": round(ok / len(rows), 4) if rows else None}
    summary["_rows"] = rows
    return summary


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("path", help="annotated .c file to judge")
    parser.add_argument("--preset", choices=["final", "sound"], default="final",
                        help="final = Typed+var+int+real (paper rule); "
                             "sound = default Typed (supplementary)")
    parser.add_argument("--wp-timeout", type=int, default=30)
    parser.add_argument("--wp-par", type=int, default=8)
    parser.add_argument("--prover", default="alt-ergo,z3,cvc5")
    parser.add_argument("--frama-c", dest="frama_c", default="frama-c")
    parser.add_argument("--expect-rte-from", dest="expect_rte_from",
                        help="scan the expected rte_N label set from this "
                             "file (the bench input) instead of the scored "
                             "file; dropped targets then count as unproved")
    parser.add_argument("--proc-timeout", type=int, default=0,
                        help="hard kill for the whole frama-c run (s); 0 = none")
    parser.add_argument("--json", help="write summary JSON here")
    parser.add_argument("--detail-csv", help="write per-property CSV here")
    parser.add_argument("--keep-log", help="write raw frama-c output here")
    args = parser.parse_args()

    summary = score_file(args.path, args)
    rows = summary.pop("_rows")

    if args.detail_csv:
        with open(args.detail_csv, "w", newline="") as fp:
            w = csv.DictWriter(fp, fieldnames=["function", "category", "property",
                                               "status", "valid", "line"])
            w.writeheader()
            w.writerows(rows)
    if args.json:
        with open(args.json, "w") as fp:
            json.dump(summary, fp, indent=2)

    for cat in ("rte", "callsite", "spec", "all"):
        s = summary[cat]
        ratio = f"{s['ratio']:.2%}" if s["ratio"] is not None else "n/a"
        print(f"{cat:9s} {s['valid']}/{s['total']} valid ({ratio}), "
              f"unproved {s['unproved']}")
    if summary["rte_missing_from_report"]:
        flag = "counted as unproved" if summary["rte_missing_counted"] \
            else "NOT counted (file has main; likely main-internal)"
        print(f"note: {len(summary['rte_missing_from_report'])} source rte "
              f"labels missing from report — {flag}", file=sys.stderr)
    if summary["axiomatic_blocks"] or summary["admit_clauses"] \
            or summary["extern_contract_clauses"]:
        print(f"note: unverified assumptions — "
              f"{summary['extern_contract_clauses']} extern contract clause(s), "
              f"{summary['axiomatic_blocks']} axiomatic block(s), "
              f"{summary['admit_clauses']} admit clause(s)", file=sys.stderr)
    return 0 if summary["frama_c_rc"] == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
