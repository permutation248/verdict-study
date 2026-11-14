#!/usr/bin/env bash
# Run the Preguss baseline (official Docker image) on ONE Verdict benchmark.
#
#   run_preguss.sh <bench> [model] [iters] [rep]
#   run_preguss.sh submodule-1 claude-4 2 1
#
# Runs the official Preguss image (pregussdocker/rte-spec-gen). The image is
# published for linux/arm64; on an x86 host set PREGUSS_IMAGE to an amd64 build.
#
# Interface mirrors the artifact's own docker-run.sh `cli` path:
#   - endpoint/keys are ENV-CONFIGURABLE (*_ZHI vars) — pristine image,
#     zero patching
#   - input dir mounted writable at /app/host_input (Frama-C writes
#     intermediates next to the source)
#   - cli.pyc -f <file> -m <model> -i <iters>
#
# Optional evaluation-semantics ablation (OUR overlay, off by default):
#   PREGUSS_WP_MODEL=Typed+var+int+real run_preguss.sh ...
# mounts docker/framac-wpmodel-wrapper.sh over a PATH-precedent frama-c.
# REAL_FRAMAC inside the image must be validated once per image digest
# (docker run --rm --entrypoint bash IMG -c 'eval $(opam env); which frama-c').

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
# shellcheck disable=SC1091
source "$ROOT/scripts/experiments/lib/benchmarks.sh"

bench="${1:?usage: run_preguss.sh <bench> [model] [iters] [rep]}"
model="${2:-claude-4}"
iters="${3:-2}"
rep="${4:-1}"

IMAGE="${PREGUSS_IMAGE:-pregussdocker/rte-spec-gen:latest}"
# Default: no proxy (direct egress). Set PREGUSS_PROXY to route the
# container's LLM egress through an HTTP proxy if the host needs one.
PROXY="${PREGUSS_PROXY:-}"
# Their anthropic relay path appends /v1/messages itself -> bare host
# (a /v1 suffix here produced POST /v1/v1/messages, observed 2026-06-11).
# The OpenAI-client path follows the usual convention (base includes /v1).
ANTHROPIC_BASE="${PREGUSS_ANTHROPIC_BASE:-https://llm-api-endpoint.example.com}"
OPENAI_BASE="${PREGUSS_OPENAI_BASE:-https://llm-api-endpoint.example.com}"
# (both bare: the v2.0.0 image appends /v1/messages itself for claude
# models regardless of which *_ZHI base it reads — observed empirically.)

SRC="$(bench_source "$bench")" || { echo "unknown bench $bench" >&2; exit 2; }
SRC_ABS="$ROOT/$SRC"
[ -f "$SRC_ABS" ] || { echo "bench file missing: $SRC_ABS" >&2; exit 2; }

# API key: reuse the repo .env (never committed). Their pipeline speaks
# OpenAI chat-completions through the *_ZHI relay vars; endpoint B
# serves that format natively (validated 2026-06-11, local + direct).
KEY="$(grep -m1 '^ANTHROPIC_API_KEY=' "$ROOT/.env" | cut -d= -f2)"
[ -n "$KEY" ] || { echo "no ANTHROPIC_API_KEY in $ROOT/.env" >&2; exit 2; }

docker image inspect "$IMAGE" >/dev/null 2>&1 || {
  echo "image $IMAGE not loaded — pull it first, e.g."
  echo "  docker pull $IMAGE"
  exit 2
}

out="$ROOT/scripts/experiments/results/baseline-preguss/$bench/$model/rep$rep"
if [ -e "$out/DONE" ]; then
  echo "SKIP $bench model=$model rep=$rep (already DONE: $out)"
  exit 0
fi
if [ -d "$out" ] && [ -n "$(ls -A "$out" 2>/dev/null)" ]; then
  mv "$out" "${out}.partial_$(date +%Y%m%d_%H%M%S)"
fi
mkdir -p "$out"/docker_workspace/{output,logs,llm_interactions,cache} "$out/input"

# Isolated writable copy of the bench source (the container writes
# Frama-C intermediates next to it; benchmarks/ must stay pristine).
# Benches with a separate header (x509 submodules) need it alongside.
cp "$SRC_ABS" "$out/input/"
HDR="$(bench_header "$bench" 2>/dev/null || true)"
[ -n "$HDR" ] && cp "$ROOT/$HDR" "$out/input/"
fname="$(basename "$SRC_ABS")"

# In-image frama-c path (resolve once per image digest via:
#   docker run --rm --entrypoint bash IMG -c 'eval $(opam env); which frama-c').
REAL_FRAMAC="${REAL_FRAMAC:-/root/.opam/4.14.0/bin/frama-c}"

# Evaluation-semantics policy: embedded/ and util-libs/ benches run Preguss
# WITH the +real wrapper so its internal verification matches the unified judge
# semantics (float-heavy util-libs would otherwise dead-end in their repair
# loop under sound Typed). The x509 submodules stay on the image's defaults.
if [ -z "${PREGUSS_WP_MODEL:-}" ]; then
  # shellcheck disable=SC1091
  source "$ROOT/scripts/experiments/lib/benchmarks.sh"
  PREGUSS_WP_MODEL="$(bench_wp_model "$bench")"
  [ -n "$PREGUSS_WP_MODEL" ] && \
    echo "preguss: defaulting PREGUSS_WP_MODEL=$PREGUSS_WP_MODEL for bench '$bench'"
fi

# -wp-no-init-const matches Verdict's _wp_core(); injected for ALL benches
# (override with PREGUSS_WP_NOINIT=0).
PREGUSS_WP_NOINIT="${PREGUSS_WP_NOINIT:-1}"

WRAPPER_MOUNT=()
WRAPPER_ENV=()
# `eval $(opam env)` PREPENDS the opam bin dir, which would shadow a wrapper
# mounted at /usr/local/bin — so when the wrapper is active the container
# command re-prepends /usr/local/bin after the opam eval.
PATH_FIX=""
if [ -n "${PREGUSS_WP_MODEL:-}" ] || [ "$PREGUSS_WP_NOINIT" = "1" ]; then
  WRAPPER_MOUNT=(-v "$SCRIPT_DIR/docker/framac-wpmodel-wrapper.sh:/usr/local/bin/frama-c:ro")
  WRAPPER_ENV=(-e "PREGUSS_WP_MODEL=${PREGUSS_WP_MODEL:-}"
               -e "PREGUSS_WP_NOINIT=$PREGUSS_WP_NOINIT"
               -e "REAL_FRAMAC=$REAL_FRAMAC")
  PATH_FIX="export PATH=/usr/local/bin:\$PATH && "
fi

# PREGUSS_DOCKER_NET=host lets the container reach a host-loopback proxy when
# the host's direct egress is flaky; combine with a loopback PREGUSS_PROXY.
#
# Note: the image's LLM client uses a custom httpx client that ignores proxy
# env vars, so proxy vars alone do NOT reroute it. A DNS-level interpose works:
# PREGUSS_ADD_HOST=<api-host>:127.0.0.1 plus base URLs pointing at a local
# TLS pass-through listener. TLS stays end-to-end.
NET_ARGS=()
[ -n "${PREGUSS_DOCKER_NET:-}" ] && NET_ARGS=(--network "$PREGUSS_DOCKER_NET")
[ -n "${PREGUSS_ADD_HOST:-}" ] && NET_ARGS+=(--add-host "$PREGUSS_ADD_HOST")

PREGUSS_TIMEOUT="${PREGUSS_TIMEOUT:-86400}"

echo "preguss: bench=$bench model=$model iters=$iters rep=$rep image=$IMAGE net=${PREGUSS_DOCKER_NET:-bridge} proxy=${PROXY:-none} timeout=${PREGUSS_TIMEOUT}s"
start_epoch=$(date +%s)
timeout "${PREGUSS_TIMEOUT}s" \
docker run --rm \
  ${NET_ARGS[@]+"${NET_ARGS[@]}"} \
  -e OPENAI_API_KEY_ZHI="$KEY" \
  -e OPENAI_BASE_URL_ZHI="$OPENAI_BASE" \
  -e ANTHROPIC_API_KEY_ZHI="$KEY" \
  -e ANTHROPIC_BASE_URL_ZHI="$ANTHROPIC_BASE" \
  -e HTTPS_PROXY="$PROXY" -e HTTP_PROXY="$PROXY" \
  -e NO_PROXY="localhost,127.0.0.1" \
  -v "$out/docker_workspace/output:/app/output" \
  -v "$out/docker_workspace/logs:/app/logs" \
  -v "$out/docker_workspace/llm_interactions:/app/results" \
  -v "$out/docker_workspace/cache:/app/verification_cache" \
  -v "$out/input:/app/host_input" \
  ${WRAPPER_MOUNT[@]+"${WRAPPER_MOUNT[@]}"} \
  ${WRAPPER_ENV[@]+"${WRAPPER_ENV[@]}"} \
  --entrypoint /bin/bash \
  "$IMAGE" \
  -c "eval \$(opam env) && ${PATH_FIX}cd /app && cd preprocessing/bindings && source setup_env.sh && cd ../.. && python3 cli.pyc -f /app/host_input/$fname -m $model -i $iters" \
  > "$out/run.log" 2>&1
rc=$?
end_epoch=$(date +%s)

# Record provenance + outcome. Their per-VO scoring is printed in the
# run log's VERIFICATION SUMMARY; the annotated outputs land under
# docker_workspace/output for post-hoc score_rte.py scoring.
{
  echo "bench=$bench model=$model iters=$iters rep=$rep"
  echo "image=$IMAGE"
  docker image inspect "$IMAGE" --format 'image_id={{.Id}}' 2>/dev/null
  echo "rc=$rc wall_min=$(awk "BEGIN{printf \"%.2f\", ($end_epoch-$start_epoch)/60}")"
  echo "wp_model_override=${PREGUSS_WP_MODEL:-none}"
  echo "wp_noinit_injected=$PREGUSS_WP_NOINIT"
} > "$out/provenance.txt"

grep -A 30 "VERIFICATION SUMMARY" "$out/run.log" | head -40 > "$out/verification_summary.txt" 2>/dev/null || true

# DONE only for a run whose LLM channel actually worked: their pipeline
# exits 0 even when every LLM call fails (degrades to no-spec sweeps).
# grep -c prints the 0 itself (exiting 1) on no match — appending a fallback
# echo would yield "0\n0" and break the -eq test below (hit 2026-06-12).
# Count LLM failures BEFORE the VERIFICATION SUMMARY only — the summary
# itself prints "Connection error" as a status tag for unverified VOs,
# not a runtime network failure (false positive discovered 2026-06-13).
sumline=$(grep -n "VERIFICATION SUMMARY" "$out/run.log" 2>/dev/null | head -1 | cut -d: -f1)
if [ -n "$sumline" ]; then
  bad_llm=$(head -"$sumline" "$out/run.log" | grep -ciE "invalid url|no api key|authenticationerror|connection error|econnreset" 2>/dev/null)
else
  bad_llm=$(grep -ciE "invalid url|no api key|authenticationerror|connection error|econnreset" "$out/run.log" 2>/dev/null)
fi
bad_llm=${bad_llm:-0}
if [ $rc -eq 124 ]; then
  echo "preguss: TIMEOUT after ${PREGUSS_TIMEOUT}s — preserving partial results"
  echo "timeout=${PREGUSS_TIMEOUT}s" >> "$out/provenance.txt"
  touch "$out/DONE"
elif [ $rc -eq 0 ] && [ "${bad_llm:-0}" -eq 0 ]; then
  touch "$out/DONE"
else
  echo "llm_failures=$bad_llm" >> "$out/provenance.txt"
fi
echo "preguss: rc=$rc -> $out"
exit $rc
