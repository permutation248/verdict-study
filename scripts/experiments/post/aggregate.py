#!/usr/bin/env python3
"""Aggregate RQ results: group by (benchmark, <config column>) across reps.

Usage: aggregate.py <summary.jsonl> [--by max_parallel|model|...]

The --by column is the RQ's independent variable: max_parallel for RQ2
(default), model for RQ1. Prints a table of median/mean wall-clock and
convergence per config. Reads the summary.jsonl built by post/collect.sh
(one row per run).
"""

from __future__ import annotations

import json
import statistics
import sys
from collections import defaultdict


def _num(xs):
    return [x for x in xs if isinstance(x, (int, float))]


def _sort_key(value):
    """Numeric sort when the column is numeric (mp: 1,2,4,8,16 not 1,16,2...)."""
    try:
        return (0, float(value), "")
    except (TypeError, ValueError):
        return (1, 0.0, str(value))


def main() -> None:
    args = sys.argv[1:]
    by = "max_parallel"
    if "--by" in args:
        i = args.index("--by")
        try:
            by = args[i + 1]
        except IndexError:
            print("usage: aggregate.py <summary.jsonl> [--by <column>]",
                  file=sys.stderr)
            sys.exit(2)
        del args[i:i + 2]
    if len(args) < 1:
        print("usage: aggregate.py <summary.jsonl> [--by <column>]",
              file=sys.stderr)
        sys.exit(2)

    def _flatten(row: dict) -> dict:
        """Sectioned metrics (config/run/... sub-dicts) -> one flat row.
        Pre-sectioning flat rows pass through unchanged; list-valued keys
        (solve_summary) are dropped."""
        flat: dict = {}
        for k, v in row.items():
            if isinstance(v, dict):
                flat.update(v)
            elif not isinstance(v, list):
                flat[k] = v
        return flat

    rows = []
    with open(args[0]) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                rows.append(_flatten(json.loads(line)))
            except json.JSONDecodeError:
                continue

    groups = defaultdict(list)
    for r in rows:
        groups[(r.get("benchmark", "?"), str(r.get(by, "?")))].append(r)

    by_w = max([len(by)] + [len(k[1]) for k in groups]) + 2
    hdr = (
        f"{'benchmark':<14}{by:>{by_w}}{'reps':>5}{'wall_med':>10}"
        f"{'wall_mean':>10}{'g_ok_med':>9}{'conv':>6}{'cyc_med':>9}"
        f"{'dem_med':>9}{'stall_med':>10}"
    )
    print(hdr)
    print("-" * len(hdr))
    for key in sorted(groups, key=lambda k: (k[0], _sort_key(k[1]))):
        bench, by_val = key
        g = groups[key]
        wall = _num([r.get("wall_clock_min") for r in g])
        gok = _num([r.get("global_ok_frac") for r in g])
        cyc = _num([r.get("n_cycles") for r in g])
        dem = _num([r.get("demands_raised") for r in g])
        stall = _num([r.get("api_stall_min") for r in g])
        conv = sum(1 for r in g if r.get("converged"))

        def med(xs, fmt=".1f"):
            return f"{statistics.median(xs):{fmt}}" if xs else "-"

        def mean(xs):
            return f"{statistics.mean(xs):.1f}" if xs else "-"

        print(
            f"{bench:<14}{by_val:>{by_w}}{len(g):>5}{med(wall):>10}{mean(wall):>10}"
            f"{med(gok, '.2f'):>9}{f'{conv}/{len(g)}':>6}{med(cyc):>9}"
            f"{med(dem):>9}{med(stall):>10}"
        )


if __name__ == "__main__":
    main()
