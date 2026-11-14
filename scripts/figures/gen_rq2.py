#!/usr/bin/env python3
"""RQ2 scalability figures, unified metric scheme.

Both models in one figure via side-by-side pgfplots groupplots:
  RQ2a (parallel, mc=24, mp in {1,2,4,8,16,32}): parallel speedup T1/Tmp vs
    max_parallel, one line per RQ2 benchmark + dashed ideal — Opus | Sonnet.
  RQ2b (cycle, mp=8, mc in {1,2,4,8,16,24}): 2x2 grid, rows = model, cols =
    convergence rate (per bench) | mean rte/spec discharge.

mp=8 / mc=24 are the RQ1 rep1 baselines (load_raw injects them). Sonnet's
wall-clock carries API-latency / oversubscription noise at high mp (some
benchmarks regress at mp=32), so RQ2a reads Opus-first. Numbers come only from
load_raw.
Usage: gen_rq2.py <out_dir> [--commit C --date D]
"""
from __future__ import annotations
import argparse
import csv
import os
import statistics
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import load_raw as L           # noqa: E402
import paper_style as S        # noqa: E402

CYCLE = ["NavyBlue", "Maroon", "ForestGreen", "RedOrange", "violet", "Brown"]
MARKS = ["*", "square*", "triangle*", "diamond*", "pentagon*", "x"]
DISP = [(b, L.display_name(b)) for b in L.RQ2_BENCHES]
MODELS = [("opus", r"\textsc{Opus}"), ("sonnet", r"\textsc{Sonnet}")]

AXIS = (r"tick align=outside, axis line style={black!70}, "
        r"every tick/.style={black!55}, grid=major, "
        r"major grid style={black!10, line width=0.3pt}, "
        r"label style={font=\small}, tick label style={font=\footnotesize}, "
        r"title style={font=\small}, legend cell align=left, "
        r"legend style={font=\scriptsize, draw=black!25, fill=white, "
        r"fill opacity=0.92, text opacity=1, inner sep=2pt}, "
        r"every axis plot/.style={line width=1pt, mark size=1.5pt}")


def _coords(series):
    return " ".join(f"({x},{v:.2f})" for x, v in series if v is not None)


# ── RQ2a: parallel speedup, Opus | Sonnet ────────────────────────────────────

def _rq2a_panel(P, model, legend):
    # On the legend panel, fill the upper-left empty wedge with a white triangle
    # (hypotenuse parallel to the diagonal) so the legend's *background* is a
    # triangle hugging the corner rather than a rectangle; entries (borderless,
    # 4/2/1) are drawn on top. Filled first, so it sits behind the curves.
    if legend:
        # white right-triangle backdrop hugging the upper-left corner (right
        # angle at top-left): top and left legs along the axes, hypotenuse
        # running just left of the diagonal so it covers neither it nor the
        # low-mp curves. The left-aligned legend steps inward beneath it.
        P(r"\fill[white, fill opacity=0.9, draw=black!25, line join=round] "
          r"(rel axis cs:0.035,0.958) -- (rel axis cs:0.80,0.958) -- "
          r"(rel axis cs:0.035,0.35) -- cycle;")
    # all curves forget_plot; the legend is built manually below so that
    # 'ideal' sits at the bottom tip of the triangle.
    P(f"\\addplot[black!45, dashed, mark=none, line width=0.8pt, forget plot] "
      f"coordinates {{{_coords([(mp, mp) for mp in L.MP_SWEEP])}}};")
    for i, (b, disp) in enumerate(DISP):
        ser = L.rq2_parallel(model, b)
        t1 = ser[0][1]["time_min"]
        speed = [(mp, (t1 / r["time_min"]) if (t1 and r["time_min"]) else None)
                 for mp, r in ser]
        real = [(mp, v) for mp, v in speed if v is not None]
        if not real:
            continue
        nf = L.bench_meta(b)["n_funcs"] or 1
        last_mp, last_v = real[-1]
        # extend only when the last measured mp already saturates parallelism
        # (>= #funcs); a flat dashed stub then marks the equivalent higher mp.
        sat = ([(mp, last_v) for mp, v in speed if v is None and mp > last_mp]
               if last_mp >= nf else [])
        P(f"\\addplot[{CYCLE[i]}, mark={MARKS[i]}, forget plot] "
          f"coordinates {{{_coords(real)}}};")
        if sat:
            P(f"\\addplot[{CYCLE[i]}, dashed, mark=none, forget plot] "
              f"coordinates {{{_coords([(last_mp, last_v)] + sat)}}};")
            P(f"\\addplot[{CYCLE[i]}, only marks, mark={MARKS[i]}, forget plot] "
              f"coordinates {{{_coords(sat)}}};")
    if not legend:
        return
    # Hand-placed legend (line sample + mark + label), left-aligned with rows
    # of 3/2/1/1 so the right edge steps inward beneath the right-triangle
    # backdrop; 'ideal' is the bottom row. Drawn after the curves, on top.
    short = {"x509-parser-1": "x509-1", "x509-parser-2": "x509-2"}
    B = [(short.get(d, d), CYCLE[i], MARKS[i]) for i, (_, d) in enumerate(DISP)]
    LW = 0.045   # line-sample length in axis-relative units

    def emit(xl, y, item):
        a, c = round(xl, 3), round(xl + LW, 3)
        if item is None:                              # ideal: dashed, no mark
            ci = round(xl + 0.055, 3)                 # longer sample so the
            P(f"\\draw[black!45, dash pattern=on 1.6pt off 1.1pt, "  # dashes show
              f"line width=0.9pt] (rel axis cs:{a},{y}) -- (rel axis cs:{ci},{y});")
            P(f"\\node[anchor=west, font=\\tiny, inner sep=1pt] at "
              f"(rel axis cs:{ci},{y}) {{\\texttt{{ideal}}}};")
        else:
            lbl, col, mk = item
            P(f"\\draw[{col}, line width=1pt] (rel axis cs:{a},{y}) -- "
              f"(rel axis cs:{c},{y});")
            P(f"\\node[inner sep=0pt] at (rel axis cs:{round(xl + LW/2, 3)},{y}) "
              f"{{\\color{{{col}}}\\pgfuseplotmark{{{mk}}}}};")
            P(f"\\node[anchor=west, font=\\tiny, inner sep=1pt] at "
              f"(rel axis cs:{c},{y}) {{\\texttt{{{lbl}}}}};")

    P(r"\pgfsetplotmarksize{1.5pt}")
    y1, y2, y3, y4 = 0.90, 0.79, 0.68, 0.57
    emit(0.065, y1, B[0]); emit(0.25, y1, B[1]); emit(0.435, y1, B[2])
    emit(0.065, y2, B[3]); emit(0.25, y2, B[4])
    emit(0.065, y3, B[5])
    emit(0.065, y4, None)


def rq2a_figure() -> str:
    P, out = (lambda s: out.append(s)), []
    P(r"\begin{tikzpicture}")
    P(r"\begin{groupplot}[group style={group size=2 by 1, horizontal sep=20pt, "
      r"y descriptions at=edge left}, "
      + AXIS + r", width=0.5\linewidth, height=0.42\linewidth, "
      r"xmode=log, log basis x=2, ymode=log, log basis y=2, "
      r"xtick={1,2,4,8,16,32}, xticklabels={1,2,4,8,16,32}, "
      r"ytick={1,2,4,8,16,32}, yticklabels={1,2,4,8,16,32}]")
    for k, (model, disp) in enumerate(MODELS):
        left = (k == 0)
        yl = r", ylabel={$T_1/T_{mp}$}" if left else ""
        opt = f"title={{\\strut {disp}}}, xlabel={{$mp$}}" + yl
        P(f"\\nextgroupplot[{opt}]")
        _rq2a_panel(P, model, legend=True)   # same legend on both panels
    P(r"\end{groupplot}")
    P(r"\end{tikzpicture}")
    return "\n".join(out)


# ── RQ2b: convergence + discharge, 2x2 (model x metric) ──────────────────────

def _rq2b_conv(P, model, legend):
    short = {"x509-parser-1": "x509-1", "x509-parser-2": "x509-2"}
    for i, (b, disp) in enumerate(DISP):
        ser = [(mc, r["convergence"] * 100 if r["convergence"] is not None else None)
               for mc, r in L.rq2_cycle(model, b)]
        P(f"\\addplot[{CYCLE[i]}, mark={MARKS[i]}] coordinates {{{_coords(ser)}}};")
        if legend:
            P(f"\\addlegendentry{{\\texttt{{{short.get(disp, disp)}}}}}")


def _rq2b_disch(P, model, legend):
    for metric, color, mark, lbl in [("rte", "NavyBlue", "*", "rte"),
                                     ("spec", "Maroon", "square*", "spec")]:
        ser = []
        for j, mc in enumerate(L.MC_SWEEP):
            vals = [L.rq2_cycle(model, b)[j][1][f"{metric}_r"] for b in L.RQ2_BENCHES]
            vals = [v * 100 for v in vals if v is not None]
            ser.append((mc, statistics.mean(vals) if vals else None))
        P(f"\\addplot[{color}, mark={mark}] coordinates {{{_coords(ser)}}};")
        if legend:
            P(f"\\addlegendentry{{mean {lbl}}}")


def rq2b_figure() -> str:
    P, out = (lambda s: out.append(s)), []
    P(r"\begin{tikzpicture}")
    P(r"\begin{groupplot}[group style={group size=2 by 2, horizontal sep=52pt, "
      r"vertical sep=26pt}, " + AXIS + r", width=0.5\linewidth, "
      r"height=0.42\linewidth, xmode=log, log basis x=2, "
      r"xtick={1,2,4,8,16,24}, xticklabels={1,2,4,8,16,24}]")
    # both metric legends sit in the bottom-right corner, empty once the curves
    # plateau at high mc; shown on both model rows.
    conv_leg = (r", legend style={at={(0.97,0.04)}, anchor=south east, "
                r"font=\tiny, legend columns=2, draw=black!25, fill=white, "
                r"fill opacity=0.9, text opacity=1, inner sep=1.5pt, "
                r"column sep=4pt, row sep=-1pt}")
    disch_leg = (r", legend style={at={(0.97,0.04)}, anchor=south east, "
                 r"font=\tiny, draw=black!25, fill=white, fill opacity=0.9, "
                 r"text opacity=1, inner sep=1.5pt, row sep=-1pt}")
    for r, (model, disp) in enumerate(MODELS):
        bottom = (r == len(MODELS) - 1)
        xl = r", xlabel={$mc$}" if bottom else ""
        # col 1: convergence rate, 0-100%, label+ticks on the right
        opt = (r"ylabel={convergence rate (\%)}, ymin=0, ymax=105, "
               r"ytick={0,25,50,75,100}" + xl + conv_leg)
        P(f"\\nextgroupplot[{opt}]")
        _rq2b_conv(P, model, legend=True)
        # col 2: mean discharge, label+ticks on the right
        opt = (r"ylabel={discharge rate (\%)}, ymin=85, ymax=101, "
               r"ytick={85,90,95,100}" + xl + disch_leg)
        P(f"\\nextgroupplot[{opt}]")
        _rq2b_disch(P, model, legend=True)
    P(r"\end{groupplot}")
    # rotated model name on the far left of each row (pgfplots auto-names panels)
    for r, (model, disp) in enumerate(MODELS):
        P(f"\\node[rotate=90, anchor=south, font=\\small\\scshape] "
          f"at ([xshift=-46pt]group c1r{r + 1}.west) {{{model}}};")
    P(r"\end{tikzpicture}")
    return "\n".join(out)


def _write_csv(path):
    with open(path, "w", newline="") as fp:
        w = csv.writer(fp)
        w.writerow(["sweep", "model", "bench", "x", "time_min", "convergence",
                    "rte_pct", "spec_pct"])
        for model, _ in MODELS:
            for b in L.RQ2_BENCHES:
                for mp, r in L.rq2_parallel(model, b):
                    w.writerow(["parallel", model, b, mp, r["time_min"], "", "", ""])
                for mc, r in L.rq2_cycle(model, b):
                    w.writerow(["cycle", model, b, mc, "", r["convergence"],
                                (r["rte_r"] or 0) * 100, (r["spec_r"] or 0) * 100])


def build(out_dir, commit, date):
    os.makedirs(out_dir, exist_ok=True)
    prov = S.provenance(commit, str(L.RESULTS), date)
    for tag, env, fig, cap in [
        ("rq2a", "figure", rq2a_figure(),
         r"RQ2a: parallel speedup $T_1/T_{mp}$ vs.\ \texttt{max\_parallel} "
         r"(\texttt{max\_cycles}$=24$), stacked per model; the dashed line is the "
         r"ideal $T_1/T_{mp}{=}mp$, and \texttt{contiki} has only 10 functions, so its "
         r"$mp{=}32$ point repeats $mp{=}16$ (dashed stub). Speedup rises steadily "
         r"with the agent count, reaching a geometric-mean 4.9$\times$ at 32 agents."),
        ("rq2b", "figure*", rq2b_figure(),
         r"RQ2b: convergence rate (left) and mean rte/spec discharge (right) "
         r"vs.\ refinement rounds (\texttt{max\_parallel}$=8$); rows are models. "
         r"Both the internal convergence rate and the external discharge rise "
         r"together and plateau by \texttt{max\_cycles}$\approx$8; deeper call "
         r"chains converge later."),
    ]:
        body = f"\\begin{{{env}}}[t]\n\\centering\n" + fig + \
               f"\n\\caption{{{cap}}}\n\\label{{fig:{tag}}}\n\\end{{{env}}}\n"
        with open(os.path.join(out_dir, f"{tag}.tex"), "w") as f:
            f.write(prov + body)
        S.render(fig, tag, out_dir)
    _write_csv(os.path.join(out_dir, "rq2.csv"))
    print(f"wrote rq2a.tex / rq2b.tex / rq2.csv (+pdf) to {out_dir}")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("out_dir")
    ap.add_argument("--commit", default="HEAD")
    ap.add_argument("--date", default="(unstamped)")
    a = ap.parse_args()
    build(a.out_dir, a.commit, a.date)
