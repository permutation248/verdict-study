#!/usr/bin/env bash
# RQ2 — hyperparameter sensitivity: parallelism scaling + cycle budget.
#
# Two independent sweeps, same bench set:
#   RQ2a: --max-parallel in {1, 2, 4, 16, 32}, max-cycles fixed at 24
#   RQ2b: --max-cycles  in {1, 2, 4, 8, 16}, max-parallel fixed at 8
#
# Each sweep isolates one dimension while holding the other at its
# default. Wall-clock is a primary metric for RQ2a (runs are serial,
# owning the machine); convergence rate is primary for RQ2b.
#
# Grid size = |BENCHMARKS| x (|PARALLEL| + |CYCLES|) x REPS
# Currently 6 x (5 + 5) x 3 = 180 runs.

# Bench selection: x509 submodules (chain-deep), util-libs with call
# chains (jc-voronoi/hypatia/sokol-shape), and contiki-aes-ccm
# (embedded, lsl wall). Covers depth 2-10, functions 10-55.
BENCHMARKS=(submodule-1 submodule-2 hypatia sokol-shape jc-voronoi contiki-aes-ccm)

# ── RQ2a: parallelism scaling ──
MAX_PARALLEL_VALUES=(1 2 4 16 32)
RQ2A_MAX_CYCLES=24          # fixed cycle budget (matches RQ1 default)

# ── RQ2b: cycle budget scaling ──
# mc=24 is the RQ1 default — reuse RQ1 data directly, no separate run needed.
MAX_CYCLES_VALUES=(1 2 4 8 16)
RQ2B_MAX_PARALLEL=8         # fixed parallelism (production config)

# ── Shared fixed knobs ──
REPS="${REPS:-3}"
WP_PROCS=8
SCHEDULE=leaf-first
SCOPE=global
MODEL=claude-sonnet-4-6
MAX_TURNS=30

# Overall solve timeout per run (seconds). 12h (same as RQ1).
TIMEOUT=43200
