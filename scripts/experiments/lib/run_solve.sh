#!/usr/bin/env bash
# ATOM: run ONE fully-specified verdict solve, capture its output into a given
# directory, and emit that run's metrics row. Generic and RQ-agnostic — every
# RQ driver builds on this. The caller is responsible for environment setup
# (PATH/proxy/.env) and for choosing the output directory.
#
# Runs are SERIAL by design: each run's wall-clock IS the measurement, so two
# runs must never share the machine.
#
# Usage:
#   run_solve.sh --bench submodule-1 --out <dir> \
#     --max-parallel 8 --wp-procs 8 --wp-jobs 8 \
#     --schedule leaf-first --scope global --model claude-sonnet-4-6 \
#     --timeout 10800 --max-turns 30 --max-cycles 24
#
# Produces under <dir>/:
#   run/          the moved verdict run directory (run.json, events.jsonl, ...)
#   solve.log     solve stdout/stderr
#   metrics.json  one flat metrics row (config knobs + parsed results)
#   DONE          marker written only on a clean finish (for resumability)

set -uo pipefail

LIB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck disable=SC1091
source "$LIB_DIR/benchmarks.sh"

BENCH="" OUT="" MAX_PARALLEL="" WP_PROCS=8 WP_JOBS="" SCHEDULE="leaf-first"
SCOPE="global" MODEL="claude-sonnet-4-6" TIMEOUT=10800 MAX_TURNS=30 MAX_CYCLES=24
EXTRA=""
META_ARGS=()

while [ $# -gt 0 ]; do
  case "$1" in
    --bench) BENCH="$2"; shift 2 ;;
    --out) OUT="$2"; shift 2 ;;
    --max-parallel) MAX_PARALLEL="$2"; shift 2 ;;
    --wp-procs) WP_PROCS="$2"; shift 2 ;;
    --wp-jobs) WP_JOBS="$2"; shift 2 ;;
    --schedule) SCHEDULE="$2"; shift 2 ;;
    --scope) SCOPE="$2"; shift 2 ;;
    --model) MODEL="$2"; shift 2 ;;
    --timeout) TIMEOUT="$2"; shift 2 ;;
    --max-turns) MAX_TURNS="$2"; shift 2 ;;
    --max-cycles) MAX_CYCLES="$2"; shift 2 ;;
    --extra) EXTRA="$2"; shift 2 ;;
    # Extra metrics column, e.g. --meta arm=no-demands (repeatable).
    # Forwarded to extract_metrics.py verbatim so the RQ's independent
    # variable lands in metrics.json even when it is not a solve knob.
    --meta) META_ARGS+=("--${2%%=*}" "${2#*=}"); shift 2 ;;
    *) echo "run_solve: unknown arg $1" >&2; exit 2 ;;
  esac
done

[ -n "$BENCH" ] && [ -n "$OUT" ] && [ -n "$MAX_PARALLEL" ] || {
  echo "run_solve: --bench, --out, --max-parallel required" >&2; exit 2; }
[ -n "$WP_JOBS" ] || WP_JOBS="$MAX_PARALLEL"   # default policy: wp-jobs = max-parallel

SRC="$(bench_source "$BENCH")" || { echo "unknown bench $BENCH" >&2; exit 2; }
HDR="$(bench_header "$BENCH")"
STEM="$(bench_stem "$BENCH")"
WP_MODEL="${WP_MODEL:-$(bench_wp_model "$BENCH")}"

# Single self-contained benchmarks (crypto modules) have no separate header.
HDR_ARG=()
[ -n "$HDR" ] && HDR_ARG=(-H "$HDR")

WP_MODEL_ARG=()
[ -n "$WP_MODEL" ] && WP_MODEL_ARG=(--wp-model "$WP_MODEL")

# Never overwrite previous results (verdict itself never reuses a run dir).
# Deleting or archiving an old cell is a deliberate caller-level decision,
# not something the atom does silently.
if [ -e "$OUT/run" ] || [ -e "$OUT/metrics.json" ] || [ -e "$OUT/DONE" ]; then
  echo "run_solve: refusing to overwrite existing results in $OUT — pass a fresh --out" >&2
  exit 2
fi
mkdir -p "$OUT"

# Hard wall to catch a hung process that ignores solve's own --timeout.
HARD_TIMEOUT=$(awk "BEGIN{print $TIMEOUT + 600}")

# --no-sync: use the pre-built venv as-is. Experiments must not re-resolve deps
# mid-run, and a run host with no outbound network cannot reach PyPI anyway.
start_epoch=$(date +%s)
timeout "${HARD_TIMEOUT}s" \
  uv run --no-sync verdict solve \
    -f "$SRC" ${HDR_ARG[@]+"${HDR_ARG[@]}"} \
    --scope "$SCOPE" \
    --max-parallel "$MAX_PARALLEL" \
    --wp-procs "$WP_PROCS" --wp-jobs "$WP_JOBS" \
    --schedule "$SCHEDULE" \
    --model "$MODEL" \
    --max-turns "$MAX_TURNS" --max-cycles "$MAX_CYCLES" \
    --timeout "$TIMEOUT" \
    ${WP_MODEL_ARG[@]+"${WP_MODEL_ARG[@]}"} \
    $EXTRA \
  > "$OUT/solve.log" 2>&1 < /dev/null
solve_rc=$?
end_epoch=$(date +%s)
wall_min=$(awk "BEGIN{printf \"%.2f\", ($end_epoch-$start_epoch)/60}")

# Capture the run directory verdict just created. The CLI prints its own
# "Logs: <dir>" line at startup — parse THAT for exact ownership. Sorting
# by mtime (ls -dt) races: a concurrent same-stem run keeps touching its
# live dir, sorts newest, and would get captured/moved mid-run.
run_dir="$(grep -m1 '^Logs: ' "$OUT/solve.log" 2>/dev/null | sed 's/^Logs: //')"
if [ -z "$run_dir" ]; then
  run_dir="$(ls -dt logs/${STEM}_*/ 2>/dev/null | head -1)"
fi
captured="none"
if [ -n "$run_dir" ] && [ -d "$run_dir" ]; then
  mv "$run_dir" "$OUT/run"
  captured="$OUT/run"
fi

# Emit metrics row (parsed results + harness-measured config & wall-clock).
python3 "$LIB_DIR/../post/extract_metrics.py" "$OUT/run" \
  --benchmark "$BENCH" \
  --max_parallel "$MAX_PARALLEL" \
  --wp_procs "$WP_PROCS" --wp_jobs "$WP_JOBS" \
  --schedule "$SCHEDULE" --scope "$SCOPE" --model "$MODEL" \
  --timeout_s "$TIMEOUT" \
  --wall_clock_min "$wall_min" \
  --solve_rc "$solve_rc" \
  --hostname "$(hostname)" \
  ${META_ARGS[@]+"${META_ARGS[@]}"} \
  > "$OUT/metrics.json"

echo "run_solve: bench=$BENCH mp=$MAX_PARALLEL rc=$solve_rc wall=${wall_min}m captured=$captured"
[ "$captured" != "none" ] && touch "$OUT/DONE"
exit 0
