#!/usr/bin/env bash
# RQ3 — coordination-mechanism ablation. Sweep the ablation arm;
# everything else fixed at the RQ1/RQ2 fixed point.
#
# Independent variable : ablation arm (ARMS below -> --ablate flag)
# Fixed                : model, max-parallel = wp-jobs = 8, wp-procs,
#                        schedule, scope, max-turns, max-cycles, sound
#                        WP model (never combine with --wp-model)
#
# Grid size = |BENCHMARKS| x |ARMS| x REPS serial runs.
# Currently 6 x 4 x 3 = 72 runs.

# Chain-deep benches (submodule-1/2) carry the effect; the shallow benches
# (contiki, hypatia, jc-voronoi, sokol-shape) are the low-demand comparison.
BENCHMARKS=(submodule-1 submodule-2 hypatia sokol-shape jc-voronoi contiki-aes-ccm)

# arm name -> solve flag (looked up via arm_flag below; bash-3.2 safe).
ARMS=(baseline no-demands no-assume no-demands-no-assume)

arm_flag() {
  case "$1" in
    baseline)              echo "" ;;
    no-demands)            echo "--ablate demands" ;;
    no-assume)             echo "--ablate assume" ;;
    no-demands-no-assume)  echo "--ablate demands,assume" ;;
    *) return 1 ;;
  esac
}

REPS="${REPS:-3}"

MODEL="${MODEL:-claude-sonnet-4-6}"
MAX_PARALLEL=8
WP_PROCS=8
SCHEDULE=leaf-first
SCOPE=global
MAX_TURNS=30
MAX_CYCLES=24

# Overall solve timeout per run (seconds). 12h (same as RQ1).
TIMEOUT=43200
