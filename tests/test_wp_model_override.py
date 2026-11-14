"""The -wp-model override (evaluation-semantics ablation).

Default: no -wp-model flag → Frama-C's sound Typed model. When
configured (e.g. SpecSyn's Typed+var+int+real), every WP command
carries the flag; the RTE-assertion set is untouched (preprocessing is
model-independent), so discharge rates stay denominator-comparable.
"""

from __future__ import annotations

from verdict.framac import wp_resources
from verdict.tools._wp_runner import _build_wp_command


def teardown_function():
    wp_resources.configure(8, 1, model=None)


def test_default_has_no_model_flag():
    wp_resources.configure(8, 1, model=None)
    cmd = _build_wp_command(timeout=10)
    assert "-wp-model" not in cmd


def test_configured_model_lands_in_command():
    wp_resources.configure(8, 1, model="Typed+var+int+real")
    cmd = _build_wp_command(timeout=10, wp_prop_targets=["rte_1"])
    i = cmd.index("-wp-model")
    assert cmd[i + 1] == "Typed+var+int+real"
    # Core soundness-relevant flags unchanged by the override.
    assert "-wp-no-init-const" in cmd
    assert "rte_1" in cmd[cmd.index("-wp-prop") + 1]


def test_empty_model_treated_as_none():
    wp_resources.configure(8, 1, model="")
    assert wp_resources.model() is None
    assert "-wp-model" not in _build_wp_command(timeout=10)
