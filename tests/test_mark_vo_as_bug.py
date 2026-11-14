"""Tests for the tool-level mark_vo_as_bug guards."""

from __future__ import annotations

import logging

import pytest

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.tools.runtime.vo_actions import mark_vo_as_bug


def _make_ctx(
    agent_id: str,
    function_info: FunctionInfo,
    registry: AgentRegistry,
    tracker: VOTracker,
) -> ToolContext:
    bus = AsyncQueueBus()
    return ToolContext(
        agent_id=agent_id,
        function_info=function_info,
        registry=registry,
        message_bus=bus,
        logger=logging.getLogger("test"),
        vo_tracker=tracker,
    )


@pytest.fixture
def main_and_callee():
    reg = AgentRegistry()
    main_info = FunctionInfo(
        name="main",
        source_file="m.c",
        source_code="int main() { return callee(1); }",
        callees=["callee"],
        callers=[],
    )
    callee_info = FunctionInfo(
        name="callee",
        source_file="c.c",
        source_code="int callee(int x) { return x; }",
        callees=[],
        callers=["main"],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=main_info))
    reg.register(AgentEntry(agent_id="agent_callee", function_info=callee_info))
    return reg, main_info


# ---------------------------------------------------------------------------
# Basic guards
# ---------------------------------------------------------------------------


def test_mark_bug_requires_reason(main_and_callee):
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.mark_failed("rte_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(ValueError, match="reason"):
        mark_vo_as_bug(ctx, "rte_1", "")
    with pytest.raises(ValueError, match="reason"):
        mark_vo_as_bug(ctx, "rte_1", "   ")


def test_mark_bug_unknown_vo(main_and_callee):
    reg, main_info = main_and_callee
    tracker = VOTracker()
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(KeyError, match="Unknown VO"):
        mark_vo_as_bug(ctx, "rte_does_not_exist", "reason")


# ---------------------------------------------------------------------------
# Reject: already verified VO
# ---------------------------------------------------------------------------


def test_mark_bug_refuses_verified_vo(main_and_callee):
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.mark_verified("rte_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(ValueError, match="already VERIFIED"):
        mark_vo_as_bug(ctx, "rte_1", "want to mark it anyway")

    # Status is still VERIFIED
    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED


# ---------------------------------------------------------------------------
# Reject: call-site VO with unpublished callee
# ---------------------------------------------------------------------------


def test_mark_bug_refuses_callsite_when_callee_unpublished(main_and_callee):
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_1",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    # callee is registered but has NOT published yet
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(ValueError, match="has not published yet"):
        mark_vo_as_bug(ctx, "cs_1", "waiting for callee to publish")

    # Status must be unchanged — still PENDING
    assert tracker.call_site_vos["cs_1"].status == VOStatus.PENDING


def test_mark_bug_refuses_callsite_when_callee_unknown(main_and_callee):
    """A call-site VO pointing at a function the registry has never heard
    of still counts as 'unpublished' (no published_spec). Reject."""
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_1",
            vo_type="call_site",
            description="call to external lib",
            related_function="external_lib_fn",
        )
    )
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(ValueError, match="has not published"):
        mark_vo_as_bug(ctx, "cs_1", "external library, can't prove")


# ---------------------------------------------------------------------------
# Accept: call-site VO with published callee
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_mark_bug_allows_callsite_when_callee_published(main_and_callee):
    reg, main_info = main_and_callee
    # callee publishes a real spec
    await reg.write_spec(
        "agent_callee",
        FunctionSpec(
            function_name="callee",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_1",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    tracker.mark_failed("cs_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    result = mark_vo_as_bug(
        ctx, "cs_1", "callee's published precondition cannot be enforced here"
    )
    assert result["status"] == "bug"
    assert tracker.call_site_vos["cs_1"].status == VOStatus.BUG
    assert "precondition" in tracker.call_site_vos["cs_1"].bug_reason


# ---------------------------------------------------------------------------
# Accept: failed RTE VO
# ---------------------------------------------------------------------------


def test_mark_bug_allows_failed_rte(main_and_callee):
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="div"))
    tracker.mark_failed("rte_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    result = mark_vo_as_bug(ctx, "rte_1", "division by zero requires unreachable branch")
    assert result["status"] == "bug"
    assert tracker.rte_vos["rte_1"].status == VOStatus.BUG


def test_mark_bug_allows_pending_rte(main_and_callee):
    """PENDING RTE VOs are allowed to be bug-marked — PENDING here does
    not imply deferral, unlike call-site VOs."""
    reg, main_info = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="idx"))
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    result = mark_vo_as_bug(ctx, "rte_1", "known issue in legacy code")
    assert result["status"] == "bug"
    assert tracker.rte_vos["rte_1"].status == VOStatus.BUG


# ── spec VO laundering guard ──


@pytest.mark.asyncio
async def test_spec_vo_cannot_be_marked_bug():
    """Marking the spec contract VO as BUG would publish a never-verified
    contract through the gate (which only blocks PENDING spec VOs)."""
    info = FunctionInfo(name="f", source_file="f.c", source_code="int f(){return 0;}")
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_f", function_info=info))
    tracker = VOTracker()
    tracker.invalidate_spec("stale for test")
    ctx = _make_ctx("agent_f", info, reg, tracker)
    with pytest.raises(ValueError, match="cannot be marked as a bug"):
        mark_vo_as_bug(ctx, vo_id="spec", reason="cannot prove it")
    assert tracker.spec_vo.status == VOStatus.PENDING
