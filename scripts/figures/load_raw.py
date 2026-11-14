#!/usr/bin/env python3
"""Unified raw-data loader for every RQ1/2/3 paper table and figure.

Single source of truth: every number in
the paper is derived *here* from the canonical artifacts
(judge_final.json / run.json / events.jsonl / provenance.txt / bench .c).
Each gen_*.py imports from this module and NEVER re-parses raw files or
hand-types a number.

Active tree only — the 14 paper benches live flat under each tool dir; dropped
benches are in results/unused/, contaminated cells in results/_quarantine/
(both excluded here). Grouping (RQ1 real-world/util-lib, RQ3 deep/shallow) is
applied logically from the maps below, not from the directory layout.
"""
from __future__ import annotations

import json
import os
import re
from datetime import datetime
from functools import lru_cache
from glob import glob
from pathlib import Path

# ── configuration ────────────────────────────────────────────────────────────

def _repo_root() -> Path:
    """Locate the Verdict repo root (holds benchmarks/ + scripts/experiments/),
    so the loader works no matter where the generator scripts live or run."""
    here = Path(__file__).resolve()
    for d in [here.parent, *here.parents]:
        if (d / "benchmarks").is_dir() and (d / "scripts" / "experiments").is_dir():
            return d
    return Path.cwd()


REPO_ROOT = _repo_root()
RESULTS = Path(os.environ.get("VERDICT_RESULTS",
                              REPO_ROOT / "scripts" / "experiments" / "results"))
BENCH_SRC = REPO_ROOT / "benchmarks"    # benchmarks/<domain>/<bench>/<file>.c

REAL14 = ["submodule-1", "submodule-2", "contiki-aes-ccm",
          "hypatia", "sokol-shape", "jc-voronoi"]
UTIL14 = ["ccan-time", "cgl", "char-utils", "maratis-math",
          "punity", "stb-divide", "student-enrollment", "zpl-math"]
B14 = REAL14 + UTIL14

RQ1_GROUP = {b: "real-world" for b in REAL14}
RQ1_GROUP.update({b: "util-lib" for b in UTIL14})

# The whole-module x509 parser is reported inside the Real-world group (it is a
# real-world program), sorted with the rest by #funcs; rep counts differ per
# tool but the mean±std formatting tolerates that.
REAL_WITH_FULL = REAL14 + ["full-module"]

# RQ3 uses a DIFFERENT axis (call-chain depth), on a 6-bench subset.
RQ3_CHAIN = {
    "submodule-1": "deep", "submodule-2": "deep",
    "contiki-aes-ccm": "shallow", "hypatia": "shallow",
    "jc-voronoi": "shallow", "sokol-shape": "shallow",
}
RQ2_BENCHES = ["submodule-1", "submodule-2", "contiki-aes-ccm",
               "hypatia", "jc-voronoi", "sokol-shape"]

# Display names aligned with the SpecSyn corpus module names (paper tables).
NAME_MAP = {
    "submodule-1": "x509-parser-1", "submodule-2": "x509-parser-2",
    "full-module": "x509-parser-full", "contiki-aes-ccm": "contiki",
    "hypatia": "hypatia", "sokol-shape": "sokol", "jc-voronoi": "voronoi",
    "ccan-time": "ccan", "cgl": "cgl", "char-utils": "clib",
    "maratis-math": "maratis", "punity": "punity", "stb-divide": "stb",
    "student-enrollment": "Java-JML", "zpl-math": "zpl",
}


def display_name(bench):
    return NAME_MAP.get(bench, bench)

MODELS = [("opus", "claude-opus-4-5-20251101"), ("sonnet", "claude-sonnet-4-6")]
MODEL_LONG = dict(MODELS)
REPS = ["rep1", "rep2", "rep3"]
ARMS = ["baseline", "no-assume", "no-demands", "no-demands-no-assume"]

MP_SWEEP = [1, 2, 4, 8, 16, 32]         # RQ2a (mc=24); mp=8 == RQ1 rep1 baseline
MC_SWEEP = [1, 2, 4, 8, 16, 24]         # RQ2b (mp=8); mc=24 == RQ1 rep1 baseline

# convergence rate = fraction of agents in an "ok" proof state (spec §1).
# Both global_ok and local_ok count as converged (internal self-report).
CONVERGENCE_STATES = ("global_ok", "local_ok")

# ── low-level artifact parsers ───────────────────────────────────────────────

def _jload(p):
    try:
        return json.loads(Path(p).read_text())
    except (OSError, json.JSONDecodeError):
        return None


def _dur_min(started, finished):
    if not started or not finished:
        return None
    try:
        def parse(t):
            return datetime.fromisoformat(t.replace("Z", "+00:00"))
        return round((parse(finished) - parse(started)).total_seconds() / 60, 2)
    except ValueError:
        return None


def judge(judge_path):
    """rte/spec/all -> valid/total/ratio, from judge_final.json.

    **spec includes callsite**: the json stores spec (postconditions, assigns,
    loop invariants) and callsite (instances of callee preconditions at call
    sites) separately. We merge them: spec_v = json.spec.valid + json.callsite.valid,
    spec_t = json.spec.total + json.callsite.total. This is the paper's
    "spec discharge" metric — everything the tool wrote beyond rte.

    The raw callsite_* fields are still populated for diagnostics."""
    d = _jload(judge_path)
    if not d:
        return None
    o = {"rc": d.get("frama_c_rc")}
    # Read all four raw categories from the json
    for c in ("rte", "callsite", "spec", "all"):
        s = d.get(c) or {}
        o[f"{c}_v"] = s.get("valid")
        o[f"{c}_t"] = s.get("total")
        o[f"{c}_r"] = s.get("ratio")
    # Merge callsite into spec: spec = spec + callsite (the paper metric)
    raw_sv, raw_st = o["spec_v"], o["spec_t"]
    cs_v, cs_t = o["callsite_v"] or 0, o["callsite_t"] or 0
    if raw_sv is not None and raw_st is not None:
        o["spec_v"] = raw_sv + cs_v
        o["spec_t"] = raw_st + cs_t
        o["spec_r"] = o["spec_v"] / o["spec_t"] if o["spec_t"] else None
    return o


def verdict_run(run_dir):
    """time/bugs/convergence/n_funcs from run.json (verdict only)."""
    out = {"time_min": None, "bugs": None, "convergence": None, "n_funcs": None}
    rj = _jload(Path(run_dir) / "run.json")
    if not rj:
        return out
    out["time_min"] = _dur_min(rj.get("started_at"), rj.get("finished_at"))
    agents = rj.get("agents") or {}
    out["n_funcs"] = len(agents)
    bugs = ok = 0
    for a in agents.values():
        vs = a.get("vo_summary") or {}
        b = vs.get("bugs") if isinstance(vs, dict) else None
        if isinstance(b, dict):
            bugs += len(b)
        if str(a.get("proof_state", "")).lower() in CONVERGENCE_STATES:
            ok += 1
    out["bugs"] = bugs
    out["convergence"] = round(ok / len(agents), 4) if agents else None
    return out


def events(run_dir):
    """demands raised/satisfied + cycles from events.jsonl (verdict only)."""
    out = {"demands_raised": None, "demands_satisfied": None, "cycles": None}
    p = Path(run_dir) / "events.jsonl"
    if not p.exists():
        return out
    raised = satisfied = cycles = 0
    try:
        for line in p.open():
            try:
                e = json.loads(line)
            except json.JSONDecodeError:
                continue
            t = e.get("type", "")
            if t == "proof_demand_raised":
                raised += 1
            elif t == "proof_demand_resolved" and (e.get("data") or {}).get("resolution") == "satisfied":
                satisfied += 1
            elif t == "agent_cycle_ended":
                cycles += 1
    except OSError:
        return out
    out.update(demands_raised=raised, demands_satisfied=satisfied, cycles=cycles)
    return out


def _wall_min(cell_dir):
    """Baseline wall-clock minutes from provenance.txt (preguss/specsyn)."""
    p = Path(cell_dir) / "provenance.txt"
    if not p.exists():
        return None
    m = re.search(r"wall_min=([0-9.]+)", p.read_text(errors="replace"))
    return round(float(m.group(1)), 2) if m else None

# ── benchmark metadata (computed once per bench) ─────────────────────────────

_RE_MAIN = re.compile(r"\b(?:int|void)\s+main\s*\(")  # synthetic main() harness (never agentized)


def _bench_c(bench):
    hits = sorted(glob(str(BENCH_SRC / "*" / bench / "*.c")))
    return hits[0] if hits else None


def _callgraph(agents):
    """(#funcs, #call-edges, max-call-depth) over the internal call graph.
    callees are function names; only edges to other in-set functions count."""
    names = {a.get("function_name") for a in agents.values()}
    edges = {a.get("function_name"): [c for c in (a.get("callees") or []) if c in names]
             for a in agents.values()}
    n_edges = sum(len(v) for v in edges.values())

    depth_memo: dict[str, int] = {}
    def depth(fn, stack):
        if fn in depth_memo:
            return depth_memo[fn]
        if fn in stack:               # cycle guard
            return 0
        best = 0
        for c in edges.get(fn, []):
            best = max(best, 1 + depth(c, stack | {fn}))
        depth_memo[fn] = best
        return best
    max_depth = max((depth(fn, set()) for fn in edges), default=0)
    return len(agents), n_edges, max_depth


@lru_cache(maxsize=None)
def bench_meta(bench):
    """{loc, n_funcs, n_funcs_total, call_edges, max_depth, n_rte, group, chain}.

    n_funcs counts the functions VERDICT agentizes (the parallel units consumed
    by RQ2); n_funcs_total adds the synthetic main() harness when the source
    defines one, giving the program's full function count for the
    RQ1 table. They differ by exactly 1 on benches with a
    main() (which is never agentized).
    """
    meta = {"bench": bench, "group": RQ1_GROUP.get(bench),
            "chain": RQ3_CHAIN.get(bench), "loc": None, "n_funcs": None,
            "n_funcs_total": None, "call_edges": None, "max_depth": None,
            "n_rte": None}
    src = _bench_c(bench)
    src_text = Path(src).read_text(errors="replace") if src else ""
    if src:
        meta["loc"] = src_text.count("\n")  # wc -l semantics
    # #funcs + call graph from a verdict rq1 run.json (prefer opus rep1)
    for model, _ in MODELS:
        for rep in REPS:
            rj = _jload(RESULTS / "rq1" / f"verdict-{model}" / bench / rep / "run" / "run.json")
            if rj and rj.get("agents"):
                meta["n_funcs"], meta["call_edges"], meta["max_depth"] = _callgraph(rj["agents"])
                break
        if meta["n_funcs"]:
            break
    if meta["n_funcs"] is not None:
        meta["n_funcs_total"] = meta["n_funcs"] + (1 if _RE_MAIN.search(src_text) else 0)
    # #rte = judge rte.total (anchored to bench; identical across tools)
    for model, _ in MODELS:
        for rep in REPS:
            j = judge(RESULTS / "rq1" / f"verdict-{model}-scores" / bench / rep / "judge_final.json")
            if j and j["rte_t"] is not None:
                meta["n_rte"] = j["rte_t"]
                break
        if meta["n_rte"] is not None:
            break
    return meta

# ── uniform cell row ─────────────────────────────────────────────────────────

_ROW_KEYS = ("tool", "model", "bench", "rq", "rep", "config",
             "rte_v", "rte_t", "rte_r", "callsite_v", "callsite_t", "callsite_r",
             "spec_v", "spec_t", "spec_r", "all_v", "all_t", "all_r",
             "time_min", "bugs", "convergence",
             "demands_raised", "demands_satisfied", "cycles", "present")


def _row(**kw):
    r = {k: None for k in _ROW_KEYS}
    r["present"] = False
    r.update(kw)
    return r


def _merge_judge(row, j):
    if j:
        for c in ("rte", "callsite", "spec", "all"):
            for s in ("v", "t", "r"):
                row[f"{c}_{s}"] = j[f"{c}_{s}"]
        row["present"] = True
    return row

# ── cell accessors (return a uniform row) ────────────────────────────────────

def verdict_cell(model, bench, *, rep=None, rq2=None, arm=None):
    """One verdict cell. Exactly one selector:
       rep='rep1'.. (RQ1) | rq2=(kind,key) (RQ2) | arm='no-assume'.. (RQ3)."""
    if rep is not None:
        rq, cfg = "rq1", rep
        run = RESULTS / "rq1" / f"verdict-{model}" / bench / rep / "run"
        jp = RESULTS / "rq1" / f"verdict-{model}-scores" / bench / rep / "judge_final.json"
    elif rq2 is not None:
        kind, key = rq2
        rq, cfg = ("rq2a" if kind == "parallel" else "rq2b"), key
        run = RESULTS / "rq2" / model / kind / bench / key / "rep1" / "run"
        jp = RESULTS / "rq2" / f"{model}-scores" / kind / bench / key / "rep1" / "judge_final.json"
    elif arm is not None:
        rq, cfg = "rq3", arm
        run = RESULTS / "rq3" / model / bench / arm / "rep1" / "run"
        jp = RESULTS / "rq3" / f"{model}-scores" / bench / arm / "rep1" / "judge_final.json"
    else:
        raise ValueError("verdict_cell: need rep, rq2, or arm")
    row = _row(tool="verdict", model=model, bench=bench, rq=rq, rep="rep1", config=cfg)
    _merge_judge(row, judge(jp))
    row.update({k: v for k, v in verdict_run(run).items() if k != "n_funcs"})
    row.update(events(run))
    if any(row[k] is not None for k in ("time_min", "all_r")):
        row["present"] = True
    return row


def baseline_cell(tool, model, bench, rep):
    """preguss|specsyn cell: judge from -scores, time from raw provenance.txt."""
    jp = RESULTS / f"baseline-{tool}-scores" / bench / f"{model}-{rep}" / "judge_final.json"
    cell = RESULTS / f"baseline-{tool}" / bench / MODEL_LONG[model] / rep
    row = _row(tool=tool, model=model, bench=bench, rq="rq1", rep=rep, config=None)
    _merge_judge(row, judge(jp))
    row["time_min"] = _wall_min(cell)
    if row["time_min"] is not None:
        row["present"] = True
    return row

# ── series helpers (RQ1 rep1 baseline injected where the sweep needs it) ──────

def rq2_parallel(model, bench):
    """[(mp, row)] over MP_SWEEP; mp=8 is the RQ1 rep1 baseline."""
    out = []
    for mp in MP_SWEEP:
        if mp == 8:
            out.append((mp, verdict_cell(model, bench, rep="rep1")))
        else:
            out.append((mp, verdict_cell(model, bench, rq2=("parallel", f"mp{mp:02d}-mc24"))))
    return out


_BATCH_RX = re.compile(r"batch \d+ \([\w-]+\): (\d+) agents, ([\d.]+)m")


def _parallel_run(model, bench, mp):
    """Run dir for a parallel sweep point (mp=8 falls back to the RQ1 baseline)."""
    if mp == 8:
        return RESULTS / "rq1" / f"verdict-{model}" / bench / "rep1" / "run"
    return RESULTS / "rq2" / model / "parallel" / bench / f"mp{mp:02d}-mc24" / "rep1" / "run"


def rq2_batches(model, bench, mp):
    """[(n_agents, minutes)] per scheduling wave from pipeline.log's 'Per-batch
    convergence' block; [] if absent. Wave 1 is the initial (leaf) burst; waves
    2+ are the reactivation cascade (assume/demand resolution). The orchestrator
    logs this verbatim (orchestrator.py); we only read it."""
    p = _parallel_run(model, bench, mp) / "pipeline.log"
    if not p.exists():
        return []
    out = []
    for line in p.open(errors="replace"):
        m = _BATCH_RX.search(line)
        if m:
            out.append((int(m.group(1)), float(m.group(2))))
    return out


def rq2_cycle(model, bench):
    """[(mc, row)] over MC_SWEEP; mc=24 is the RQ1 rep1 baseline."""
    out = []
    for mc in MC_SWEEP:
        if mc == 24:
            out.append((mc, verdict_cell(model, bench, rep="rep1")))
        else:
            out.append((mc, verdict_cell(model, bench, rq2=("cycle", f"mp08-mc{mc:02d}"))))
    return out


def rq3_arms(model, bench):
    """{arm: row}; 'baseline' is the RQ1 rep1 cell."""
    out = {"baseline": verdict_cell(model, bench, rep="rep1")}
    for arm in ARMS[1:]:
        out[arm] = verdict_cell(model, bench, arm=arm)
    return out

# ── bulk RQ1 loader (verdict + both baselines, all reps) ─────────────────────

def load_rq1():
    rows = []
    for model, _ in MODELS:
        for bench in B14:
            for rep in REPS:
                rows.append(verdict_cell(model, bench, rep=rep))
                rows.append(baseline_cell("preguss", model, bench, rep))
                rows.append(baseline_cell("specsyn", model, bench, rep))
    return rows

# ── self-test ────────────────────────────────────────────────────────────────

def _selftest():
    print("RESULTS =", RESULTS.resolve())
    print("\n── bench_meta (compare to spec §4 reference) ──")
    print(f"{'bench':20}{'grp':12}{'chain':8}{'#fn':>5}{'edges':>7}{'depth':>7}{'LoC':>7}{'#rte':>7}")
    for b in sorted(B14, key=lambda x: -(bench_meta(x)["n_funcs"] or 0)):
        m = bench_meta(b)
        print(f"{b:20}{str(m['group']):12}{str(m['chain'] or '-'):8}"
              f"{m['n_funcs'] or 0:5}{m['call_edges'] or 0:7}{m['max_depth'] or 0:7}"
              f"{m['loc'] or 0:7}{m['n_rte'] or 0:7}")
    rows = load_rq1()
    present = [r for r in rows if r["present"]]
    print(f"\n── RQ1 rows: {len(present)}/{len(rows)} present ──")
    for tool in ("verdict", "preguss", "specsyn"):
        n = sum(1 for r in present if r["tool"] == tool)
        print(f"   {tool:9}: {n}  (expect 84 = 14 bench × 2 model × 3 rep)")
    # spot-check one verdict cell end to end
    c = verdict_cell("opus", "submodule-1", rep="rep1")
    print(f"\n── spot: opus/submodule-1/rep1 → all={c['all_v']}/{c['all_t']} "
          f"({(c['all_r'] or 0)*100:.1f}%) time={c['time_min']}m bugs={c['bugs']} "
          f"cr={c['convergence']} dem={c['demands_raised']}/{c['demands_satisfied']} cyc={c['cycles']}")
    # spot-check RQ2 series + baseline injection
    par = rq2_parallel("opus", "submodule-2")
    print("── spot: opus/submodule-2 RQ2a time by mp:",
          ", ".join(f"mp{mp}={r['time_min']}" for mp, r in par))


if __name__ == "__main__":
    _selftest()
