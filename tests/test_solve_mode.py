"""Tests for solve-mode scope selection helpers."""

from io import StringIO
from unittest.mock import patch

import pytest

from verdict.core.types import (
    AgentResult,
    FunctionSpec,
    ProofState,
    VOSummary,
)
from verdict.cli._setup import _resolve_scope_function_names
from verdict.cli.solve_mode import (
    _format_vo_inline,
    _print_batch_progress,
    _print_solve_summary,
)


def test_global_scope_returns_all_available_names_in_order():
    names = _resolve_scope_function_names(
        available_names=["a", "b", "c"],
        scope="global",
        requested_names=[],
    )
    assert names == ["a", "b", "c"]


def test_function_scope_requires_explicit_selection():
    with pytest.raises(ValueError, match="at least one --function"):
        _resolve_scope_function_names(
            available_names=["a", "b"],
            scope="functions",
            requested_names=[],
        )


def test_function_scope_validates_unknown_names():
    with pytest.raises(ValueError, match="Unknown functions"):
        _resolve_scope_function_names(
            available_names=["a", "b"],
            scope="functions",
            requested_names=["a", "missing"],
        )


def test_function_scope_deduplicates_requested_names_preserving_order():
    names = _resolve_scope_function_names(
        available_names=["a", "b", "c"],
        scope="functions",
        requested_names=["b", "a", "b"],
    )
    assert names == ["b", "a"]


def test_function_scope_expands_transitive_callees():
    call_graph = {"main": ["helper"], "helper": ["leaf"], "leaf": []}
    names = _resolve_scope_function_names(
        available_names=["main", "helper", "leaf"],
        scope="functions",
        requested_names=["main"],
        call_graph=call_graph,
    )
    assert names == ["main", "helper", "leaf"]


def test_function_scope_skips_callees_not_in_available():
    call_graph = {"main": ["external_lib_fn"]}
    names = _resolve_scope_function_names(
        available_names=["main"],
        scope="functions",
        requested_names=["main"],
        call_graph=call_graph,
    )
    assert names == ["main"]


def test_function_scope_without_call_graph_returns_only_requested():
    names = _resolve_scope_function_names(
        available_names=["a", "b", "c"],
        scope="functions",
        requested_names=["a"],
    )
    assert names == ["a"]


def _make_result(
    name: str,
    *,
    cycle_count: int = 1,
    cycle_limit_reached: bool = False,
    proof_state: ProofState = ProofState.LOCAL_OK,
    vo_summary: VOSummary | None = None,
) -> AgentResult:
    return AgentResult(
        agent_id=f"agent_{name}",
        function_name=name,
        proof_state=proof_state,
        spec=FunctionSpec(function_name=name),
        wake_reasons=[],
        cycle_count=cycle_count,
        cycle_limit_reached=cycle_limit_reached,
        vo_summary=vo_summary,
    )


def test_format_vo_inline_no_vos():
    assert _format_vo_inline(None) == "no VOs"
    assert _format_vo_inline(VOSummary()) == "no VOs"


def test_format_vo_inline_complete_with_bugs():
    s = VOSummary(rte_verified=2, rte_bug=1, callsite_verified=1)
    out = _format_vo_inline(s)
    assert "rte 2/3" in out
    assert "cs 1/1" in out
    assert "1 bug" in out
    assert "✓" in out


def test_format_vo_inline_partial_with_failure():
    s = VOSummary(
        rte_verified=1,
        rte_pending=1,
        callsite_verified=1,
        callsite_failed=1,
    )
    out = _format_vo_inline(s)
    assert "rte 1/2" in out
    assert "cs 1/2" in out
    assert "1 failed" in out
    assert "✓" not in out


def test_print_batch_progress_outputs_cycle_and_vo():
    result = _make_result(
        "main",
        cycle_count=3,
        vo_summary=VOSummary(rte_verified=1, rte_pending=1),
    )
    buf = StringIO()
    with patch("sys.stdout", buf):
        _print_batch_progress("reactivation", [result])
    out = buf.getvalue()
    assert "reactivation" in out
    assert "main" in out
    assert "cycle=3" in out
    assert "rte 1/2" in out


def test_print_batch_progress_empty_noop():
    buf = StringIO()
    with patch("sys.stdout", buf):
        _print_batch_progress("initial", [])
    assert buf.getvalue() == ""


def test_solve_summary_lists_failed_and_bug_vo_ids():
    summary = VOSummary(
        rte_verified=1,
        rte_failed=1,
        callsite_bug=1,
        failed_ids=["rte_3"],
        bugs={"cs_2": "intentional UB sink"},
    )
    result = _make_result(
        "main",
        cycle_count=5,
        vo_summary=summary,
    )
    buf = StringIO()
    with patch("sys.stdout", buf):
        _print_solve_summary([result], scope="global")
    out = buf.getvalue()
    assert "cycles=5" in out
    assert "failed VOs: rte_3" in out
    assert "bug cs_2: intentional UB sink" in out


def test_solve_summary_flags_cycle_limited_agents():
    result = _make_result(
        "loopy",
        cycle_count=25,
        cycle_limit_reached=True,
        vo_summary=VOSummary(rte_verified=1),
    )
    buf = StringIO()
    with patch("sys.stdout", buf):
        _print_solve_summary([result], scope="global")
    out = buf.getvalue()
    assert "[cycle limit]" in out
    assert "Cycle limit reached for: loopy" in out
