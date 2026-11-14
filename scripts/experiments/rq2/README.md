# RQ2 — Hyperparameter sensitivity

**RQ2: How do Verdict's convergence and wall-clock scale with (a) agent
parallelism and (b) per-agent cycle budget?**

Two independent sweeps on the same bench set:

## RQ2a — Parallelism scaling

Independent variable: `--max-parallel` ∈ {1, 2, 4, 8, 16}, with
`--wp-jobs` co-scaled. `--max-cycles` fixed at 25 (default).
Primary metric: wall-clock time.

## RQ2b — Cycle budget scaling

Independent variable: `--max-cycles` ∈ {1, 2, 3, 4, 6, 8, 10}, with
`--max-parallel` fixed at 8 (production config).
Primary metric: convergence rate (global_ok / total functions).

The cycle sweep tests how many coordination rounds are needed:
- cap=1: single-shot generation, zero multi-round coordination
- cap=3-4: shallow chain convergence point
- cap=6-8: embedded bench saturation zone
- cap=10: full-module convergence ceiling (observed max=10 in production)

## Benchmarks

| bench | depth | functions | role |
|---|---|---|---|
| submodule-1 | 8 | 20 | chain-deep, demand-rich |
| submodule-2 | 10 | 52 | chain-deep, demand-storm |
| jc-voronoi | 4 | 21 | medium depth |
| hypatia | 4 | 55 | float-heavy geometry |
| sokol-shape | 2 | 33 | struct-heavy shapes |
| contiki-aes-ccm | 4 | 10 | embedded + lsl wall |

## Run

```
bash scripts/experiments/rq2/run.sh                 # full grid
bash scripts/experiments/rq2/run.sh --parallel-only  # RQ2a only
bash scripts/experiments/rq2/run.sh --cycle-only     # RQ2b only
bash scripts/experiments/rq2/run.sh submodule-1      # one bench, both sweeps
```

Serial by design: each run owns the machine for wall-clock fidelity.
Resumable (DONE markers skip completed cells).

## Aggregate

```
bash    scripts/experiments/post/collect.sh rq2
python3 scripts/experiments/post/aggregate.py scripts/experiments/results/rq2/summary.jsonl
```
