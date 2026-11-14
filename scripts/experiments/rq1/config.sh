#!/usr/bin/env bash
# RQ1 — baseline comparison: Verdict vs Preguss vs SpecSyn, one model.
#
# RQ1: On the final benchmark set, how do Verdict's discharge rates (RTE /
# spec, unified judge) compare with the two strongest published
# pipelines, run under matched semantics?
#
# Matched-semantics policy:
#   - model: claude-sonnet-4-6 for all three tools
#   - -wp-no-init-const injected into BOTH baselines' internal verification
#     (matches Verdict's _wp_core; const globals are proven, not assumed)
#   - evaluation model: util-libs/embedded benches run baselines with
#     Typed+var+int+real (their published semantics; auto via runner);
#     x509 submodules use the tools' own defaults
#   - final numbers come from lib/score_final.py (one judge for everyone)
#
# Hosts: everything runs on the evaluation host. All runs are SERIAL — one cell
# at a time (POOL_SLOTS = VERDICT_CONCURRENCY = 1) — so wall-clock is trustworthy
# and the grid runs reliably on any machine, same discipline as RQ2/RQ3. Each
# cell still uses its own intra-run parallelism (VERDICT_MAX_PARALLEL agents,
# WP_PROCS provers); only the number of cells in flight is serialized.

MODEL="claude-sonnet-4-6"

# Longest-processing-time-first queue for the two baseline pools (a freed slot
# starts the next bench), ordered roughly by function count so the heaviest
# benches start first. The comparison set is the SpecSyn-corpus utility
# libraries plus the contiki AES-CCM module; full-module is handled separately.
BASELINE_BENCHES_LPT=(
  contiki-aes-ccm submodule-2 sokol-shape jc-voronoi submodule-1
  ccan-time student-enrollment zpl-math hypatia char-utils maratis-math
  cgl punity stb-divide
)
POOL_SLOTS=1                  # serial: one baseline bench at a time

# Verdict cells: same comparison set (x509 x2 + contiki + util-libs).
# full-module is run/scored separately (209-function whole module).
VERDICT_BENCHES_LPT=(
  submodule-2 contiki-aes-ccm
  zpl-math hypatia char-utils maratis-math sokol-shape ccan-time
  jc-voronoi submodule-1 student-enrollment cgl punity stb-divide
)
VERDICT_TIMEOUT=43200          # 12 h hard cap per cell
VERDICT_CONCURRENCY=1          # serial: one cell at a time (trustworthy wall-clock)
VERDICT_MAX_PARALLEL=8         # agents per cell
VERDICT_WP_PROCS=8
VERDICT_MAX_CYCLES=24
VERDICT_MAX_TURNS=30

REPS=1

# Default locations for the SpecSyn artifact and venv (override if yours differ).
SPECSYN_ARTIFACT_DEFAULT=/path/to/SpecSyn-Artifact
SPECSYN_VENV_DEFAULT=/path/to/specsyn-venv
PREGUSS_IMAGE_AMD64=pregussdocker/rte-spec-gen:v2.0.0-amd64
