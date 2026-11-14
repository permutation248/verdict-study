#!/usr/bin/env bash
# RQ1 SpecSyn pool. 8 slots over the LPT queue; resumable
# (run_specsyn.sh skips DONE cells). nohup this script.
set -uo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/config.sh"

# Artifact/venv locations (override the defaults in config.sh if yours differ).
export SPECSYN_ARTIFACT="${SPECSYN_ARTIFACT:-$SPECSYN_ARTIFACT_DEFAULT}"
export SPECSYN_VENV="${SPECSYN_VENV:-$SPECSYN_VENV_DEFAULT}"

command -v frama-c >/dev/null || { echo "frama-c not on PATH (source env_setup.sh)"; exit 2; }

printf '%s\n' "${BASELINE_BENCHES_LPT[@]}" | xargs -P "$POOL_SLOTS" -I{} \
  bash "$ROOT/scripts/experiments/baseline-specsyn/run_specsyn.sh" {} "$MODEL" 1
echo "SPECSYN-POOL-DONE"
