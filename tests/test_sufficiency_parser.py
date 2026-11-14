"""Tests for parse_sufficiency_failures — the dedicated sufficiency-check parser."""

from verdict.tools._spec_dep_check import (
    SufficiencyFailure,
    parse_sufficiency_failures,
)


def _build_wp_output(goal_lines: list[str], status_lines: list[str]) -> str:
    goals = "\n".join(goal_lines)
    statuses = "\n".join(status_lines)
    return f"{goals}\n[report] Computing properties status...\n{statuses}\n"


def test_spec_failure_resolved_to_clause():
    wp = _build_wp_output([], [
        "[  -  ] Post-condition 'postcondition_0' in 'my_func'",
        "[ Valid ] Post-condition 'postcondition_1' in 'my_func'",
        "[  -  ] Assigns 'assign_0' in 'my_func'",
    ])
    annotations = {
        "postconditions": ["ensures \\result >= 0;", "ensures \\result <= 100;"],
        "assigns": ["assigns \\nothing;"],
    }
    result = parse_sufficiency_failures(wp, annotations, [], {})
    spec_failures = [f for f in result if f.kind == "spec"]
    assert len(spec_failures) == 2
    assert spec_failures[0].label == "postcondition_0"
    assert spec_failures[0].clause == "ensures \\result >= 0;"
    assert spec_failures[1].label == "assign_0"
    assert spec_failures[1].clause == "assigns \\nothing;"


def test_rte_failure():
    wp = _build_wp_output([], [
        "[ Unknown ] Assertion 'rte_3' (file /tmp/f.c, line 10)",
        "[ Valid ] Assertion 'rte_4' (file /tmp/f.c, line 12)",
    ])
    result = parse_sufficiency_failures(wp, {}, ["rte_3", "rte_4"], {})
    rte_failures = [f for f in result if f.kind == "rte"]
    assert len(rte_failures) == 1
    assert rte_failures[0].label == "rte_3"


def test_callsite_failure_identifies_callee_and_precondition():
    goals = [
        "Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' "
        "in 'main' at call 'helper' (file /tmp/m.c, line 10):",
        "  Assume ...",
        "Goal Instance of 'Pre-condition 'precondition_test' in 'helper'' "
        "in 'main' at call 'helper' (file /tmp/m.c, line 10):",
        "  Assume ...",
    ]
    statuses = [
        "[ Valid ] Instance of 'Pre-condition 'precondition_test'' "
        "at call 'helper' (file /tmp/m.c, line 10)",
        "[ Unknown ] Instance of 'Pre-condition 'precondition_test'' "
        "at call 'helper' (file /tmp/m.c, line 10)",
    ]
    wp = _build_wp_output(goals, statuses)
    callee_pres = {"helper": ["requires x > 0;", "requires \\valid(p);"]}
    result = parse_sufficiency_failures(wp, {}, [], callee_pres)
    cs_failures = [f for f in result if f.kind == "callsite"]
    assert len(cs_failures) == 1
    assert cs_failures[0].callee == "helper"
    assert cs_failures[0].precondition_index == 1
    assert cs_failures[0].clause == "requires \\valid(p);"
    assert cs_failures[0].call_site_line == 10


def test_multi_callee_callsite_failures():
    goals = [
        "Goal Instance of 'Pre-condition 'precondition_test' in 'alpha'' "
        "in 'caller' at call 'alpha' (file /tmp/c.c, line 5):",
        "Goal Instance of 'Pre-condition 'precondition_test' in 'beta'' "
        "in 'caller' at call 'beta' (file /tmp/c.c, line 8):",
    ]
    statuses = [
        "[ Unknown ] Instance of 'Pre-condition 'precondition_test'' "
        "at call 'alpha' (file /tmp/c.c, line 5)",
        "[ Unknown ] Instance of 'Pre-condition 'precondition_test'' "
        "at call 'beta' (file /tmp/c.c, line 8)",
    ]
    wp = _build_wp_output(goals, statuses)
    callee_pres = {
        "alpha": ["requires a > 0;"],
        "beta": ["requires b != NULL;"],
    }
    result = parse_sufficiency_failures(wp, {}, [], callee_pres)
    cs_failures = [f for f in result if f.kind == "callsite"]
    assert len(cs_failures) == 2
    assert cs_failures[0].callee == "alpha"
    assert cs_failures[0].clause == "requires a > 0;"
    assert cs_failures[1].callee == "beta"
    assert cs_failures[1].clause == "requires b != NULL;"


def test_mixed_failures_all_three_types():
    goals = [
        "Goal Instance of 'Pre-condition 'precondition_test' in 'h'' "
        "in 'f' at call 'h' (file /tmp/f.c, line 20):",
    ]
    statuses = [
        "[  -  ] Post-condition 'postcondition_0' in 'f'",
        "[ Unknown ] Assertion 'rte_1' (file /tmp/f.c, line 7)",
        "[ Unknown ] Instance of 'Pre-condition 'precondition_test'' "
        "at call 'h' (file /tmp/f.c, line 20)",
    ]
    wp = _build_wp_output(goals, statuses)
    annotations = {"postconditions": ["ensures \\result == 0;"]}
    callee_pres = {"h": ["requires len > 0;"]}
    result = parse_sufficiency_failures(
        wp, annotations, ["rte_1"], callee_pres,
    )
    kinds = [f.kind for f in result]
    assert kinds == ["spec", "rte", "callsite"]
    assert result[0].clause == "ensures \\result == 0;"
    assert result[1].label == "rte_1"
    assert result[2].callee == "h"
    assert result[2].clause == "requires len > 0;"


def test_no_report_section_returns_empty():
    wp = "[wp] Proved goals: 3 / 5\n"
    result = parse_sufficiency_failures(wp, {}, ["rte_1"], {"h": ["requires x;"]})
    assert result == []


def test_all_valid_returns_empty():
    wp = _build_wp_output([], [
        "[ Valid ] Post-condition 'postcondition_0' in 'f'",
        "[ Valid ] Assertion 'rte_1' (file /tmp/f.c, line 5)",
    ])
    result = parse_sufficiency_failures(
        wp, {"postconditions": ["ensures 1;"]}, ["rte_1"], {},
    )
    assert result == []
