"""Tests for SDK env construction (model-family API key overrides)."""

import verdict.core.env as env_mod


def _fake_dotenv():
    return {
        "ANTHROPIC_API_KEY": "sk-default",
        "ANTHROPIC_BASE_URL": "https://b.example",
        "ANTHROPIC_API_KEY_GPT": "sk-gpt",
        "ANTHROPIC_API_KEY_DEEPSEEK": "sk-ds",
    }


def test_model_family_extraction():
    assert env_mod._model_family("gpt-5.5") == "GPT"
    assert env_mod._model_family("deepseek-v4-pro") == "DEEPSEEK"
    assert env_mod._model_family("claude-opus-4-8") == "CLAUDE"
    # Versioned prefixes collapse to the family (valid env-var names).
    assert env_mod._model_family("qwen3.7-max") == "QWEN"
    assert env_mod._model_family("qwen3-coder-next") == "QWEN"
    assert env_mod._model_family("glm-5") == "GLM"
    assert env_mod._model_family("minimax-m2.5") == "MINIMAX"
    assert env_mod._model_family("kimi-k2.5") == "KIMI"


def test_family_key_override(monkeypatch):
    monkeypatch.setattr(env_mod, "_load_dotenv", _fake_dotenv)
    gpt = env_mod.get_sdk_env(0, model="gpt-5.5")
    assert gpt["ANTHROPIC_API_KEY"] == "sk-gpt"
    assert gpt["ANTHROPIC_BASE_URL"] == "https://b.example"
    ds = env_mod.get_sdk_env(0, model="deepseek-v4-pro")
    assert ds["ANTHROPIC_API_KEY"] == "sk-ds"


def test_no_override_falls_back_to_pool(monkeypatch):
    monkeypatch.setattr(env_mod, "_load_dotenv", _fake_dotenv)
    # claude has no ANTHROPIC_API_KEY_CLAUDE entry → default pool key.
    claude = env_mod.get_sdk_env(0, model="claude-opus-4-8")
    assert claude["ANTHROPIC_API_KEY"] == "sk-default"
    # No model at all → unchanged behavior.
    assert env_mod.get_sdk_env(0)["ANTHROPIC_API_KEY"] == "sk-default"


def test_family_max_output_override(monkeypatch):
    def fake():
        d = _fake_dotenv()
        d["ANTHROPIC_API_KEY_MINIMAX"] = "sk-mm"
        d["CLAUDE_CODE_MAX_OUTPUT_TOKENS_MINIMAX"] = "32768"
        return d
    monkeypatch.setattr(env_mod, "_load_dotenv", fake)
    mm = env_mod.get_sdk_env(0, model="minimax-m2.5")
    assert mm["CLAUDE_CODE_MAX_OUTPUT_TOKENS"] == "32768"
    gpt = env_mod.get_sdk_env(0, model="gpt-5.5")
    assert gpt["CLAUDE_CODE_MAX_OUTPUT_TOKENS"] == "128000"
