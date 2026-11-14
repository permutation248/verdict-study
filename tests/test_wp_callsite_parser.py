"""Tests for parse_wp_callsite_result — per-(call_site_line, precondition_index)
parser that matches the reference implementation's 4-level indexing."""

from __future__ import annotations

import pytest

from verdict.tools._wp_runner import parse_wp_callsite_result


# ---------------------------------------------------------------------------
# Realistic fragments of Frama-C WP output
# ---------------------------------------------------------------------------


# 1 call site to abs_value, 2 preconditions, both valid.
_WP_STRUCTURED_ALL_PASS = """\
[kernel] Parsing /tmp/clamp.c ...
[wp] 2 goals scheduled
[wp] Proved goals:    2 / 2
  Qed:             2
[wp:print-generated]
Goal Instance of 'Pre-condition 'precondition_test' in 'abs_value'' in 'clamp_positive' at call 'abs_value' (file /tmp/clamp.c, line 14):
Prove: ...
------------------------------------------------------------
Goal Instance of 'Pre-condition 'precondition_test' in 'abs_value'' in 'clamp_positive' at call 'abs_value' (file /tmp/clamp.c, line 14):
Prove: ...
------------------------------------------------------------
[report] Computing properties status...
--- Properties of Function 'clamp_positive' ---
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'abs_value' (file /tmp/clamp.c, line 14)
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'abs_value' (file /tmp/clamp.c, line 14)
"""


# 1 call site, 2 preconditions, first valid, second unknown.
_WP_STRUCTURED_SECOND_FAILS = """\
[kernel] Parsing /tmp/clamp.c ...
[wp] 2 goals scheduled
[wp] Proved goals:    1 / 2
  Qed:             1
  Unknown:         1
[wp:print-generated]
Goal Instance of 'Pre-condition 'precondition_test' in 'abs_value'' in 'clamp_positive' at call 'abs_value' (file /tmp/clamp.c, line 14):
Prove: ...
------------------------------------------------------------
Goal Instance of 'Pre-condition 'precondition_test' in 'abs_value'' in 'clamp_positive' at call 'abs_value' (file /tmp/clamp.c, line 14):
Prove: ...
------------------------------------------------------------
[report] Computing properties status...
--- Properties of Function 'clamp_positive' ---
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'abs_value' (file /tmp/clamp.c, line 14)
[ Unknown ] Instance of 'Pre-condition 'precondition_test'' at call 'abs_value' (file /tmp/clamp.c, line 14)
"""


# 2 call sites (line 10 and 15), 2 preconditions each, all valid at
# the first site but second precondition fails at the second site.
_WP_STRUCTURED_MULTI_SITE_MIXED = """\
[kernel] Parsing /tmp/m.c ...
[wp] 4 goals scheduled
[wp] Proved goals:    3 / 4
  Qed:             3
  Unknown:         1
[wp:print-generated]
Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' in 'main' at call 'helper' (file /tmp/m.c, line 10):
Prove: ...
------------------------------------------------------------
Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' in 'main' at call 'helper' (file /tmp/m.c, line 10):
Prove: ...
------------------------------------------------------------
Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' in 'main' at call 'helper' (file /tmp/m.c, line 15):
Prove: ...
------------------------------------------------------------
Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' in 'main' at call 'helper' (file /tmp/m.c, line 15):
Prove: ...
------------------------------------------------------------
[report] Computing properties status...
--- Properties of Function 'main' ---
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'helper' (file /tmp/m.c, line 10)
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'helper' (file /tmp/m.c, line 10)
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'helper' (file /tmp/m.c, line 15)
[ Unknown ] Instance of 'Pre-condition 'precondition_test'' at call 'helper' (file /tmp/m.c, line 15)
"""


_WP_AGGREGATE_ONLY = """\
[kernel] Parsing ...
[wp] Proved goals:    3 / 3
  Qed:             3
"""


_WP_NO_GOALS = """\
[kernel] Parsing ...
[wp] No goals generated.
"""

_WP_EMPTY = ""


# ---------------------------------------------------------------------------
# Structured: single call site, both valid
# ---------------------------------------------------------------------------


def test_structured_all_pass():
    pre = ["requires x > 0;", "requires y >= 0;"]
    results = parse_wp_callsite_result(
        _WP_STRUCTURED_ALL_PASS, expected_preconditions=pre
    )
    # 1 site × 2 preconditions = 2 results
    assert len(results) == 2
    assert all(r["verified"] for r in results)
    assert all(r["call_site_line"] == 14 for r in results)
    assert results[0]["precondition_index"] == 0
    assert results[0]["precondition"] == "requires x > 0;"
    assert results[1]["precondition_index"] == 1
    assert results[1]["precondition"] == "requires y >= 0;"


# ---------------------------------------------------------------------------
# Structured: one passes, the other fails (same call site)
# ---------------------------------------------------------------------------


def test_structured_second_fails():
    pre = ["requires x > 0;", "requires y >= 0;"]
    results = parse_wp_callsite_result(
        _WP_STRUCTURED_SECOND_FAILS, expected_preconditions=pre
    )
    assert len(results) == 2
    assert results[0]["verified"] is True
    assert results[1]["verified"] is False
    assert results[0]["call_site_line"] == 14
    assert results[1]["call_site_line"] == 14


# ---------------------------------------------------------------------------
# Structured: multiple call sites, per-(site, precondition) results
# ---------------------------------------------------------------------------


def test_structured_multi_site_per_pair():
    """2 sites × 2 preconditions = 4 results with individual verdicts."""
    pre = ["requires len > 0;", "requires buf != \\null;"]
    results = parse_wp_callsite_result(
        _WP_STRUCTURED_MULTI_SITE_MIXED, expected_preconditions=pre
    )
    # 2 sites × 2 preconditions = 4
    assert len(results) == 4

    # Site line 10: both pass
    site10 = [r for r in results if r["call_site_line"] == 10]
    assert len(site10) == 2
    assert all(r["verified"] for r in site10)
    assert site10[0]["precondition_index"] == 0
    assert site10[1]["precondition_index"] == 1

    # Site line 15: first passes, second fails
    site15 = [r for r in results if r["call_site_line"] == 15]
    assert len(site15) == 2
    assert site15[0]["verified"] is True   # precondition 0
    assert site15[1]["verified"] is False  # precondition 1
    assert site15[0]["precondition_index"] == 0
    assert site15[1]["precondition_index"] == 1


# ---------------------------------------------------------------------------
# Fallback: aggregate `[wp] Proved goals: N / M`
# ---------------------------------------------------------------------------


def test_aggregate_fallback_with_preconditions():
    pre = ["requires a;", "requires b;", "requires c;"]
    results = parse_wp_callsite_result(_WP_AGGREGATE_ONLY, expected_preconditions=pre)
    assert len(results) == 3
    assert all(r["verified"] for r in results)
    assert all(r["call_site_line"] == 0 for r in results)
    for i, r in enumerate(results):
        assert r["precondition_index"] == i
    assert "3/3" in results[0]["obligation"]


def test_aggregate_fallback_without_expected_preconditions():
    results = parse_wp_callsite_result(_WP_AGGREGATE_ONLY)
    assert len(results) == 1
    assert results[0]["precondition"] == "(aggregate)"
    assert results[0]["verified"] is True
    assert results[0]["call_site_line"] == 0


# ---------------------------------------------------------------------------
# No goals / empty
# ---------------------------------------------------------------------------


def test_no_goals_returns_empty():
    results = parse_wp_callsite_result(
        _WP_NO_GOALS, expected_preconditions=["requires x > 0;"]
    )
    assert results == []


def test_empty_output_returns_empty():
    results = parse_wp_callsite_result(
        _WP_EMPTY, expected_preconditions=["requires x > 0;"]
    )
    assert results == []


# ---------------------------------------------------------------------------
# Regression: single labeled precondition
# ---------------------------------------------------------------------------


def test_regression_single_labeled_precondition():
    wp_output = """\
[kernel] Parsing /tmp/clamp.c ...
[wp] 1 goal scheduled
[wp] Proved goals:    1 / 1
  Qed:             1
[wp:print-generated]
Goal Instance of 'Pre-condition 'precondition_test' in 'abs_value'' in 'clamp_positive' at call 'abs_value' (file /tmp/clamp.c, line 14):
Prove: ...
------------------------------------------------------------
[report] Computing properties status...
--- Properties of Function 'clamp_positive' ---
[  Valid  ] Instance of 'Pre-condition 'precondition_test'' at call 'abs_value' (file /tmp/clamp.c, line 14)
"""
    results = parse_wp_callsite_result(
        wp_output, expected_preconditions=["requires x > -2147483648;"]
    )
    assert len(results) == 1
    assert results[0]["verified"] is True
    assert results[0]["precondition"] == "requires x > -2147483648;"
    assert results[0]["call_site_line"] == 14
    assert results[0]["precondition_index"] == 0
