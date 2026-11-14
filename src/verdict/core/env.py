"""Build SDK subprocess environment from .env file."""

from __future__ import annotations

import os
import re
from pathlib import Path

from dotenv import dotenv_values


def _load_dotenv() -> dict[str, str]:
    env_file = Path(__file__).resolve().parents[3] / ".env"
    return dict(dotenv_values(env_file)) if env_file.exists() else {}


def _resolve(key: str, dotenv: dict[str, str]) -> str | None:
    return dotenv.get(key) or os.environ.get(key) or None


_NUMBERED_KEY_RE = re.compile(r"^ANTHROPIC_API_KEY_(\d+)$")


def get_sdk_env_pool() -> list[dict[str, str]]:
    """Return one env dict per available API key.

    Collects ``ANTHROPIC_API_KEY_1``, ``ANTHROPIC_API_KEY_2``, … from the
    ``.env`` file and ``os.environ`` (numbered keys take priority).  Falls
    back to the single ``ANTHROPIC_API_KEY`` when no numbered keys exist.

    Every dict shares the same ``ANTHROPIC_BASE_URL`` (if set).
    """
    dotenv = _load_dotenv()
    base_url = _resolve("ANTHROPIC_BASE_URL", dotenv)

    numbered: dict[int, str] = {}
    for source in (dotenv, os.environ):
        for k, v in source.items():
            m = _NUMBERED_KEY_RE.match(k)
            if m and v:
                numbered.setdefault(int(m.group(1)), v)

    if numbered:
        keys = [numbered[i] for i in sorted(numbered)]
    else:
        single = _resolve("ANTHROPIC_API_KEY", dotenv)
        keys = [single] if single else []

    pool: list[dict[str, str]] = []
    for key in keys:
        env: dict[str, str] = {
            "ANTHROPIC_API_KEY": key,
            "CLAUDE_CODE_MAX_OUTPUT_TOKENS": "64000",
        }
        if base_url:
            env["ANTHROPIC_BASE_URL"] = base_url
        pool.append(env)
    return pool


def _model_family(model: str) -> str:
    """Leading alphabetic prefix of a model name, uppercased: ``gpt-5.5`` →
    ``GPT``, ``deepseek-v4-pro`` → ``DEEPSEEK``, ``qwen3.7-max`` → ``QWEN``
    (version digits/dots are stripped so one key covers a whole family,
    and the env-var name stays valid)."""
    m = re.match(r"[A-Za-z]+", model)
    return m.group(0).upper() if m else model.upper()


def resolve_model(model: str) -> str:
    """Strip the routing prefix from a model name when it matches a per-family
    key in .env. E.g. ``openrouter/anthropic/claude-sonnet-4`` has family
    ``OPENROUTER``; if ``ANTHROPIC_API_KEY_OPENROUTER`` exists, the API model
    name is ``anthropic/claude-sonnet-4`` (prefix stripped). Models without a
    matching family key (normal claude-*) pass through unchanged."""
    family = _model_family(model)
    dotenv = _load_dotenv()
    if _resolve(f"ANTHROPIC_API_KEY_{family}", dotenv):
        prefix = family.lower() + "/"
        if model.lower().startswith(prefix):
            return model[len(prefix):]
    return model


def get_sdk_env(index: int = 0, model: str | None = None) -> dict[str, str]:
    """Return the env dict for the *index*-th key (round-robin safe).

    When *model* is given, a model-family key override
    ``ANTHROPIC_API_KEY_<FAMILY>`` (family = first dash-segment of the
    model name, uppercased) replaces the pool key if present — e.g.
    ``gpt-5.5`` → ``ANTHROPIC_API_KEY_GPT``, ``deepseek-v4-pro`` →
    ``ANTHROPIC_API_KEY_DEEPSEEK``. Families without an override (the
    normal case for claude models) use the default pool unchanged, so a
    single .env serves mixed-model experiments against the same base URL.

    Backward compatible: with a single key, ``get_sdk_env()`` returns the
    same dict as before.
    """
    if model:
        dotenv = _load_dotenv()
        family = _model_family(model)
        family_key = _resolve(f"ANTHROPIC_API_KEY_{family}", dotenv)
        if family_key:
            # Upstream output caps differ per family (e.g. minimax rejects
            # max_tokens > 32768 outright). CLAUDE_CODE_MAX_OUTPUT_TOKENS_
            # <FAMILY> in .env overrides the default.
            max_out = _resolve(
                f"CLAUDE_CODE_MAX_OUTPUT_TOKENS_{family}", dotenv
            ) or "128000"
            env = {
                "ANTHROPIC_API_KEY": family_key,
                "CLAUDE_CODE_MAX_OUTPUT_TOKENS": str(max_out),
            }
            # Per-family base URL (e.g. ANTHROPIC_BASE_URL_OPENROUTER for
            # routing specific model families to a different provider).
            base_url = (_resolve(f"ANTHROPIC_BASE_URL_{family}", dotenv)
                        or _resolve("ANTHROPIC_BASE_URL", dotenv))
            if base_url:
                env["ANTHROPIC_BASE_URL"] = base_url
            return env

    pool = get_sdk_env_pool()
    if not pool:
        return {}
    return pool[index % len(pool)]
