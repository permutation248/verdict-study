#!/usr/bin/env bash
# Run the RQ3 ablation grid by repeatedly calling run_one.sh — SERIAL (each
# run owns the machine; concurrent runs would contaminate wall-clock, and
# wall-clock is a primary RQ3 metric for the no-assume arm).
#
#   run.sh                       # full grid: all BENCHMARKS x ARMS x REPS
#   run.sh submodule-1           # one benchmark across all arms
#   run.sh submodule-1 hypatia   # a subset of benchmarks
#
# Reps are the OUTER loop so each round runs every cell once, spreading any
# time-of-day API drift evenly across arms. Resumable: cells with a DONE
# marker are skipped (run_one.sh handles that), so kill/restart is safe.
#
# Launch in background:
#   nohup bash scripts/experiments/rq3/run.sh > /tmp/rq3_sweep.log 2>&1 < /dev/null &

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

benches=("$@")
[ ${#benches[@]} -eq 0 ] && benches=("${BENCHMARKS[@]}")

total=$(( ${#benches[@]} * ${#ARMS[@]} * REPS ))
run=0
echo "=== RQ3 sweep: $total runs | benches=${benches[*]} | arms=${ARMS[*]} | reps=$REPS ==="

for rep in $(seq 1 "$REPS"); do
  for b in "${benches[@]}"; do
    for arm in "${ARMS[@]}"; do
      run=$((run + 1))
      echo "$(date '+%F %T') [$run/$total] $b arm=$arm rep=$rep"
      bash "$SCRIPT_DIR/run_one.sh" "$b" "$arm" "$rep"
    done
  done
done

echo "=== sweep done. Aggregate with:"
echo "    bash    scripts/experiments/post/collect.sh rq3"
echo "    python3 scripts/experiments/post/aggregate.py scripts/experiments/results/rq3/summary.jsonl --by arm"
