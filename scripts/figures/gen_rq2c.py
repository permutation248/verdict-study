#!/usr/bin/env python3
"""RQ2c: why parallel speedup saturates (mechanism behind RQ2a), in speedup space.

From pipeline.log's per-batch convergence (via load_raw.rq2_batches) each run's
wall-clock splits into the INITIAL wave (batch 1: every function fires its first
solve at once, optimistically assuming callee contracts) and the REACTIVATION tail
(batches 2+: the assume/demand cascade that settles caller specs once callees
publish). The batch spans sum to the run wall-clock, so T1/total is exactly the
RQ2a speedup. We plot, per representative program (speedup, log-log, like RQ2a):
  - ideal  S = mp;
  - actual S = T1/total  (= the RQ2a curve, which saturates);
  - no-tail potential S = T1/T_init  (what speedup would be without the tail).
The gap between actual and potential is the reactivation tail's drag, shaded.

A single example per chain depth (not an average): RQ2a already gives the
population speedup; this figure illustrates the mechanism on one clean program.
The tail itself parallelizes only partly (so there is no flat Amdahl ceiling) ---
the honest signal is the actual/potential gap. Numbers come only from load_raw.
Usage: gen_rq2c.py <out_dir> [--commit C --date D]
"""
from __future__ import annotations
import argparse
import csv
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import load_raw as L           # noqa: E402
import paper_style as S        # noqa: E402

MODEL = "opus"     # opus reads cleanest; sonnet wall-clock is API-noisy (cf. RQ2a)
# one representative program per chain depth (mechanism illustration, not a mean)
PANELS = [("submodule-2", "deep-chain (x509-parser-2)"),
          ("hypatia", "shallow-chain (hypatia)")]

AXIS = (r"tick align=outside, axis line style={black!70}, "
        r"every tick/.style={black!55}, grid=major, "
        r"major grid style={black!10, line width=0.3pt}, "
        r"label style={font=\small}, tick label style={font=\footnotesize}, "
        r"title style={font=\small}, legend cell align=left, "
        r"every axis plot/.style={line width=1.1pt, mark size=1.9pt}, "
        r"xmode=log, log basis x=2, ymode=log, log basis y=2, "
        r"xtick={1,2,4,8,16,32}, xticklabels={1,2,4,8,16,32}, "
        r"ytick={1,2,4,8,16,32}, yticklabels={1,2,4,8,16,32}, "
        r"xmin=0.85, xmax=38, ymin=0.85, ymax=40")


def series(bench):
    """{mp: (actual_speedup T1/total, notail_speedup T1/init)} from batch spans."""
    prof = {}
    for mp in L.MP_SWEEP:
        w = L.rq2_batches(MODEL, bench, mp)
        prof[mp] = (sum(t for _, t in w), w[0][1]) if w else None
    if not prof.get(1) or not prof[1][0]:
        return {}
    t1 = prof[1][0]
    out = {}
    for mp in L.MP_SWEEP:
        pr = prof.get(mp)
        if pr and pr[0] and pr[1]:
            out[mp] = (t1 / pr[0], t1 / pr[1])
    return out


def _coords(d, i):
    return " ".join(f"({mp},{d[mp][i]:.3f})" for mp in sorted(d))


def _panel(P, bench, title, legend):
    d = series(bench)
    P(f"\\nextgroupplot[title={{\\strut {title}}}, xlabel={{$mp$}}"
      + (r", legend style={at={(0.03,0.967)}, anchor=north west, font=\tiny,"
         r" draw=black!25, fill=white, fill opacity=0.9, text opacity=1,"
         r" row sep=0pt, inner sep=1pt, nodes={inner sep=1pt},"
         r" legend image post style={xscale=0.75}}"  # shorten samples (x only)
         if legend else "") + "]")
    # ideal S = mp
    P(f"\\addplot[black!45, dashed, mark=none, line width=0.8pt] coordinates "
      f"{{{' '.join(f'({mp},{mp})' for mp in L.MP_SWEEP)}}};")
    if legend:
        P(r"\addlegendentry{ideal}")
    # initial-wave potential and actual (named for the fill between)
    P(f"\\addplot[ForestGreen, densely dashed, mark=square*, name path=pot] "
      f"coordinates {{{_coords(d, 1)}}};")
    if legend:
        P(r"\addlegendentry{initial dispatch}")
    P(f"\\addplot[NavyBlue, mark=*, name path=act] coordinates {{{_coords(d, 0)}}};")
    if legend:
        P(r"\addlegendentry{eventual}")
    # shade the gap = reactivation-tail drag
    P(r"\addplot[Maroon, opacity=0.14] fill between[of=pot and act];")
    if legend:
        P(r"\addlegendentry{reconciliation}")


def figure() -> str:
    P, out = (lambda s: out.append(s)), []
    P(r"\begin{tikzpicture}")
    P(r"\begin{groupplot}[group style={group size=2 by 1, horizontal sep=20pt, "
      r"y descriptions at=edge left}, "
      + AXIS + r", width=0.5\linewidth, height=0.42\linewidth, "
      r"ylabel={$T_1/T_{mp}$}]")
    for k, (bench, title) in enumerate(PANELS):
        _panel(P, bench, title, legend=True)   # same legend on both panels
    P(r"\end{groupplot}")
    P(r"\end{tikzpicture}")
    return "\n".join(out)


def _write_csv(path):
    with open(path, "w", newline="") as fp:
        w = csv.writer(fp)
        w.writerow(["bench", "mp", "actual_speedup", "notail_speedup"])
        for bench, _ in PANELS:
            d = series(bench)
            for mp in sorted(d):
                w.writerow([bench, mp, round(d[mp][0], 3), round(d[mp][1], 3)])


CAP = (r"RQ2c: why parallel speedup saturates (\textsc{Opus}), in speedup space "
       r"(cf.\ RQ2a). Each run's wall-clock splits into the initial dispatch (every "
       r"function's first solve, all at once) and the \emph{reactivation tail} "
       r"(the assume/demand cascade after "
       r"callees publish); the spans sum to the wall-clock, so the \emph{actual} "
       r"curve $T_1/T_{\mathrm{total}}$ is the RQ2a speedup. The "
       r"\emph{initial-wave} curve $T_1/T_{\mathrm{init}}$ counts only that first "
       r"batch --- the speedup if there were no reactivation tail; the "
       r"shaded gap is the tail's drag. On the \textbf{deep} chain "
       r"(\texttt{x509-parser-2}) the gap widens with $mp$ --- the sequential "
       r"reactivation cascade, whose tiny waves cannot use a large "
       r"\texttt{max\_parallel}, caps the speedup. \textbf{hypatia} ($54$ "
       r"independent functions) has a thin tail and tracks the ideal. We show one "
       r"program per depth (RQ2a already gives the population); the tail "
       r"parallelizes only partly, so there is no flat ceiling.")


def build(out_dir, commit, date):
    os.makedirs(out_dir, exist_ok=True)
    body = ("\\begin{figure}[t]\n\\centering\n" + figure() +
            f"\n\\caption{{{CAP}}}\n\\label{{fig:rq2c}}\n\\end{{figure}}\n")
    with open(os.path.join(out_dir, "rq2c.tex"), "w") as f:
        f.write(S.provenance(commit, str(L.RESULTS), date) + body)
    _write_csv(os.path.join(out_dir, "rq2c.csv"))
    S.render(figure(), "rq2c", out_dir)
    print(f"wrote rq2c.tex / rq2c.csv (+pdf) to {out_dir}")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("out_dir")
    ap.add_argument("--commit", default="HEAD")
    ap.add_argument("--date", default="(unstamped)")
    a = ap.parse_args()
    build(a.out_dir, a.commit, a.date)
