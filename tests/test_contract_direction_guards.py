"""Guards for the contract-direction asymmetry:
- caller can only hypothesize callee post/assigns (not pre / loop_contracts)
- submit_proof_demand can only request post/assigns
- verify_callsite_preconditions soft-skips unpublished callees
"""

from __future__ import annotations

import logging

import pytest

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.types import FunctionInfo, FunctionSpec
from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.tools.runtime.proof_demand import submit_proof_demand
from verdict.tools.verify._shared import _validate_callee_contracts
from verdict.tools.verify.callsite import verify_callsite_preconditions
from verdict.tools.verify.contract import verify_function_contract


def _make_ctx(
    reg: AgentRegistry,
    function_info: FunctionInfo,
    tracker: VOTracker | None = None,
) -> ToolContext:
    bus = AsyncQueueBus()
    return ToolContext(
        agent_id=f"agent_{function_info.name}",
        function_info=function_info,
        registry=reg,
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
# 1. verify_function_contract.callee_contracts restrictions
# ---------------------------------------------------------------------------


def test_validate_callee_contracts_accepts_post_and_assigns():
    """Empty input and post/assigns-only input are allowed."""
    _validate_callee_contracts({})
    _validate_callee_contracts(
        {"helper": {"postconditions": ["ensures \\result >= 0;"]}}
    )
    _validate_callee_contracts(
        {"helper": {"assigns": ["assigns \\nothing;"]}}
    )
    _validate_callee_contracts(
        {
            "a": {"postconditions": ["ensures \\result > 0;"]},
            "b": {"assigns": ["assigns \\nothing;"]},
        }
    )


def test_validate_callee_contracts_rejects_preconditions():
    with pytest.raises(ValueError, match="preconditions"):
        _validate_callee_contracts(
            {"helper": {"preconditions": ["requires x > 0;"]}}
        )


def test_validate_callee_contracts_rejects_loop_contracts():
    with pytest.raises(ValueError, match="loop_contracts"):
        _validate_callee_contracts(
            {"helper": {"loop_contracts": {"L1": ["loop invariant i >= 0;"]}}}
        )


def test_validate_callee_contracts_empty_lists_are_ignored():
    """Empty precondition lists should not trip the guard — nothing is
    actually being dictated."""
    _validate_callee_contracts(
        {"helper": {"preconditions": [], "postconditions": ["ensures x;"]}}
    )


@pytest.mark.asyncio
async def test_verify_function_contract_rejects_callee_preconditions(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    with pytest.raises(ValueError, match="preconditions"):
        await verify_function_contract(
            ctx,
            annotations={"postconditions": ["ensures \\result >= 0;"]},
            callee_contracts={
                "callee": {"preconditions": ["requires x > 0;"]}
            },
        )


# ---------------------------------------------------------------------------
# 2. verify_callsite_preconditions soft-skip when callee unpublished
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_verify_callsite_preconditions_skips_unpublished_callee(main_and_callee):
    """The tool must return a descriptive result and leave the VO
    PENDING when the callee has not published yet."""
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
    ctx = _make_ctx(reg, main_info, tracker=tracker)

    result = await verify_callsite_preconditions(
        ctx,
        callee_function="callee",
    )

    # Informative summary
    assert "not published" in result.summary or "no published" in result.summary
    assert "spec_published" in result.summary
    # VO tracker is untouched
    assert tracker.call_site_vos["cs_1"].status == VOStatus.PENDING
    # No verification result details
    assert result.results == [] or result.verified_count == 0


@pytest.mark.asyncio
async def test_verify_callsite_preconditions_skips_unknown_callee(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    result = await verify_callsite_preconditions(
        ctx,
        callee_function="external_lib_fn",
    )
    assert "not published" in result.summary or "no published" in result.summary


# ---------------------------------------------------------------------------
# 3. submit_proof_demand.property_kind restrictions
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_submit_proof_demand_accepts_postcondition(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    demand = await submit_proof_demand(
        ctx,
        target_function="callee",
        property_kind="postcondition",
        property_text="ensures \\result >= 0;",
        rationale="needed for caller proof",
    )
    assert demand.property_kind == "postcondition"


@pytest.mark.asyncio
async def test_submit_proof_demand_accepts_assigns(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    demand = await submit_proof_demand(
        ctx,
        target_function="callee",
        property_kind="assigns",
        property_text="assigns \\nothing;",
        rationale="frame guarantee",
    )
    assert demand.property_kind == "assigns"


@pytest.mark.asyncio
async def test_submit_proof_demand_rejects_precondition(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    with pytest.raises(ValueError, match="precondition"):
        await submit_proof_demand(
            ctx,
            target_function="callee",
            property_kind="precondition",
            property_text="requires x > -2147483647;",
            rationale="avoid overflow",
        )


@pytest.mark.asyncio
async def test_submit_proof_demand_rejects_loop_contract(main_and_callee):
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    with pytest.raises(ValueError, match="loop_contract"):
        await submit_proof_demand(
            ctx,
            target_function="callee",
            property_kind="loop_contract",
            property_text="loop invariant i >= 0;",
            rationale="needed",
        )


@pytest.mark.asyncio
async def test_submit_proof_demand_allows_precondition_weakening(main_and_callee):
    """Caller can request callee to weaken a precondition it can't satisfy."""
    reg, main_info = main_and_callee
    ctx = _make_ctx(reg, main_info)

    demand = await submit_proof_demand(
        ctx,
        target_function="callee",
        property_kind="precondition_weakening",
        property_text="requires x > 0;",
        rationale="call site passes x=0, cannot satisfy strict bound",
    )
    assert demand.property_kind == "precondition_weakening"
    assert demand.property_text == "requires x > 0;"
