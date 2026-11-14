#!/usr/bin/env bash
# Run the RQ2 grid — SERIAL (each run owns the machine for wall-clock).
#
#   run.sh                      # full grid: RQ2a + RQ2b
#   run.sh --parallel-only      # RQ2a only
#   run.sh --cycle-only         # RQ2b only
#   run.sh submodule-1          # subset of benchmarks, both sweeps
#
# Reps are the OUTER loop so each round runs every config once, spreading
# any time-of-day API drift evenly. Resumable via DONE markers.
#
# Launch in background:
#   nohup bash scripts/experiments/rq2/run.sh > /tmp/rq2_sweep.log 2>&1 < /dev/null &

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

do_parallel=true
do_cycle=true
benches=()
for arg in "$@"; do
  case "$arg" in
    --parallel-only) do_cycle=false ;;
    --cycle-only)    do_parallel=false ;;
    *)               benches+=("$arg") ;;
  esac
done
[ ${#benches[@]} -eq 0 ] && benches=("${BENCHMARKS[@]}")

total=0
$do_parallel && total=$((total + ${#benches[@]} * ${#MAX_PARALLEL_VALUES[@]} * REPS))
$do_cycle    && total=$((total + ${#benches[@]} * ${#MAX_CYCLES_VALUES[@]} * REPS))
run=0
echo "=== RQ2 sweep: $total runs | benches=${benches[*]} ==="

for rep in $(seq 1 "$REPS"); do
  # ── RQ2a: parallel scaling (fixed cycles=25) ──
  if $do_parallel; then
    for b in "${benches[@]}"; do
      for mp in "${MAX_PARALLEL_VALUES[@]}"; do
        run=$((run + 1))
        echo "$(date '+%F %T') [$run/$total] RQ2a $b mp=$mp mc=$RQ2A_MAX_CYCLES rep=$rep"
        bash "$SCRIPT_DIR/run_one.sh" "$b" "$mp" "$RQ2A_MAX_CYCLES" "$rep"
      done
    done
  fi

  # ── RQ2b: cycle scaling (fixed parallel=8) ──
  if $do_cycle; then
    for b in "${benches[@]}"; do
      for mc in "${MAX_CYCLES_VALUES[@]}"; do
        run=$((run + 1))
        echo "$(date '+%F %T') [$run/$total] RQ2b $b mp=$RQ2B_MAX_PARALLEL mc=$mc rep=$rep"
        bash "$SCRIPT_DIR/run_one.sh" "$b" "$RQ2B_MAX_PARALLEL" "$mc" "$rep"
      done
    done
  fi
done

echo "=== sweep done. Aggregate with:"
echo "    bash    scripts/experiments/post/collect.sh rq2"
echo "    python3 scripts/experiments/post/aggregate.py scripts/experiments/results/rq2/summary.jsonl"
