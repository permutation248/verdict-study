"""Tests for the publish-time VO gate in update_my_spec."""

import logging

import pytest

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.core.vo_tracker import VOEntry, VOTracker
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.tools.runtime.spec_io import PublishBlocked, update_my_spec


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
        source_file="main.c",
        source_code="int main() { return callee(1); }",
        callees=["callee"],
        callers=[],
    )
    callee_info = FunctionInfo(
        name="callee",
        source_file="callee.c",
        source_code="int callee(int x) { return x; }",
        callees=[],
        callers=["main"],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=main_info))
    reg.register(AgentEntry(agent_id="agent_callee", function_info=callee_info))
    return reg, main_info, callee_info


@pytest.mark.asyncio
async def test_publish_blocked_by_pending_rte(main_and_callee):
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="test rte")
    )
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(PublishBlocked, match="rte_1"):
        await update_my_spec(
            ctx,
            FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        )


@pytest.mark.asyncio
async def test_publish_allowed_when_rte_verified(main_and_callee):
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="test rte")
    )
    tracker.mark_verified("rte_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    # Should not raise
    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


@pytest.mark.asyncio
async def test_publish_allowed_when_rte_bug(main_and_callee):
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="test rte")
    )
    tracker.mark_bug("rte_1", "intentional UB for test")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


@pytest.mark.asyncio
async def test_publish_blocked_by_pending_callsite_for_published_callee(
    main_and_callee,
):
    reg, main_info, _ = main_and_callee
    # callee already published
    await reg.write_spec(
        "agent_callee",
        FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
    )

    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(PublishBlocked, match="cs_0"):
        await update_my_spec(
            ctx,
            FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        )


@pytest.mark.asyncio
async def test_publish_allowed_with_unpublished_callee(main_and_callee):
    reg, main_info, _ = main_and_callee
    # callee NOT published yet
    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    # Should allow publish (call-site VO for unpublished callee is deferrable)
    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


@pytest.mark.asyncio
async def test_draft_bypasses_publish_gate(main_and_callee):
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="test rte")
    )
    # RTE still PENDING
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    await update_my_spec(
        ctx,
        FunctionSpec(
            function_name="main",
            preconditions=["requires x > 0;"],
            status=SpecStatus.DRAFT,
        ),
    )
    # Draft allowed even with pending VOs
    assert reg.read_working_spec("main").preconditions == ["requires x > 0;"]


@pytest.mark.asyncio
async def test_empty_tracker_does_not_block_publish(main_and_callee):
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()  # no VOs at all
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


@pytest.mark.asyncio
async def test_publish_blocked_by_failed_rte(main_and_callee):
    """FAILED RTE must block publish — agent has to fix, weaken, or mark bug."""
    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="overflow check")
    )
    tracker.mark_failed("rte_1")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(PublishBlocked, match="failed"):
        await update_my_spec(
            ctx,
            FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        )


@pytest.mark.asyncio
async def test_publish_blocked_by_failed_callsite(main_and_callee):
    """FAILED call-site VO must block publish, even if callee is published."""
    reg, main_info, _ = main_and_callee
    await reg.write_spec(
        "agent_callee",
        FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
    )

    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    tracker.mark_failed("cs_0")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    with pytest.raises(PublishBlocked, match="cs_0"):
        await update_my_spec(
            ctx,
            FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        )


@pytest.mark.asyncio
async def test_publish_allowed_when_failed_vo_marked_bug(main_and_callee):
    """BUG is the escape hatch for FAILED VOs."""
    reg, main_info, _ = main_and_callee
    await reg.write_spec(
        "agent_callee",
        FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
    )

    tracker = VOTracker()
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    tracker.mark_failed("cs_0")
    tracker.mark_bug("cs_0", "precondition mismatch accepted")
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    # Should succeed — BUG overrides FAILED.
    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


@pytest.mark.asyncio
async def test_no_tracker_does_not_block_publish(main_and_callee):
    reg, main_info, _ = main_and_callee
    bus = AsyncQueueBus()
    ctx = ToolContext(
        agent_id="agent_main",
        function_info=main_info,
        registry=reg,
        message_bus=bus,
        logger=logging.getLogger("test"),
        vo_tracker=None,
    )

    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None


# ── Manual drafts cannot publish unverified annotations ──


@pytest.mark.asyncio
async def test_manual_draft_annotation_change_invalidates_spec_vo(main_and_callee):
    """A manual draft that changes the contract is unverified by definition:
    the spec VO goes PENDING and the publish gate blocks until a
    verify_function_contract pass re-establishes it."""
    from verdict.core.vo_tracker import VOStatus

    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    # Simulate verify_function_contract's verified draft.
    await reg.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result == 0;"],
            status=SpecStatus.DRAFT,
        ),
    )
    tracker.mark_spec_verified()

    # Manual draft swaps in a different (never verified) postcondition.
    result = await update_my_spec(
        ctx,
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.DRAFT,
        ),
    )
    assert result["success"] is True
    assert tracker.spec_vo.status == VOStatus.PENDING

    with pytest.raises(PublishBlocked):
        await update_my_spec(
            ctx,
            FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        )


@pytest.mark.asyncio
async def test_manual_draft_identical_annotations_keep_spec_vo(main_and_callee):
    """Re-saving the same annotations is not a contract change — the spec VO
    stays VERIFIED and publish remains possible."""
    from verdict.core.vo_tracker import VOStatus

    reg, main_info, _ = main_and_callee
    tracker = VOTracker()
    ctx = _make_ctx("agent_main", main_info, reg, tracker)

    await reg.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result == 0;"],
            status=SpecStatus.DRAFT,
        ),
    )
    tracker.mark_spec_verified()

    await update_my_spec(
        ctx,
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result == 0;"],
            status=SpecStatus.DRAFT,
        ),
    )
    assert tracker.spec_vo.status == VOStatus.VERIFIED

    await update_my_spec(
        ctx,
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert reg.read_spec("main") is not None
