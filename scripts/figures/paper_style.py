#!/usr/bin/env python3
"""Shared LaTeX/pgfplots style + formatting helpers for every paper table/figure.

Design language is lifted from the paper's own assets so generated artifacts
match the figures (runtime-orchestrator.pdf) and header.tex:
  - semantic colors: ForestGreen = good/verified, Maroon = worse/bug,
    NavyBlue = VERDICT hero, Gray = baselines/secondary.
  - reuse the paper's macros \\best (bold = best) and \\similar (underline = 2nd).
  - tables: booktabs three-line, no vertical rules, restrained color.
  - figures: pgfplots, VERDICT in NavyBlue, baselines gray, ideal dashed.

Every gen_*.py imports the helpers here; numbers come only from load_raw.
"""
from __future__ import annotations
import os
import statistics
import subprocess

# ── colors (match the paper; dvipsnames names the paper already uses) ────────
C_GOOD = "ForestGreen"
C_BAD = "Maroon"
C_HERO = "NavyBlue"     # VERDICT
C_BASE = "black!55"     # baselines / secondary text
C_STD = "black!50"      # mean±std spread

# Tool display names + figure colors (pgfplots).
TOOLS = ["verdict", "preguss", "specsyn"]
TOOL_LABEL = {"verdict": "\\textsc{Verdict}", "preguss": "\\preguss",
              "specsyn": "SpecSyn"}
TOOL_COLOR = {"verdict": C_HERO, "preguss": "Maroon", "specsyn": "RedOrange"}

# ── standalone preview wrapper (local pdflatex; mirrors the paper preamble) ───
_PREAMBLE = r"""\documentclass[border=1pt]{standalone}
\usepackage[dvipsnames]{xcolor}
\usepackage{booktabs,multirow,makecell,amssymb,array,colortbl}
\usepackage{graphicx,adjustbox}
\usepackage{rotating}
\usepackage{tikz}\usetikzlibrary{arrows,calc}
\usepackage{pgfplots}\pgfplotsset{compat=1.17}\usepgfplotslibrary{groupplots,fillbetween}
% paper macros reused verbatim so previews match the real document
\newcommand{\best}[1]{{\textbf{#1}}}
\newcommand{\similar}[1]{{\underline{#1}}}
\newcommand{\green}[1]{\textcolor{ForestGreen}{#1}}
\newcommand{\red}[1]{\textcolor{Maroon}{#1}}
\newcommand{\nblue}[1]{\textcolor{NavyBlue}{#1}}
\newcommand{\preguss}{\textsc{Preguss}}
\newcommand{\upr}[1]{#1\,{\color{Maroon}$\blacktriangle$}}
\newcommand{\dng}[1]{#1\,{\color{ForestGreen}$\blacktriangledown$}}
"""


def standalone_doc(body: str) -> str:
    """Wrap a table/figure body into a crop-to-content standalone document."""
    return _PREAMBLE + "\\begin{document}\n" + body + "\n\\end{document}\n"


_BUILD_EXTS = (".tex", ".aux", ".log", ".out", ".fls", ".fdb_latexmk",
               ".synctex.gz", ".pdf")


def render(body: str, name: str, out_dir: str) -> bool:
    """Render a table/figure body to <out_dir>/<name>.pdf, then remove every
    LaTeX build artifact so only the .pdf remains. Returns True on success."""
    stem = f"_build_{name}"
    base = os.path.join(out_dir, stem)
    with open(base + ".tex", "w") as fp:
        fp.write(standalone_doc(body))
    subprocess.run(["pdflatex", "-interaction=nonstopmode", "-halt-on-error",
                    stem + ".tex"], cwd=out_dir, capture_output=True)
    ok = os.path.exists(base + ".pdf")
    if ok:
        os.replace(base + ".pdf", os.path.join(out_dir, f"{name}.pdf"))
    for ext in _BUILD_EXTS:
        try:
            os.remove(base + ext)
        except OSError:
            pass
    return ok


def provenance(commit: str, results: str, date: str) -> str:
    """Header comment stamped into every generated .tex (spec §0.4)."""
    return (f"% AUTO-GENERATED — do not edit by hand.\n"
            f"% source commit: {commit}\n% results dir: {results}\n"
            f"% generated: {date}\n")

# ── pgfplots shared style (figures read the same numbers as their table) ─────
PGF_AXIS_STYLE = r"""\pgfplotsset{
  verdictaxis/.style={
    width=\linewidth, height=0.62\linewidth,
    tick align=outside, tick pos=left,
    axis line style={black!70}, every tick/.style={black!55},
    grid=major, major grid style={black!12, line width=0.3pt},
    label style={font=\small}, tick label style={font=\footnotesize},
    legend style={font=\footnotesize, draw=black!30, fill=white,
                  fill opacity=0.9, text opacity=1, row sep=1pt,
                  inner sep=2pt},
    legend cell align=left,
    every axis plot/.style={line width=1.1pt, mark size=1.8pt},
  },
}"""

# ── number / cell formatting ─────────────────────────────────────────────────

def pct(ratio, digits=1):
    """A 0–1 ratio as a percentage string (no % sign — header carries it)."""
    if ratio is None:
        return "--"
    return f"{ratio * 100:.{digits}f}"


def vt(valid, total):
    if valid is None or total is None:
        return "--"
    return f"{valid}/{total}"


def num(x, digits=0):
    if x is None:
        return "--"
    return f"{x:.{digits}f}" if digits else f"{round(x)}"


def meanstd(vals, digits=1, bold=False):
    """`mean$\\pm$std` — std in gray at the SAME font size as the mean; only the
    mean is bolded (bold=True) so emphasis never changes the apparent size."""
    xs = [v for v in vals if v is not None]
    if not xs:
        return "--"
    m = statistics.mean(xs)
    s = statistics.pstdev(xs) if len(xs) > 1 else 0.0
    mean_str = f"\\best{{{m:.{digits}f}}}" if bold else f"{m:.{digits}f}"
    return f"{mean_str}\\,\\textcolor{{{C_STD}}}{{$\\pm${s:.{digits}f}}}"


def best(s):      return f"\\best{{{s}}}"
def similar(s):   return f"\\similar{{{s}}}"
def good(s):      return f"\\textcolor{{{C_GOOD}}}{{{s}}}"
def bad(s):       return f"\\textcolor{{{C_BAD}}}{{{s}}}"
def hero(s):      return f"\\textcolor{{{C_HERO}}}{{{s}}}"
def gray(s):      return f"\\textcolor{{{C_STD}}}{{{s}}}"


def arrow_vs(value_str, delta, *, worse_up=True, eps=1e-9):
    """Annotate a value with ▲/▼ vs a baseline. By convention (RQ3) an INCREASE
    is worse for bugs/time → red ▲; a DECREASE is better → green ▼."""
    if delta is None or abs(delta) <= eps:
        return value_str
    up = delta > 0
    worse = up if worse_up else (not up)
    mark = "\\blacktriangle" if up else "\\blacktriangledown"
    color = C_BAD if worse else C_GOOD
    return f"{value_str}\\,{{\\color{{{color}}}$\\{mark.lstrip(chr(92))}$}}"


def rotgroup(label, span):
    """A rotated multirow group label spanning `span` rows (col 1)."""
    return f"\\multirow{{{span}}}{{*}}{{\\rotatebox[origin=c]{{90}}{{{label}}}}}"
