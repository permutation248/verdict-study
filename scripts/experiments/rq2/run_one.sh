#!/usr/bin/env bash
# Run ONE RQ2 configuration in isolation:
#   run_one.sh <benchmark> <max-parallel> <max-cycles> [rep]
#
# Supports both RQ2a (parallel sweep) and RQ2b (cycle sweep) — the
# caller picks which knob to vary and which to hold fixed.
#
# Output: scripts/experiments/results/rq2/<bench>/mp<NN>-mc<NN>/rep<rep>/
# Skips (no-op) if that run already has a DONE marker — safe to re-invoke.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VERDICT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
export VERDICT_ROOT
RUN_SOLVE="$SCRIPT_DIR/../lib/run_solve.sh"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/../lib/env.sh"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

bench="${1:?usage: run_one.sh <benchmark> <max-parallel> <max-cycles> [rep]}"
mp="${2:?usage: run_one.sh <benchmark> <max-parallel> <max-cycles> [rep]}"
mc="${3:?usage: run_one.sh <benchmark> <max-parallel> <max-cycles> [rep]}"
rep="${4:-1}"

setup_env || exit 1

out="scripts/experiments/results/rq2/$bench/mp$(printf '%02d' "$mp")-mc$(printf '%02d' "$mc")/rep$rep"
if [ -f "$out/DONE" ]; then
  echo "SKIP $bench mp=$mp mc=$mc rep=$rep (already DONE: $out)"
  exit 0
fi

if [ -d "$out" ] && [ -n "$(ls -A "$out" 2>/dev/null)" ]; then
  archived="${out}.partial_$(date +%Y%m%d_%H%M%S)"
  mv "$out" "$archived"
  echo "ARCHIVED incomplete attempt: $archived"
fi

bash "$RUN_SOLVE" \
  --bench "$bench" --out "$out" \
  --max-parallel "$mp" --wp-procs "$WP_PROCS" --wp-jobs "$mp" \
  --schedule "$SCHEDULE" --scope "$SCOPE" --model "$MODEL" \
  --timeout "$TIMEOUT" --max-turns "$MAX_TURNS" --max-cycles "$mc"
