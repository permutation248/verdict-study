# Verdict — Artifact

> 个人学习副本：原始实现来自论文 **VERDICT: Multi-Agent Parallel Modular Verification** 的配套 artifact。
> 从[中文源码阅读路线](READING_GUIDE.zh-CN.md)开始；下载来源、完整性检查和本副本的改动见[来源与检查记录](ARTIFACT_PROVENANCE.md)。
> 下方保留作者原始说明；本副本不代表作者官方维护仓库。

Verdict is a multi-agent system for automated formal verification of C
programs. Each function is assigned its own LLM agent that writes and verifies
ACSL contracts with Frama-C/WP; agents verify in parallel under optimistic
assumptions about their callees and reconcile conflicting contracts through a
version-based fixpoint with assume–guarantee composition.

This artifact contains the implementation, benchmarks, experiment scripts,
included results, and scripts that generate the evaluation tables and figures.

## Repository layout

```
src/verdict/              the Verdict implementation (orchestrator, agents, tools)
benchmarks/               15 evaluation programs plus two small examples
scripts/experiments/
  ├── lib/                core runners + the unified judge (score_final.py)
  ├── rq1/ rq2/ rq3/      per-RQ experiment drivers + configs
  ├── baseline-preguss/   Preguss baseline runner (Preguss itself not bundled)
  ├── baseline-specsyn/   SpecSyn baseline runner (SpecSyn itself not bundled)
  └── results/            included experiment results (see "Data" below)
scripts/figures/          experiment results → evaluation tables/figures
frama-c-plugins/global-deps/   bundled Frama-C plugin (call-graph dependencies)
```

## Requirements

- Frama-C (with WP), the SMT solvers **z3 / cvc5 / alt-ergo**, and `dune` on
  `PATH` (e.g. via an opam switch). When both Frama-C and `dune` are available,
  `setup.sh` attempts to build and install the bundled `global-deps` plugin;
  check its warnings because plugin-install failures are non-fatal.
- [`uv`](https://docs.astral.sh/uv/) for the Python environment.
- A LaTeX installation with `pdflatex` for the generated PDFs (see below).
- An Anthropic API key.

## Setup

```bash
cp .env.example .env       # add your ANTHROPIC_API_KEY
./setup.sh                 # system deps → uv sync → clang binding → plugin (if available)
source env_setup.sh        # put frama-c + provers on PATH
uv run verdict doctor      # check the environment
```

## Quick start

```bash
# Verify one benchmark (all functions, parallel fixpoint):
uv run verdict solve -f benchmarks/x509/submodule-1/x509-submodule-1.c \
  --model claude-sonnet-4-6 --max-parallel 8

# Score any tool's annotated .c output under the unified judge (same config for
# Verdict, Preguss, and SpecSyn):
uv run python scripts/experiments/lib/score_final.py <annotated.c> --json out.json
```

Other CLI modes (LLM-backed modes require an explicit model):

```bash
# Interactive step-through solve:
uv run verdict step -f <file> --model claude-sonnet-4-6

# Call graph + RTE counts (no API call):
uv run verdict info -f <file>

# Resume a run; <run-id> names a directory under logs/:
uv run verdict solve -f <same-source.c> --model claude-sonnet-4-6 \
  --resume-from <run-id>
```

## LaTeX for tables and figures

Install the PDF-generation dependencies separately from `setup.sh`:

```bash
# Debian / Ubuntu
sudo apt-get update
sudo apt-get install -y \
  texlive-latex-base texlive-latex-recommended \
  texlive-latex-extra texlive-pictures texlive-fonts-recommended

# macOS (full TeX Live without GUI applications)
brew install --cask mactex-no-gui
eval "$(/usr/libexec/path_helper)"
```

## Regenerating experiment tables and figures

Use the included results to regenerate the RQ1/RQ2/RQ3 tables and figures:

```bash
uv run python scripts/figures/gen_all.py
# → rq1.{tex,csv,pdf}
#   rq2.csv + rq2a.{tex,pdf} + rq2b.{tex,pdf}
#   rq2c.{tex,csv,pdf} + rq3.{tex,csv,pdf}
#   (all under scripts/figures/generated/)
```

To re-run an experiment from scratch (instead of using the included data), use
the per-RQ drivers, e.g. `scripts/experiments/rq1/`. Each cell is run serially
via `scripts/experiments/lib/run_solve.sh` (its header documents the flags).
The baselines (`baseline-preguss/`, `baseline-specsyn/`) require the respective
public artifacts, which are **not** redistributed here; see their READMEs.

## Data

`scripts/experiments/results/` holds the included result artifacts:

```
rq1/verdict-<model>/<bench>/<rep>/run/        run.json, events.jsonl, pipeline.log, final_annotated.c
rq1/verdict-<model>-scores/<bench>/<rep>/     judge_final.json, judge_detail.csv
rq2/<model>[-scores]/{parallel,cycle}/<bench>/<cfg>/rep1/...
rq3/<model>[-scores]/<bench>/<arm>/rep1/...
baseline-preguss/<bench>/<model>/<rep>/        provenance.txt, annotated.c (judge-scored output)
baseline-specsyn/<bench>/<model>/<rep>/        provenance.txt, restored.annotated.c (judge-scored output)
baseline-{preguss,specsyn}-scores/<bench>/<model>-<rep>/ judge_final.json (unified judge)
```

This is a curated subset. Each included tool run retains its final annotated C
so its score can be re-derived with `lib/score_final.py`: Verdict's
`final_annotated.c`, Preguss's `annotated.c`, and SpecSyn's
`restored.annotated.c`. `judge_final.json` carries each tool's unified-judge
score, and `pipeline.log` is reduced to the per-batch convergence lines RQ2c
reads. Not included: full run logs, the baseline tools' intermediate outputs
(LLM transcripts, caches, per-iteration dumps), the third-party baseline
artifacts themselves, and contaminated or superseded cells.

## License

MIT — see `LICENSE`.
