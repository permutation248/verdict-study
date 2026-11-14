"""Tests for seeding (cross-run resume): the proof-context bookkeeping
(callee_contracts, assumed_callees) and the settled-assume marks must
survive a resume — otherwise the based_on=0 debt anchor evaporates and a
resumed run can reach GLOBAL_OK on an undischarged assumption.
"""

import pytest

from verdict.core.types import FunctionInfo, ProofState
from verdict.core.vo_tracker import VOEntry, VOStatus
from verdict.orchestrator import seeding
from verdict.orchestrator.orchestrator import Orchestrator
from verdict.orchestrator.proof_state import recompute_proof_state


def _orch() -> Orchestrator:
    funcs = [
        FunctionInfo(
            name="caller",
            source_file="c.c",
            source_code="int caller(){ return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
        FunctionInfo(
            name="callee",
            source_file="d.c",
            source_code="int callee(int x){ return x; }",
            callees=[],
            callers=["caller"],
        ),
    ]
    return Orchestrator(funcs, timeout=10)


def _prev_run() -> dict:
    """A previous run where caller published under assume_callee on callee
    (based_on=0 anchor) and the debt was settled."""
    cc = {"callee": {"postconditions": ["ensures \\result >= 0;"],
                     "assigns": ["assigns \\nothing;"]}}
    return {
        "agents": {
            # callee first so the caller's recomputed based_on sees its v1.
            "callee": {
                "proof_state": "global_ok",
                "cycle_count": 1,
                "vo_summary": {"vo_states": {}, "spec_status": "verified"},
                "spec": {
                    "function_name": "callee",
                    "postconditions": ["ensures \\result >= 0;"],
                    "version": 1,
                    "status": "published",
                    "based_on_versions": {},
                },
            },
            "caller": {
                "proof_state": "local_ok",
                "cycle_count": 3,
                "settled_assumes": ["callee"],
                "vo_summary": {
                    "vo_states": {"rte_1": "verified"},
                    "spec_status": "verified",
                },
                "spec": {
                    "function_name": "caller",
                    "postconditions": ["ensures \\result >= 1;"],
                    "version": 1,
                    "status": "published",
                    "based_on_versions": {"callee": 0},
                    "callee_contracts": cc,
                    "assumed_callees": ["callee"],
                },
            },
        }
    }


@pytest.mark.asyncio
async def test_seed_restores_assume_bookkeeping_and_anchor():
    orch = _orch()
    orch._agents_by_id["agent_caller"].vo_tracker.add_rte_vo(
        VOEntry(vo_id="rte_1", vo_type="rte", description="r")
    )

    await seeding.seed_from_previous_run(
        orch.registry, orch._agents_by_id, orch._wakes, _prev_run(),
    )

    spec = orch.registry.read_spec("caller")
    assert spec.assumed_callees == ["callee"]
    assert spec.callee_contracts["callee"]["postconditions"] == [
        "ensures \\result >= 0;"
    ]
    # The anchor: write_spec recomputed based_on from the live registry
    # (callee published v1) and re-zeroed the assumed callee.
    assert spec.based_on_versions["callee"] == 0

    # Anchor still blocks GLOBAL_OK after resume.
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    assert orch.registry.get("agent_caller").proof_state == ProofState.LOCAL_OK

    # Restored VO state intact.
    tracker = orch._agents_by_id["agent_caller"].vo_tracker
    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_seed_restores_settled_assumes():
    orch = _orch()
    await seeding.seed_from_previous_run(
        orch.registry, orch._agents_by_id, orch._wakes, _prev_run(),
    )
    assert orch.registry.settled_assumes("agent_caller") == {"callee"}

    # Settled debt stays quiet after resume: reconcile produces no
    # assume_callee_debt wake for it.
    orch.registry.pop_publish_events()
    orch._pending_reasons = {}
    orch._reconcile_dependency_staleness()
    reasons = [
        r
        for r in orch._pending_reasons.get("agent_caller", [])
        if r.reason == "assume_callee_debt"
    ]
    assert reasons == []
