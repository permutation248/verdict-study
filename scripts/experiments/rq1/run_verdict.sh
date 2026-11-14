#!/usr/bin/env bash
# RQ1 Verdict cells. Low concurrency for trustworthy wall times; resumable
# (run_solve refuses existing cells, skip via DONE check). nohup this script.
# Caller env: PATH with uv, opam env for frama-c.
set -uo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

cd "$ROOT"
run_cell() {
  local bench="$1"
  local out="scripts/experiments/results/rq1/verdict/$bench/rep1"
  if [ -e "$out/DONE" ]; then echo "SKIP $bench (DONE)"; return 0; fi
  [ -d "$out" ] && mv "$out" "${out}.partial_$(date +%Y%m%d_%H%M%S)"
  bash scripts/experiments/lib/run_solve.sh \
    --bench "$bench" --out "$out" \
    --max-parallel "$VERDICT_MAX_PARALLEL" --wp-procs "$VERDICT_WP_PROCS" \
    --model "$MODEL" --timeout "$VERDICT_TIMEOUT" \
    --max-turns "$VERDICT_MAX_TURNS" --max-cycles "$VERDICT_MAX_CYCLES"
}
export -f run_cell
export VERDICT_MAX_PARALLEL VERDICT_WP_PROCS MODEL VERDICT_TIMEOUT VERDICT_MAX_TURNS VERDICT_MAX_CYCLES

printf '%s\n' "${VERDICT_BENCHES_LPT[@]}" | xargs -P "$VERDICT_CONCURRENCY" -I{} \
  bash -c 'run_cell "$@"' _ {}
echo "VERDICT-CELLS-DONE"
