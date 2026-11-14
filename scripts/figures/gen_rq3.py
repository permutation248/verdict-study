#!/usr/bin/env python3
"""RQ3 ablation figure, unified metric scheme.

A quality-coordination *trade-off* view: each ablation arm is a point placed by
coordination effort (proof demands raised, x) versus spec discharge (quality, y),
with arrows from the full system showing the DIRECTION each removal degrades.
This makes the three ablations' costs land on orthogonal axes instead of forcing
them into one "up/down" line — and turns "0 demands" from a misleading drop into
a position ("this config does zero coordination; see what quality costs").

  - Remove assumed contracts (-asm): arrow RIGHT — demand traffic ~doubles to
    keep quality, but discharge holds (effort cost).
  - Remove proof demands (-dem): arrow DOWN — agents give up instead of
    coordinating, discharge collapses and false bugs surge (quality cost).
  - Remove both: lands with -dem — demands is the load-bearing mechanism; once
    it is gone, also dropping assume adds little.

Deep chains move sharply; shallow chains (hollow) barely move. A second row
shows wall-clock honestly: Opus gives up early without demands (faster), Sonnet
thrashes (slower) — wall-clock is noisy (cf. RQ2). Numbers come only from
load_raw. Usage: gen_rq3.py <out_dir> [--commit C --date D]
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

DEEP = [b for b in L.RQ2_BENCHES if L.RQ3_CHAIN.get(b) == "deep"]
SHALLOW = [b for b in L.RQ2_BENCHES if L.RQ3_CHAIN.get(b) == "shallow"]
ARMS = ["baseline", "no-assume", "no-demands", "no-demands-no-assume"]
ARM_LAB = {"baseline": "full", "no-assume": "$-$asm",
           "no-demands": "$-$dem", "no-demands-no-assume": "$-$both"}
ABLATIONS = ["no-assume", "no-demands", "no-demands-no-assume"]
MODELS = [("opus", r"\textsc{Opus}"), ("sonnet", r"\textsc{Sonnet}")]

# three ablations get three colors (same on both panels); base = neutral ref.
# fill (filled/hollow) separately encodes deep/shallow.
ARM_COLOR = {"baseline": "black!75", "no-assume": "NavyBlue",
             "no-demands": "RedOrange", "no-demands-no-assume": "Maroon"}
# label anchor per arm so the four points do not collide. -dem and -both both
# sit at x=0 (zero demands) only ~2 apart in y, and their order flips by model;
# a fixed up/down anchor clips the lower label under ymin (e.g. Opus -dem at
# 80.8 with ymin 80), so both are labelled to the right ("west") at their own
# height instead.
ARM_ANCHOR = {"baseline": "south", "no-assume": "south",
              "no-demands": "west", "no-demands-no-assume": "west"}

AXIS = (r"tick align=outside, axis line style={black!70}, "
        r"every tick/.style={black!55}, grid=major, "
        r"major grid style={black!10, line width=0.3pt}, "
        r"label style={font=\small}, tick label style={font=\footnotesize}, "
        r"title style={font=\small, yshift=-2pt}, "
        r"every axis plot/.style={line width=1.1pt}")


def _mean(model, benches, arm, key):
    vals = [r[key] for b in benches
            for r in [L.rq3_arms(model, b).get(arm, {})]
            if r.get("present") and r.get(key) is not None]
    return statistics.mean(vals) if vals else None


def _cell(model, benches, arm):
    return {k: _mean(model, benches, arm, k)
            for k in ("rte_r", "spec_r", "bugs", "time_min", "demands_raised")}


def _coords(series):
    return " ".join(f"({x},{v:.2f})" for x, v in series if v is not None)


def _xy(cell):
    """(coordination effort, quality) = (demands raised, spec discharge %)."""
    return (cell["demands_raised"] or 0.0, (cell["spec_r"] or 0.0) * 100)


# ── col 1: quality-coordination trade-off scatter ───────────────────────────

def _scatter(P, model):
    deep = {a: _cell(model, DEEP, a) for a in ARMS}
    shal = {a: _cell(model, SHALLOW, a) for a in ARMS}
    # shallow control: hollow markers, colored per arm (hollow = shallow)
    for a in ARMS:
        sx, sy = _xy(shal[a])
        P(f"\\addplot[only marks, mark=o, {ARM_COLOR[a]}, mark size=2pt, "
          f"forget plot] coordinates {{({sx:.1f},{sy:.1f})}};")
    # base is the reference; arrows fan out to each ablation (direction = cost)
    bx, by = _xy(deep["baseline"])
    for a in ABLATIONS:
        x, y = _xy(deep[a])
        P(f"\\addplot[{ARM_COLOR[a]}!75, -latex, line width=0.7pt, forget plot] "
          f"coordinates {{({bx:.1f},{by:.1f}) ({x:.1f},{y:.1f})}};")
    # deep arm markers (filled) + labels, colored per arm
    for a in ARMS:
        x, y = _xy(deep[a])
        P(f"\\addplot[only marks, mark=*, {ARM_COLOR[a]}, mark size=2.6pt, "
          f"forget plot] coordinates {{({x:.1f},{y:.1f})}} "
          f"node[anchor={ARM_ANCHOR[a]}, font=\\scriptsize, {ARM_COLOR[a]}, "
          f"inner sep=3pt]{{{ARM_LAB[a]}}};")


def _scatter_opt(bottom):
    o = (r"xlabel={Event Demands}, " if bottom else "")
    o += (r"ylabel={spec discharge (\%)}, xmin=-4, xmax=56, ymin=80, ymax=105, "
          r"xtick={0,10,20,30,40,50}, ytick={80,85,90,95,100}")
    return o


# ── col 2: wall-clock per arm (honest; model-dependent) ──────────────────────

def _time(P, model):
    """Wall-clock per arm: neutral connecting lines (solid = deep, dashed =
    shallow) with markers colored per arm (filled = deep, hollow = shallow), so
    color and fill mean the same thing as in the scatter."""
    xs = list(range(1, len(ARMS) + 1))
    deep = [(i, _mean(model, DEEP, a, "time_min")) for i, a in zip(xs, ARMS)]
    shal = [(i, _mean(model, SHALLOW, a, "time_min")) for i, a in zip(xs, ARMS)]
    P(f"\\addplot[black!55, mark=none, forget plot] coordinates {{{_coords(deep)}}};")
    P(f"\\addplot[black!35, dashed, mark=none, forget plot] "
      f"coordinates {{{_coords(shal)}}};")
    for i, a in zip(xs, ARMS):
        td, ts = _mean(model, DEEP, a, "time_min"), _mean(model, SHALLOW, a, "time_min")
        P(f"\\addplot[only marks, mark=*, {ARM_COLOR[a]}, mark size=2.2pt, "
          f"forget plot] coordinates {{({i},{td:.2f})}};")
        P(f"\\addplot[only marks, mark=o, {ARM_COLOR[a]}, mark size=2pt, "
          f"forget plot] coordinates {{({i},{ts:.2f})}};")


def _time_opt(bottom):
    o = (r"ylabel={wall-clock (min)}, ymin=0, ymax=85, ytick={0,20,40,60,80}, "
         r"xmin=0.6, xmax=4.4, "
         r"xtick={1,2,3,4}, xticklabels={" + ",".join(ARM_LAB[a] for a in ARMS)
         + r"}, x tick label style={font=\scriptsize}")
    if bottom:
        o = r"xlabel={ablation}, " + o
    return o


def _depth_rule():
    """Script-derived deep/shallow membership + the max-call-depth threshold that
    separates them (numbers from load_raw, never hand-typed)."""
    deep = [(L.display_name(b), L.bench_meta(b)["max_depth"] or 0) for b in DEEP]
    shal = [(L.display_name(b), L.bench_meta(b)["max_depth"] or 0) for b in SHALLOW]
    dmin = min(d for _, d in deep)
    smax = max(d for _, d in shal)
    names = lambda xs: ", ".join(f"\\texttt{{{n}}}" for n, _ in xs)
    return dmin, smax, names(deep), names(shal)


def _strip(P):
    """One boxed legend shared by both columns: filled/hollow markers (matching
    the plots) encode call-chain depth, with membership and the depth rule.
    Numbers are script-derived; nothing is hand-typed."""
    dmin, smax, dn, sn = _depth_rule()
    # neutral fill markers: they encode filled/hollow (depth), not arm color
    deep_m = r"\tikz[baseline=-0.55ex]{\fill[black!65] (0,0) circle (2.4pt);}"
    shal_m = (r"\tikz[baseline=-0.55ex]{\draw[black!55, line width=1pt] "
              r"(0,0) circle (2.1pt);}")
    P(r"\node[draw=black!35, line width=0.5pt, rounded corners=2pt, fill=white, "
      r"inner sep=5pt, anchor=north, align=left, font=\footnotesize] "
      r"at ([yshift=-34pt]$(group c1r2.south)!0.5!(group c2r2.south)$) {%")
    P(deep_m + r"~\textbf{deep-chain} (max call depth $\geq" + str(dmin)
      + r"$): " + dn + r"\\[3pt]")
    P(shal_m + r"~\textbf{shallow-chain} (max call depth $\leq" + str(smax)
      + r"$): " + sn + r"};")


def figure() -> str:
    P, out = (lambda s: out.append(s)), []
    P(r"\begin{tikzpicture}")
    P(r"\begin{groupplot}[group style={group size=2 by 2, horizontal sep=46pt, "
      r"vertical sep=26pt}, " + AXIS + r", width=0.5\linewidth, "
      r"height=0.42\linewidth]")
    for k, (model, disp) in enumerate(MODELS):       # one model = one row
        bottom = (k == len(MODELS) - 1)
        P(f"\\nextgroupplot[{_scatter_opt(bottom)}]")          # col 1: scatter
        _scatter(P, model)
        P(f"\\nextgroupplot[{_time_opt(bottom)}]")             # col 2: time
        _time(P, model)
    P(r"\end{groupplot}")
    _strip(P)                                        # bottom classification legend
    # rotated model name on the far left of each row (pgfplots auto-names panels)
    for k, (model, _) in enumerate(MODELS):
        P(f"\\node[rotate=90, anchor=south, font=\\small\\scshape] "
          f"at ([xshift=-46pt]group c1r{k + 1}.west) {{{model}}};")
    P(r"\end{tikzpicture}")
    return "\n".join(out)


def _write_csv(path):
    with open(path, "w", newline="") as fp:
        w = csv.writer(fp)
        w.writerow(["model", "group", "arm", "rte_pct", "spec_pct", "bugs",
                    "time_min", "demands_raised", "demands_satisfied"])
        for model, _ in MODELS:
            for gname, benches in [("deep", DEEP), ("shallow", SHALLOW)]:
                for a in ARMS:
                    row = [model, gname, a]
                    for key, sc in [("rte_r", 100), ("spec_r", 100), ("bugs", 1),
                                    ("time_min", 1), ("demands_raised", 1),
                                    ("demands_satisfied", 1)]:
                        v = _mean(model, benches, a, key)
                        row.append(round(v * sc, 2) if v is not None else "")
                    w.writerow(row)


CAP = (r"RQ3 ablation on deep call chains; rows are models; filled $=$ "
       r"deep-chain, hollow $=$ shallow-chain (bottom legend). \textbf{Left:} "
       r"quality--coordination trade-off --- base is the reference and arrows "
       r"point to each ablation, placed by coordination effort (proof demands "
       r"raised, $x$) vs.\ spec discharge ($y$). Removing \emph{assumed "
       r"contracts} ($-$asm) pushes \emph{right} ($\sim$2$\times$ demand traffic) "
       r"yet holds quality; removing \emph{proof demands} ($-$dem) pushes "
       r"\emph{down} (discharge collapses, $+35$ false bugs) as agents give up "
       r"instead of coordinating; $-$both lands with $-$dem --- demands is the "
       r"load-bearing mechanism, so also dropping assume adds little. Shallow "
       r"chains barely move. \textbf{Right:} wall-clock per arm --- without "
       r"demands \textsc{Opus} gives up early (faster) while \textsc{Sonnet} "
       r"thrashes (slower); wall-clock is noisy (cf.\ RQ2). rte discharge tracks "
       r"spec ($100\!\to\!94\%$ at $-$dem).")


def build(out_dir, commit, date):
    os.makedirs(out_dir, exist_ok=True)
    body = ("\\begin{figure*}[t]\n\\centering\n" + figure() +
            f"\n\\caption{{{CAP}}}\n\\label{{fig:rq3}}\n\\end{{figure*}}\n")
    with open(os.path.join(out_dir, "rq3.tex"), "w") as f:
        f.write(S.provenance(commit, str(L.RESULTS), date) + body)
    _write_csv(os.path.join(out_dir, "rq3.csv"))
    S.render(figure(), "rq3", out_dir)
    print(f"wrote rq3.tex / rq3.csv (+pdf) to {out_dir}")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("out_dir")
    ap.add_argument("--commit", default="HEAD")
    ap.add_argument("--date", default="(unstamped)")
    a = ap.parse_args()
    build(a.out_dir, a.commit, a.date)
