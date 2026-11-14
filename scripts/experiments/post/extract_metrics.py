#!/usr/bin/env python3
"""Extract a single metrics row from one Verdict solve run directory.

Usage:
    extract_metrics.py <run_dir> [--key value ...]

Reads <run_dir>/run.json (final proof states) and <run_dir>/pipeline.log
(convergence table, demand stats, cost, API stalls), and prints a SECTIONED,
indented JSON object to stdout:

    config         harness knobs (--key value pairs that are config)
    run            identity & outcome (status, durations, convergence)
    proof_states   per-state function counts + bug count
    coordination   event tallies (cycles, publishes, demands, suspensions)
    cost           cost_usd, api_stall_min
    solve_summary  the final "Solve Summary" block lines from the sibling
                   solve.log (ANSI-stripped), for human reading in place

Consumers that need flat rows (post/aggregate.py via post/collect.sh) flatten
the dict-valued sections back into one row; solve_summary is dropped there.

Defensive by design: a run may be incomplete (timeout/crash) with a missing or
partial run.json / pipeline.log. Missing fields become null rather than errors,
so every run in the experiment grid yields a row.
"""

from __future__ import annotations

import json
import re
import sys
from collections import Counter
from datetime import datetime
from pathlib import Path


def _load_run_json(run_dir: Path) -> dict:
    p = run_dir / "run.json"
    if not p.exists():
        return {}
    try:
        return json.loads(p.read_text())
    except (json.JSONDecodeError, OSError):
        return {}


def _proof_state_counts(run: dict) -> dict:
    agents = run.get("agents") or {}
    c = Counter(str(a.get("proof_state", "?")).lower() for a in agents.values())
    n_bugs = 0
    for a in agents.values():
        vs = a.get("vo_summary") or {}
        bugs = vs.get("bugs") if isinstance(vs, dict) else None
        if isinstance(bugs, dict):
            n_bugs += len(bugs)
    # CONDITIONAL_OK is a derived label: LOCAL_OK with settled assume debts
    # (wake budget spent, residual hypotheses on callees). Subset of n_local_ok.
    n_conditional = sum(
        1
        for a in agents.values()
        if str(a.get("proof_state", "?")).lower() == "local_ok"
        and a.get("settled_assumes")
    )
    return {
        "n_functions": len(agents),
        "n_global_ok": c.get("global_ok", 0),
        "n_local_ok": c.get("local_ok", 0),
        "n_conditional": n_conditional,
        "n_draft": c.get("draft", 0),
        "n_dirty": c.get("dirty", 0),
        "n_failed": c.get("failed", 0),
        "n_bugs": n_bugs,
    }


def _run_json_duration_min(run: dict) -> float | None:
    st, fi = run.get("started_at"), run.get("finished_at")
    if not st or not fi:
        return None
    try:
        def p(t: str) -> datetime:
            return datetime.fromisoformat(t.replace("Z", "+00:00"))
        return round((p(fi) - p(st)).total_seconds() / 60, 2)
    except ValueError:
        return None


def _count_events(run_dir: Path) -> dict:
    """Tally event types from events.jsonl (demands, publishes, cycles)."""
    p = run_dir / "events.jsonl"
    out = {
        "n_cycles": None, "n_published": None, "n_noop": None,
        "demands_raised": None, "demands_satisfied": None,
        "demands_rejected": None, "suspensions": None, "releases": None,
    }
    if not p.exists():
        return out
    c: Counter = Counter()
    resolutions: Counter = Counter()
    try:
        for line in p.open():
            try:
                e = json.loads(line)
            except json.JSONDecodeError:
                continue
            t = e.get("type", "")
            c[t] += 1
            if t == "proof_demand_resolved":
                resolutions[(e.get("data") or {}).get("resolution", "?")] += 1
    except OSError:
        return out
    return {
        "n_cycles": c.get("agent_cycle_ended", 0),
        "n_published": c.get("spec_published", 0),
        "n_noop": c.get("spec_republish_noop", 0),
        "demands_raised": c.get("proof_demand_raised", 0),
        "demands_satisfied": resolutions.get("satisfied", 0),
        "demands_rejected": resolutions.get("rejected", 0),
        "suspensions": c.get("agent_suspended", 0),
        "releases": c.get("agent_released", 0),
    }


def _parse_pipeline(run_dir: Path) -> dict:
    """Extract batch count, cost, and API-stall time from pipeline.log."""
    p = run_dir / "pipeline.log"
    out = {"batches_to_converge": None, "cost_usd": None, "api_stall_min": None}
    if not p.exists():
        return out
    try:
        text = p.read_text(errors="replace")
    except OSError:
        return out

    batches = re.findall(r"batch (\d+) \(", text)
    if batches:
        out["batches_to_converge"] = max(int(b) for b in batches)

    m = re.search(r"TOTAL:\s*\$([0-9.]+)", text)
    if m:
        out["cost_usd"] = float(m.group(1))

    # API stalls: gaps > 30s between consecutive timestamped log lines.
    ts = []
    for line in text.splitlines():
        m = re.match(r"^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2},\d+)", line)
        if m:
            try:
                ts.append(datetime.strptime(m.group(1), "%Y-%m-%d %H:%M:%S,%f"))
            except ValueError:
                pass
    if len(ts) > 1:
        stall = sum(
            (ts[i] - ts[i - 1]).total_seconds()
            for i in range(1, len(ts))
            if (ts[i] - ts[i - 1]).total_seconds() > 30
        )
        out["api_stall_min"] = round(stall / 60, 2)
    return out


_ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")


def _solve_summary_lines(run_dir: Path) -> list[str]:
    """Final "Solve Summary" block from the harness-captured solve.log.

    run_solve.sh moves the run dir to <cell>/run next to <cell>/solve.log,
    so the sibling is the right default. Absent file -> empty list.
    """
    p = run_dir.parent / "solve.log"
    if not p.exists():
        return []
    try:
        text = p.read_text(errors="replace")
    except OSError:
        return []
    idx = text.rfind("Solve Summary")
    if idx == -1:
        return []
    lines = [_ANSI_RE.sub("", ln).rstrip() for ln in text[idx:].splitlines()]
    return lines[:1000]


def _status(run: dict, counts: dict) -> str:
    raw = str(run.get("status", "")).lower()
    if raw in {"complete", "completed"}:
        return "complete"
    if raw in {"timeout", "timed_out"}:
        return "timeout"
    if not run:
        return "no_run_json"
    return raw or "unknown"


def main() -> None:
    if len(sys.argv) < 2:
        print("usage: extract_metrics.py <run_dir> [--key value ...]", file=sys.stderr)
        sys.exit(2)
    run_dir = Path(sys.argv[1])

    def _coerce(v: str):
        """Numeric-looking CLI values become numbers so downstream aggregation
        (which filters for int/float) sees them; everything else stays a string."""
        try:
            return int(v)
        except ValueError:
            try:
                return float(v)
            except ValueError:
                return v

    extra: dict = {}
    args = sys.argv[2:]
    i = 0
    while i < len(args):
        if args[i].startswith("--") and i + 1 < len(args):
            extra[args[i][2:]] = _coerce(args[i + 1])
            i += 2
        else:
            i += 1

    run = _load_run_json(run_dir)
    counts = _proof_state_counts(run)
    pipeline = _parse_pipeline(run_dir)

    # Harness extras that are measurements, not configuration.
    measured = {k: extra.pop(k) for k in ("wall_clock_min", "solve_rc", "hostname")
                if k in extra}

    run_section: dict = {"run_dir": str(run_dir)}
    run_section.update(measured)
    run_section["status"] = _status(run, counts)
    run_section["run_json_duration_min"] = _run_json_duration_min(run)
    if counts["n_functions"]:
        run_section["converged"] = counts["n_global_ok"] == counts["n_functions"]
        run_section["global_ok_frac"] = round(
            counts["n_global_ok"] / counts["n_functions"], 4
        )
    else:
        run_section["converged"] = False
        run_section["global_ok_frac"] = None

    coordination = _count_events(run_dir)
    coordination["batches_to_converge"] = pipeline["batches_to_converge"]

    doc = {
        "config": extra,
        "run": run_section,
        "proof_states": counts,
        "coordination": coordination,
        "cost": {"cost_usd": pipeline["cost_usd"],
                 "api_stall_min": pipeline["api_stall_min"]},
        "solve_summary": _solve_summary_lines(run_dir),
    }
    print(json.dumps(doc, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
