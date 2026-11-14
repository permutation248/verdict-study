#!/bin/sh
# Shadow-mount wrapper for frama-c inside the Preguss docker image.
#
# Purpose: make the WP proving semantics configurable WITHOUT touching the
# image's (bytecode-compiled) pipeline. Mount this script over a
# PATH-precedent frama-c location, e.g.:
#
#   docker run \
#     -v $PWD/framac-wpmodel-wrapper.sh:/usr/local/bin/frama-c:ro \
#     -e REAL_FRAMAC=<image frama-c path> \
#     -e PREGUSS_WP_MODEL=Typed+var+int+real \
#     pregussdocker/rte-spec-gen@sha256:... <args>
#
# Behavior:
#   - PREGUSS_WP_MODEL unset/empty  -> exec the real frama-c untouched
#     (sound-baseline runs use the PRISTINE image with no mount at all;
#     this wrapper exists only for the evaluation-semantics ablation column).
#   - PREGUSS_WP_MODEL set          -> inject "-wp-model $PREGUSS_WP_MODEL"
#     immediately AFTER the "-wp" token, and only when one is present.
#     Position matters: their WP command ends in "-then -report", and
#     anything appended after "-then" would configure the second phase
#     only — so plain appending would silently not apply. The -rte
#     preprocessing calls (no -wp) pass through untouched.
#
# REAL_FRAMAC must point at the image's actual frama-c binary (resolved at
# validation time; opam-installed images usually have it outside /usr/local).
set -e
REAL="${REAL_FRAMAC:-/usr/bin/frama-c.real}"

# Injection set, each independently optional:
#   PREGUSS_WP_MODEL   -> -wp-model <value>
#   PREGUSS_WP_NOINIT  -> -wp-no-init-const   (set to 1; matches Verdict's
#                         _wp_core() so const-global initial values are not
#                         assumed — the x509 known_* tables otherwise hand
#                         baselines facts Verdict has to prove)
if [ -n "${PREGUSS_WP_MODEL:-}" ] || [ "${PREGUSS_WP_NOINIT:-0}" = "1" ]; then
  case " $* " in
    *" -wp "*)
      # Rotate the arg list once, injecting right after -wp.
      i=0
      n=$#
      while [ "$i" -lt "$n" ]; do
        a=$1
        shift
        set -- "$@" "$a"
        if [ "$a" = "-wp" ]; then
          if [ -n "${PREGUSS_WP_MODEL:-}" ]; then
            set -- "$@" -wp-model "$PREGUSS_WP_MODEL"
          fi
          if [ "${PREGUSS_WP_NOINIT:-0}" = "1" ]; then
            set -- "$@" -wp-no-init-const
          fi
        fi
        i=$((i + 1))
      done
      ;;
  esac
fi

exec "$REAL" "$@"
