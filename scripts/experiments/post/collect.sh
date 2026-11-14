#!/usr/bin/env bash
# Gather every per-run metrics.json under an RQ's results tree into one
# summary.jsonl. Idempotent: truncates and rebuilds, so re-running after more
# runs finish (or after re-running runs) never double-counts.
#
# Decoupled from running on purpose — aggregate whenever, without re-running.
#
#   collect.sh rq2     ->  scripts/experiments/results/rq2/summary.jsonl

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"

rq="${1:?usage: collect.sh <rq>   e.g. collect.sh rq2}"
base="$ROOT/scripts/experiments/results/$rq"
[ -d "$base" ] || { echo "no results dir: $base" >&2; exit 1; }

out="$base/summary.jsonl"
: > "$out"
n=0
while IFS= read -r m; do
  # metrics.json is sectioned & indented for humans; jsonl needs one line
  # per run. solve_summary (a text block) is dropped from the aggregate.
  python3 -c '
import json, sys
d = json.load(open(sys.argv[1]))
d.pop("solve_summary", None)
print(json.dumps(d, ensure_ascii=False))' "$m" >> "$out" || continue
  n=$((n + 1))
done < <(find "$base" -name metrics.json -not -path "*.partial_*" -not -path "*_archive*" | sort)

echo "collected $n runs -> $out"
