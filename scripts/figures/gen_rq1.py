#!/usr/bin/env python3
"""RQ1 comparison table, unified metric scheme:
report only rte and spec discharge rates + wall-clock time (NO callsite, NO
pooled 'all'). One combined table: each benchmark shows BOTH models (opus,
sonnet) on two rows; leading #Fn/Dep/LoC columns; a bottom Overall aggregate.
\\best (bold, mean only) marks the best rte%, spec%, and fastest time across the
three tools per row. All benchmarks are listed in one block (no grouping),
sorted by \\textsc{Verdict}'s Opus wall-clock (hardest first); each \\textsc{Verdict}
time appends its speedup over each baseline (green if faster, gray if slower).

Numbers come only from load_raw. Usage: gen_rq1.py <out_dir> [--commit C --date D]
"""
from __future__ import annotations
import argparse
import csv
import math
import os
import statistics
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import load_raw as L           # noqa: E402
import paper_style as S        # noqa: E402

TOOLS = ["verdict", "preguss", "specsyn"]
HEAD = {"verdict": "\\nblue{\\textsc{Verdict}}", "preguss": "\\preguss",
        "specsyn": "SpecSyn"}
ALL_BENCHES = L.REAL_WITH_FULL + L.UTIL14


def collect(model, bench, tool):
    """Per-rep lists of rte% / spec% / time, plus summed valid/total goals."""
    v = {"rte": [], "spec": [], "time": [], "rv": 0, "rt": 0, "sv": 0, "st": 0}
    for rep in L.REPS:
        c = (L.verdict_cell(model, bench, rep=rep) if tool == "verdict"
             else L.baseline_cell(tool, model, bench, rep))
        if not c["present"]:
            continue
        if c["rte_r"] is not None:
            v["rte"].append(c["rte_r"] * 100); v["rv"] += c["rte_v"]; v["rt"] += c["rte_t"]
        if c["spec_r"] is not None:
            v["spec"].append(c["spec_r"] * 100); v["sv"] += c["spec_v"]; v["st"] += c["spec_t"]
        if c["time_min"] is not None:
            v["time"].append(c["time_min"])
    return v


def _mean(xs):
    return statistics.mean(xs) if xs else None


def _winners(means, maximize, tol=0.05):
    vals = [(i, m) for i, m in enumerate(means) if m is not None]
    if not vals:
        return set()
    target = max(m for _, m in vals) if maximize else min(m for _, m in vals)
    return {i for i, m in vals if abs(m - target) <= tol}


def _model_cells(model, bench):
    """9 mean±std cells (rte, spec, time per tool); only best mean bolded.
    \\textsc{Verdict}'s time cell also appends its per-row speedup over each baseline."""
    data = {t: collect(model, bench, t) for t in TOOLS}
    rte_w = _winners([_mean(data[t]["rte"]) for t in TOOLS], True)
    spec_w = _winners([_mean(data[t]["spec"]) for t in TOOLS], True)
    time_w = _winners([_mean(data[t]["time"]) for t in TOOLS], False)
    vt = _mean(data["verdict"]["time"])
    spds = [bt / vt for bt in (_mean(data["preguss"]["time"]),
            _mean(data["specsyn"]["time"])) if bt and vt]
    cells = []
    for ti, t in enumerate(TOOLS):
        d = data[t]
        cells.append(S.meanstd(d["rte"], bold=ti in rte_w))
        cells.append(S.meanstd(d["spec"], bold=ti in spec_w))
        cells.append(S.meanstd(d["time"], digits=0, bold=ti in time_w))
        if t == "verdict":
            cells.append(_spd(spds) if spds else "")
    return cells


def _agg(model, tool):
    """Overall: micro-avg rte%/spec% (Σvalid/Σtotal) + geo-mean time over all benches."""
    rv = rt = sv = st = 0
    times = []
    for b in ALL_BENCHES:
        d = collect(model, b, tool)
        rv += d["rv"]; rt += d["rt"]; sv += d["sv"]; st += d["st"]
        if d["time"]:
            times.append(statistics.mean(d["time"]))
    rte = rv / rt * 100 if rt else None
    spec = sv / st * 100 if st else None
    tg = math.exp(statistics.mean([math.log(t) for t in times if t > 0])) if times else None
    return rte, spec, tg


def _v(x, bold=False, digits=1):
    if x is None:
        return "--"
    s = f"{x:.{digits}f}"
    return S.best(s) if bold else s


def _spd(ratios):
    """\\textsc{Verdict}'s speedup over each baseline (baseline time /
    \\textsc{Verdict}'s), appended to \\textsc{Verdict}'s time in italic and coloured
    per factor: green when \\textsc{Verdict} is faster ($\\geq 1$), gray when slower."""
    parts = []
    for r in ratios:
        c = "ForestGreen" if r >= 1 else "black!45"
        parts.append(f"{{\\color{{{c}}}\\itshape {r:.1f}$\\times$}}")
    return ",\\,".join(parts)


def _overall_cells(model):
    aggs = {t: _agg(model, t) for t in TOOLS}
    rte_w = _winners([aggs[t][0] for t in TOOLS], True)
    spec_w = _winners([aggs[t][1] for t in TOOLS], True)
    time_w = _winners([aggs[t][2] for t in TOOLS], False)
    vt = aggs["verdict"][2]                       # speedup reference
    spds = [aggs[t][2] / vt for t in ("preguss", "specsyn") if aggs[t][2] and vt]
    cells = []
    for ti, t in enumerate(TOOLS):
        r, s, tg = aggs[t]
        cells += [_v(r, ti in rte_w), _v(s, ti in spec_w), _v(tg, ti in time_w, 0)]
        if t == "verdict":
            cells.append(_spd(spds) if spds else "")
    return cells


def _loc(n):
    return f"{n:,}".replace(",", "\\,") if n is not None else "--"


SEP = {"space": r"\addlinespace[3.5pt]", "rule": r"\cmidrule(l){1-15}", "none": ""}


def _vtime(bench):
    """\textsc{Verdict}'s Opus wall-clock, the row-sort key (largest = hardest
    first); Opus is used because Sonnet wall-clock carries API noise."""
    return _mean(collect("opus", bench, "verdict")["time"]) or 0.0


def build_table(sep="space") -> str:
    out = []
    P = out.append
    P(r"\begin{tabular}{@{}l rrr l rrrr rrr rrr@{}}")
    P(r"\toprule")
    P(r"& & & & & \multicolumn{4}{c}{" + HEAD["verdict"] + r"} "
      r"& \multicolumn{3}{c}{" + HEAD["preguss"] + r"} "
      r"& \multicolumn{3}{c}{" + HEAD["specsyn"] + r"} \\")
    P(r"\cmidrule(lr){6-9}\cmidrule(lr){10-12}\cmidrule(lr){13-15}")
    P(r"\textbf{Bench} & \textbf{\#Fn} & \textbf{Dep} & \textbf{LoC} "
      r"& \textbf{Model} & rte & spec & time\,{\scriptsize(min)} & speedup "
      r"& rte & spec & time\,{\scriptsize(min)} "
      r"& rte & spec & time\,{\scriptsize(min)} \\")
    P(r"\midrule")
    sepcmd = SEP[sep]
    metas = sorted((L.bench_meta(b) for b in ALL_BENCHES),
                   key=lambda m: -_vtime(m["bench"]))
    for bi, m in enumerate(metas):
        def mr(v):
            return f"\\multirow{{2}}{{*}}{{{v}}}"
        name = mr(f"\\texttt{{{L.display_name(m['bench'])}}}")
        head = (f"{name} & {mr(m['n_funcs_total'])} & {mr(m['max_depth'])} "
                f"& {mr(_loc(m['loc']))}")
        P(f"{head} & opus & " + " & ".join(_model_cells('opus', m['bench'])) + r" \\")
        P(r" & & & & sonnet & " + " & ".join(_model_cells('sonnet', m['bench'])) + r" \\")
        if bi < len(metas) - 1 and sepcmd:
            P(sepcmd)
    P(r"\midrule")
    P(r"\multirow{2}{*}{\emph{Overall}} & & & & opus & "
      + " & ".join(_overall_cells('opus')) + r" \\")
    P(r" & & & & sonnet & " + " & ".join(_overall_cells('sonnet')) + r" \\")
    P(r"\bottomrule")
    P(r"\end{tabular}")
    return "\n".join(out)


CAPTION = (r"RQ1 specification quality and verification time. "
           r"\textbf{\#Fn}/\textbf{Dep}/\textbf{LoC}: functions, call-graph "
           r"depth, lines of code. Each tool cell is mean$\pm$std over 3 "
           r"repetitions; \textbf{rte}/\textbf{spec}: \% of runtime-error / "
           r"tool-written specification goals discharged by the unified judge; "
           r"\textbf{time}: end-to-end minutes. \emph{Overall} is the "
           r"goal-weighted mean (rte/spec) and geometric-mean time over all "
           r"benchmarks. \best{Bold} marks the best rte\%, spec\%, and fastest "
           r"time across tools per row. Rows are ordered by \textsc{Verdict}'s "
           r"Opus verification time (hardest first); the \textbf{speedup} column gives "
           r"\textsc{Verdict}'s factor over \preguss then SpecSyn "
           r"({\color{ForestGreen}green} where \textsc{Verdict} is faster, "
           r"{\color{black!45}gray} where slower).")


def build(out_dir, commit, date, sep="space"):
    os.makedirs(out_dir, exist_ok=True)
    body = build_table(sep)
    tex = (S.provenance(commit, str(L.RESULTS), date) +
           "\\begin{table*}[t]\n\\centering\n\\scriptsize\n"
           f"\\caption{{{CAPTION}}}\n\\label{{tab:rq1}}\n"
           "\\begin{adjustbox}{max width=\\textwidth}\n"
           f"{body}\n\\end{{adjustbox}}\n\\end{{table*}}\n")
    with open(os.path.join(out_dir, "rq1.tex"), "w") as fp:
        fp.write(tex)
    _write_csv(os.path.join(out_dir, "rq1.csv"))
    S.render("\\scriptsize\n" + body, "rq1", out_dir)   # -> rq1.pdf (clean)
    print(f"wrote rq1.tex (sep={sep}) / rq1.csv / rq1.pdf to {out_dir}")


def _write_csv(path):
    with open(path, "w", newline="") as fp:
        w = csv.writer(fp)
        w.writerow(["model", "bench", "tool", "rte_mean", "rte_std",
                    "spec_mean", "spec_std", "time_mean", "time_std"])
        for model in ("opus", "sonnet"):
            for b in ALL_BENCHES:
                for t in TOOLS:
                    d = collect(model, b, t)
                    def ms(xs):
                        return (round(statistics.mean(xs), 2) if xs else "",
                                round(statistics.pstdev(xs), 2) if len(xs) > 1 else 0)
                    rm, rs = ms(d["rte"]); sm, ss = ms(d["spec"]); tm, ts = ms(d["time"])
                    w.writerow([model, b, t, rm, rs, sm, ss, tm, ts])


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("out_dir")
    ap.add_argument("--commit", default="HEAD")
    ap.add_argument("--date", default="(unstamped)")
    a = ap.parse_args()
    build(a.out_dir, a.commit, a.date)
