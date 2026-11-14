#!/usr/bin/env bash
# Environment setup for experiment runs.
# VERDICT_ROOT must be set by the caller (run.sh resolves it from its own path).

setup_env() {
  cd "$VERDICT_ROOT" || { echo "VERDICT_ROOT not set / invalid" >&2; return 1; }

  # uv's default install location; non-interactive shells skip .bashrc.
  [ -d "$HOME/.local/bin" ] && export PATH="$HOME/.local/bin:$PATH"

  # Put frama-c and its provers (z3 / cvc5 / alt-ergo) on PATH.
  # build.sh records the toolchain location; env_setup.sh applies it.
  # shellcheck disable=SC1091
  [ -f env_setup.sh ] && source env_setup.sh >/dev/null 2>&1

  # Load API credentials from .env (ANTHROPIC_API_KEY, optional ANTHROPIC_BASE_URL).
  # ${VAR:-} guards against `set -u` (callers run with nounset).
  if [ -f .env ] && [ -z "${ANTHROPIC_BASE_URL:-}" ]; then
    set -a; source .env; set +a
  fi
}
