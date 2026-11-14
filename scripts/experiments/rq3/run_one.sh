#!/usr/bin/env bash
# Run ONE RQ3 configuration in isolation:  run_one.sh <benchmark> <arm> [rep]
#
# Arms are defined in config.sh (baseline / no-demands / no-assume /
# no-demands-no-assume). Self-contained: sets up the environment itself.
#
# Output: scripts/experiments/results/rq3/<bench>/<arm>/rep<rep>/
# Skips (no-op) if that run already has a DONE marker — safe to re-invoke.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VERDICT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
export VERDICT_ROOT
# setup_env sources the repo's env_setup.sh, which defines its OWN $SCRIPT_DIR
# (repo root) and clobbers ours. Capture every path we need now.
RUN_SOLVE="$SCRIPT_DIR/../lib/run_solve.sh"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/../lib/env.sh"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

bench="${1:?usage: run_one.sh <benchmark> <arm> [rep]}"
arm="${2:?usage: run_one.sh <benchmark> <arm> [rep]}"
rep="${3:-1}"

flag="$(arm_flag "$arm")" || { echo "unknown arm: $arm (see config.sh ARMS)" >&2; exit 2; }

setup_env || exit 1

out="scripts/experiments/results/rq3/$bench/$arm/rep$rep"
if [ -f "$out/DONE" ]; then
  echo "SKIP $bench arm=$arm rep=$rep (already DONE: $out)"
  exit 0
fi

# Incomplete leftovers (killed/crashed attempt): archive, never delete.
if [ -d "$out" ] && [ -n "$(ls -A "$out" 2>/dev/null)" ]; then
  archived="${out}.partial_$(date +%Y%m%d_%H%M%S)"
  mv "$out" "$archived"
  echo "ARCHIVED incomplete attempt: $archived"
fi

EXTRA_ARG=()
[ -n "$flag" ] && EXTRA_ARG=(--extra "$flag")

bash "$RUN_SOLVE" \
  --bench "$bench" --out "$out" \
  --max-parallel "$MAX_PARALLEL" --wp-procs "$WP_PROCS" --wp-jobs "$MAX_PARALLEL" \
  --schedule "$SCHEDULE" --scope "$SCOPE" --model "$MODEL" \
  --timeout "$TIMEOUT" --max-turns "$MAX_TURNS" --max-cycles "$MAX_CYCLES" \
  --meta "arm=$arm" \
  ${EXTRA_ARG[@]+"${EXTRA_ARG[@]}"}
