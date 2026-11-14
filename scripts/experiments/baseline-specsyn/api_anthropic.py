# Drop-in replacement for the SpecSyn artifact's api.py (the ONLY file
# patched in our ported copy — the artifact itself is never modified).
#
# Same public interface as the original (gpt_request_wrapper consumed by
# algo.py), same env-var configuration pattern, same failure semantics
# (an exhausted retry returns an empty assistant message, exactly like
# the original's swallowed-exception path). Differences, documented in
# README.md §3/§4:
#   - anthropic /v1/messages instead of the OpenAI client (no OpenAI
#     channel available to us)
#   - model from SPECSYN_MODEL (default claude-sonnet-4-6 = the same
#     model as our Verdict runs) instead of hardcoded "gpt-5"
#   - bounded retries with backoff instead of a single swallowed try
#
# Deploy: copy over <ported-pipeline>/api.py before running main.py.

import os
import time

import anthropic

from utils import my_logging_context

MODEL = os.environ.get("SPECSYN_MODEL", "claude-sonnet-4-6")
MAX_TOKENS = int(os.environ.get("SPECSYN_MAX_TOKENS", "8192"))

_client = anthropic.Anthropic()  # reads ANTHROPIC_API_KEY / ANTHROPIC_BASE_URL


def gpt_request_wrapper(messages: list, temperature: float = 0):
    my_logging_context(messages, True)
    system_text = ""
    converted = []
    for msg in messages:
        if msg["role"] == "system":
            system_text = msg["content"]
        else:
            converted.append({"role": msg["role"], "content": msg["content"]})
    res = {"role": "assistant", "content": ""}
    for attempt in range(3):
        try:
            resp = _client.messages.create(
                model=MODEL,
                max_tokens=MAX_TOKENS,
                system=system_text,
                messages=converted,
                # temperature deliberately not passed — provider default
                # (1.0), matching the artifact's commented-out setting.
            )
            res = {
                "role": "assistant",
                "content": "".join(
                    b.text for b in resp.content if b.type == "text"
                ),
            }
            break
        except Exception:
            time.sleep(5 * (attempt + 1))
    my_logging_context([res], False)
    return res
