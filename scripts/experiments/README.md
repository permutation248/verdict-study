# Verdict Experiments

Reproducible experiment harness for the paper's three research questions (RQ1
baseline comparison, RQ2 scheduling-parameter sensitivity, RQ3 mechanism
ablation). Designed in three layers so every piece runs independently — debug
one config, run one benchmark, or run a whole grid, then post-process:

- **atom** (`lib/run_solve.sh`) — run ONE fully-specified Verdict solve.
- **composition** (`rq1/`, `rq2/`, `rq3/`) — thin drivers that loop the atom
  over a grid of configs. No run logic is duplicated; they only choose runs.
- **post-processing** (`post/`, `../figures/`) — extract per-run metrics and
  build the paper's tables/figures from the raw outputs. Decoupled from running.

The unified judge (`lib/score_final.py`) scores every tool's final annotated C
under one Frama-C/WP configuration, so Verdict, Preguss, and SpecSyn numbers are
directly comparable.

## Layout

```
scripts/experiments/
  README.md            this file
  lib/                 shared, RQ-agnostic machinery
    benchmarks.sh      benchmark name -> (-f source, -H header, WP model, stem)
    run_solve.sh       ATOM: run ONE fully-specified solve -> a run directory
    score_final.py     unified judge (one WP config for all three tools)
    env.sh             environment setup (PATH, env_setup.sh, .env)
  rq1/                 RQ1 — baseline comparison (Verdict vs Preguss vs SpecSyn)
  rq2/                 RQ2 — parallelism + cycle-budget sweeps
  rq3/                 RQ3 — coordination-mechanism ablation (--ablate)
  baseline-preguss/    Preguss baseline runner (image not redistributed)
  baseline-specsyn/    SpecSyn baseline runner (artifact not redistributed)
  post/                per-run metric extraction + aggregation
  results/             curated raw outputs the figures pipeline reads (shipped)
../figures/            results/ -> every paper table & figure (gen_all.py)
```

## Benchmarks

The 15 benchmarks — 14 comparison benchmarks plus the whole-module x509 parser
(`full-module`, the full 209-function ANSSI parser) — are registered in
`lib/benchmarks.sh` (name -> source / header / WP-model / run-dir stem). They
fall in three source groups under `benchmarks/`: `x509/` (ANSSI parser
submodules + full module), `embedded/` (the contiki AES-CCM module), and
`util-libs/` (utility libraries from the SpecSyn corpus). Per-bench
characteristics are reported in the RQ1 table (its left-hand columns). All 15
are scored by the same unified judge (`lib/score_final.py`); `full-module` is
scored on its own rather than in the batch driver (`post/score_all_verdict.sh`),
since WP on the 24k-line file runs for many hours.

## Running

Each RQ directory has a `config.sh` (the knobs) and driver scripts. Runs are
SERIAL by design: each run's wall-clock IS the measurement, so two runs must
never share the machine.

```bash
# One Verdict solve, for debugging:
bash scripts/experiments/lib/run_solve.sh --bench submodule-1 --out /tmp/cell \
    --max-parallel 8 --wp-procs 8 --model claude-sonnet-4-6

# An RQ grid (see each rq*/config.sh and rq*/README.md):
bash scripts/experiments/rq2/run.sh

# Score any tool's annotated output under the unified judge:
uv run python scripts/experiments/lib/score_final.py <annotated.c> --json out.json

# Rebuild every paper table/figure from the raw data:
uv run python scripts/figures/gen_all.py
```

## Locked experimental decisions

- **Repetitions: 3 per run.** The LLM is non-deterministic and temperature is
  not controllable via the Agent SDK, so every cell is repeated and reported as
  mean ± spread, never a single run.
- **One machine.** A whole sweep runs on the same evaluation host so wall-clock
  numbers are comparable; `--wp-jobs = --max-parallel` keeps WP from queuing so
  the only independent variable is how many agents/WP run at once.
- **Model fixed per cell** (`claude-sonnet-4-6` or the Opus model), recorded in
  each run's `run.json`.
- **Unified judge includes main():** WP runs over the whole file and every
  reported property counts toward the denominators (see `score_final.py`).

## Results data

`results/` is the curated raw data behind every number in the paper:

```
rq1/verdict-<model>[-scores]/<bench>/<rep>/...
rq2/<model>[-scores]/<kind>/<bench>/<cfg>/rep1/...
rq3/<model>[-scores]/<bench>/<arm>/rep1/...
baseline-{preguss,specsyn}[-scores]/<bench>/...
```

Each cell carries `run.json` (time, convergence, call graph), `events.jsonl`
(proof demands, cycles), `judge_final.json` (unified-judge score), and the
tool's `final_annotated.c`. `load_raw.py` (under `../figures/`) is the only
reader; every table and figure is derived from it.
