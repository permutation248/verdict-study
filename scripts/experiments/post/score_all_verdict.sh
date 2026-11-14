#!/bin/bash
# Score all verdict results EXCEPT full-module (scored separately due to 24h+ WP time).
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
cd "$ROOT"

SCORE="scripts/experiments/lib/score_final.py"
INPUT_DIR="scripts/experiments/baseline-specsyn/inputs"
RESULTS="${1:-scripts/experiments/results/rq1/verdict}"
OUT_DIR="${2:-scripts/experiments/results/rq1/verdict-scores}"
mkdir -p "$OUT_DIR"

for d in "$RESULTS"/*/rep*/run/final_annotated.c; do
  [ -f "$d" ] || continue
  bench=$(echo "$d" | rev | cut -d/ -f4 | rev)
  rep=$(echo "$d" | rev | cut -d/ -f3 | rev)

  # full-module scored separately (24h+ WP on 24k-line file)
  if [ "$bench" = "full-module" ]; then
    echo "=== SKIP $bench (score separately) ==="
    continue
  fi

  out="$OUT_DIR/$bench/$rep"
  mkdir -p "$out"

  if [ -f "$out/judge_final.json" ]; then
    echo "=== SKIP $bench/$rep (already scored) ==="
    continue
  fi

  input=$(ls "$INPUT_DIR/$bench"/*.c 2>/dev/null | head -1)
  EXPECT_ARG=()
  [ -n "$input" ] && EXPECT_ARG=(--expect-rte-from "$input")

  echo "=== scoring $bench/$rep ==="
  python3 "$SCORE" "$d" \
    --json "$out/judge_final.json" \
    --detail-csv "$out/judge_detail.csv" \
    --keep-log "$out/judge_framac.log" \
    ${EXPECT_ARG[@]+"${EXPECT_ARG[@]}"} \
    2>&1 | tee "$out/judge_stdout.txt"
  echo
done
echo "ALL-VERDICT-SCORING-DONE (full-module excluded)"
