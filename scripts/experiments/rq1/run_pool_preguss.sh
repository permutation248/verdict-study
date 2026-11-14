#!/usr/bin/env bash
# RQ1 Preguss pool (amd64 image). 8 slots over the LPT queue;
# resumable (run_preguss.sh skips DONE cells). nohup this script.
set -uo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

export PREGUSS_IMAGE="${PREGUSS_IMAGE:-$PREGUSS_IMAGE_AMD64}"
# Assumes the host has direct egress; no proxy by default.
export PREGUSS_PROXY="${PREGUSS_PROXY:-}"

docker image inspect "$PREGUSS_IMAGE" >/dev/null 2>&1 || { echo "image $PREGUSS_IMAGE not loaded"; exit 2; }

printf '%s\n' "${BASELINE_BENCHES_LPT[@]}" | xargs -P "$POOL_SLOTS" -I{} \
  bash "$ROOT/scripts/experiments/baseline-preguss/run_preguss.sh" {} "$MODEL" 2 1
echo "PREGUSS-POOL-DONE"
