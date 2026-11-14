#!/usr/bin/env bash
# Smoke test: run each benchmark ONCE (max-parallel=8, rep=1) to confirm it
# runs end-to-end before committing it to the full grid. Especially important
# for the crypto benchmarks, which have never completed a run.
#
#   smoke.sh                 # smoke-test every benchmark in config.sh
#   smoke.sh aes-ccm         # smoke-test just one
#
# A smoke run IS the grid's mp=8 / rep1 configuration — its DONE marker means
# run.sh skips it later, so no work is wasted.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

benches=("$@")
[ ${#benches[@]} -eq 0 ] && benches=("${BENCHMARKS[@]}")

echo "=== RQ2 smoke test: one mp=8 run per benchmark: ${benches[*]} ==="
for b in "${benches[@]}"; do
  echo "$(date '+%F %T') smoke $b"
  bash "$SCRIPT_DIR/run_one.sh" "$b" 8 1
done

echo "=== smoke test done. Inspect each results/rq2/<bench>/mp08/rep1/metrics.json"
echo "    (status=complete & converged=true => the benchmark runs). Then: run.sh ==="
