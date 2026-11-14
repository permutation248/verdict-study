"""Tests for the build_spec_deps agent tool: gating, sufficiency-check
delegation, and persistence of the declared SpecDep onto the working draft.

(Migrated out of the deleted test_supp_ablation.py, which tested the
superseded AUTO-ablation path; build_spec_deps delegates to
_spec_dep_check.verify_spec_dep, patched at its definition site per CLAUDE.md.)
"""

import logging
from unittest.mock import AsyncMock, patch

import pytest

from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.core.project_context import ProjectContext
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.tools.verify.spec_deps import build_spec_deps

_LOG = logging.getLogger("test_build_spec_deps")


def _ctx_with_published_callee() -> ToolContext:
    """Build a ToolContext where callee 'helper' is already published."""
    info = FunctionInfo(
        name="foo",
        source_file="t.c",
        source_code="int foo(){return helper();}",
        callees=["helper"],
    )
    helper_info = FunctionInfo(
        name="helper", source_file="t.c", source_code="int helper(){return 1;}",
        callees=[], callers=["foo"],
    )
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_foo", function_info=info))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=helper_info))
    # Seed foo's working draft.
    entry = reg.get("agent_foo")
    entry.spec = FunctionSpec(
        function_name="foo",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.DRAFT,
    )
    # Publish helper so read_spec("helper") returns a spec.
    helper_entry = reg.get("agent_helper")
    helper_entry.published_spec = FunctionSpec(
        function_name="helper",
        postconditions=["ensures \\result >= 0;"],
        assigns=["assigns \\nothing;"],
        status=SpecStatus.PUBLISHED, version=1,
    )
    bus = AsyncQueueBus()
    pc = ProjectContext(
        source_file="t.c",
        source_dir="/tmp",
        call_graph_dict={},
        function_declarations_map={},
        include_paths=[],
        work_dir="/tmp",
        rte_vos_by_function={"foo": ["rte_1"]},
    )
    from verdict.core.vo_tracker import VOTracker
    tracker = VOTracker()
    tracker.mark_spec_verified()
    return ToolContext(
        agent_id="agent_foo",
        function_info=info,
        registry=reg,
        message_bus=bus,
        logger=_LOG,
        project_context=pc,
        vo_tracker=tracker,
    )


@pytest.mark.asyncio
async def test_build_verified_records_clauses():
    """Sufficiency PASS → SpecDep with declared clauses."""
    from verdict.tools._spec_dep_check import SpecDepResult
    ctx = _ctx_with_published_callee()
    spy = AsyncMock(return_value=SpecDepResult(verified=True, proved=5, total=5))
    with patch("verdict.tools._spec_dep_check.verify_spec_dep", spy):
        result = await build_spec_deps(
            ctx,
            {"helper": ["ensures \\result >= 0;"]},
        )
    assert result["helper"]["status"] == "verified"
    spec = ctx.registry.read_working_spec("foo")
    dep = spec.spec_deps["helper"]
    assert dep.depends is True
    assert dep.clauses == frozenset(["ensures \\result >= 0;"])


@pytest.mark.asyncio
async def test_build_rejected_falls_back_to_all():
    """Sufficiency FAIL → SpecDep ALL."""
    from verdict.tools._spec_dep_check import SpecDepResult
    ctx = _ctx_with_published_callee()
    spy = AsyncMock(return_value=SpecDepResult(verified=False, proved=3, total=5, failed_goals=["Post-condition 'postcondition_0'"]))
    with patch("verdict.tools._spec_dep_check.verify_spec_dep", spy):
        result = await build_spec_deps(
            ctx,
            {"helper": ["ensures \\result >= 0;"]},
        )
    assert result["helper"]["status"] == "rejected"
    assert result["helper"]["proved_goals"] == 3
    assert len(result["helper"]["failed_goals"]) > 0
    spec = ctx.registry.read_working_spec("foo")
    dep = spec.spec_deps["helper"]
    assert dep.depends is True
    assert dep.clauses is None


@pytest.mark.asyncio
async def test_build_rejects_missing_clause():
    """Declared clause not in published spec → rejected without WP."""
    ctx = _ctx_with_published_callee()
    result = await build_spec_deps(
        ctx,
        {"helper": ["ensures \\result == 42;"]},
    )
    assert result["helper"]["success"] is False
    assert result["helper"]["status"] == "error"
    assert "not in published spec" in result["helper"]["reason"]


def test_parse_failed_goals_filters_libc_noise():
    """Report-wide libc contract clauses must not bury the caller's goals."""
    from verdict.tools._spec_dep_check import _parse_failed_goals

    wp_output = """\
[wp] Proved goals:   181 / 189
[report] Computing properties status...
[    -    ] result_uchar_of_eof: result_uchar_of_eof
[    -    ] correct_div: correct_div
[    -    ] Assertion 'rte_41,signed_overflow' (file /tmp/x/.verdict_agent_agent_f.c, line 5)
[    -    ] Assertion 'rte_41,signed_overflow' (file /tmp/x/.verdict_agent_agent_f.c, line 5)
[    -    ] Default behavior (file /tmp/x/.verdict_agent_agent_f.c, line 9)
[  Valid  ] Assertion 'rte_40,shift' (file /tmp/x/.verdict_agent_agent_f.c, line 4)
"""
    got = _parse_failed_goals(wp_output, ".verdict_agent_agent_f.c")
    assert got == [
        "Assertion 'rte_41,signed_overflow' "
        "(file /tmp/x/.verdict_agent_agent_f.c, line 5)"
    ]
    # Without a basename the legacy behavior (everything) still works.
    assert len(_parse_failed_goals(wp_output)) == 3
