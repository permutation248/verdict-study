"""Tests for the assume_callee-debt state machine (Plan A).

When a caller publishes under ``assume_callee`` on a callee that is itself
published, ``based_on_versions[callee] = 0`` is recorded by design to block
GLOBAL_OK. This must NOT trap the caller at DRAFT (the original bug): it should
reach LOCAL_OK, stay actionable (to discharge), and get discharge guidance.
"""

import pytest

from verdict.core.types import (
    FunctionInfo,
    FunctionSpec,
    ProofState,
    SpecStatus,
    WakeReason,
)
from verdict.core.vo_tracker import VOEntry, VOStatus
from verdict.orchestrator.orchestrator import Orchestrator
from verdict.orchestrator.proof_state import (
    recompute_proof_state,
    undischarged_assumptions,
)


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


async def _publish_callee(orch: Orchestrator) -> None:
    """callee publishes a real v1 and is promoted (leaf → GLOBAL_OK), so it is
    a trustworthy dependency for the caller."""
    await orch.registry.write_spec(
        "agent_callee",
        FunctionSpec(
            function_name="callee",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_callee")


async def _publish_caller_with_assume(orch: Orchestrator) -> None:
    """caller publishes v1 under assume_callee on the (already published) callee
    and marks all its VOs verified."""
    tracker = orch._agents_by_id["agent_caller"].vo_tracker
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.mark_verified("rte_1")
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\result >= 1;"],
            assumed_callees=["callee"],
            status=SpecStatus.PUBLISHED,
        ),
    )


@pytest.mark.asyncio
async def test_assume_publish_reaches_local_ok_not_draft():
    orch = _orch()
    await _publish_callee(orch)
    await _publish_caller_with_assume(orch)

    # based_on is zeroed for the assumed callee (blocks GLOBAL_OK)...
    entry = orch.registry.get("agent_caller")
    assert entry.published_spec.based_on_versions.get("callee") == 0

    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    # ...but it must reach LOCAL_OK, not stay trapped at DRAFT/DIRTY.
    assert orch.registry.get("agent_caller").proof_state == ProofState.LOCAL_OK


@pytest.mark.asyncio
async def test_assume_debt_keeps_local_ok_actionable():
    orch = _orch()
    await _publish_callee(orch)
    await _publish_caller_with_assume(orch)
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")

    debts = undischarged_assumptions(orch.registry, "caller")
    assert debts == ["callee"]
    # LOCAL_OK with an undischarged assumption stays actionable (to discharge).
    assert orch._is_actionable(orch._agents_by_id["agent_caller"]) is True


@pytest.mark.asyncio
async def test_assume_debt_reconcile_produces_reason():
    """Part 2 of _reconcile_dependency_staleness produces an assume_callee_debt
    reason (first-class, not a diagnose fallback)."""
    orch = _orch()
    await _publish_callee(orch)
    # Drain the callee's publish event (simulates it was already processed in
    # an earlier batch — the caller's assume happens in a later cycle).
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)

    orch._pending_reasons = {}
    orch._reconcile_dependency_staleness()
    reasons = orch._pending_reasons.get("agent_caller", [])
    debt_reasons = [r for r in reasons if r.reason == "assume_callee_debt"]
    assert len(debt_reasons) == 1
    assert "callee" in debt_reasons[0].details
    assert "assume_callee" in debt_reasons[0].details
    assert "re-verify" in debt_reasons[0].details.lower()


@pytest.mark.asyncio
async def test_discharged_assumption_promotes_to_global_ok():
    """Once the assumption is discharged (re-verify without assume → based_on
    advances to the real version), the unit reaches GLOBAL_OK."""
    orch = _orch()
    await _publish_callee(orch)
    await _publish_caller_with_assume(orch)

    # Re-verify WITHOUT assume: republish with based_on matching the real callee
    # version and no assumed_callees.
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\result >= 1;"],
            assumed_callees=[],
            status=SpecStatus.PUBLISHED,
        ),
    )
    assert undischarged_assumptions(orch.registry, "caller") == []
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    assert orch.registry.get("agent_caller").proof_state == ProofState.GLOBAL_OK


# ── GLOBAL_OK wake on outgoing-demand resolution ──


@pytest.mark.asyncio
async def test_global_ok_wakes_on_outgoing_demand_resolution():
    """A GLOBAL_OK unit that submitted a demand must wake when it resolves:
    the resolution carries follow-up work (it may have promised a clause
    contingent on the upstream answer). Open INCOMING demands are durable
    and keep the unit actionable; the outgoing-resolution event is
    transient and must not be dropped by the state gate."""
    orch = _orch()
    await _publish_callee(orch)
    # caller publishes WITHOUT assume → GLOBAL_OK.
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\result >= 1;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    assert orch.registry.get("agent_caller").proof_state == ProofState.GLOBAL_OK

    caller = orch._agents_by_id["agent_caller"]
    orch._pending_reasons = {}
    assert orch._is_actionable(caller) is False

    orch._pending_reasons = {
        "agent_caller": [WakeReason(reason="demand_satisfied", details="x")]
    }
    assert orch._is_actionable(caller) is True

    # A rejection also needs a decision from the requester.
    orch._pending_reasons = {
        "agent_caller": [WakeReason(reason="demand_rejected", details="x")]
    }
    assert orch._is_actionable(caller) is True

    # Unrelated reasons do not breach the GLOBAL_OK gate.
    orch._pending_reasons = {
        "agent_caller": [WakeReason(reason="continue", details="x")]
    }
    assert orch._is_actionable(caller) is False


# ── Settled assumes (wake budget) ──


def _reconcile(orch: Orchestrator) -> list:
    """Run reconcile with a clean reason buffer; return the caller's
    assume_callee_debt reasons."""
    orch._pending_reasons = {}
    orch._reconcile_dependency_staleness()
    return [
        r
        for r in orch._pending_reasons.get("agent_caller", [])
        if r.reason == "assume_callee_debt"
    ]


@pytest.mark.asyncio
async def test_assume_debt_budget_settles_after_limit():
    """Each debt wake burns budget only when the agent ran a cycle since the
    previous one; an exhausted budget settles the debt — no further debt
    wakes, the unit stays LOCAL_OK with the residual hypothesis marked."""
    orch = _orch()
    await _publish_callee(orch)
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)

    for attempt in (1, 2, 3):
        reasons = _reconcile(orch)
        assert len(reasons) == 1
        assert f"(attempt {attempt}/3)" in reasons[0].details
        # The agent runs a cycle in response (still publishing under assume).
        orch._wakes.seed_cycle_count("agent_caller", attempt)

    # Budget exhausted: the debt settles instead of waking again.
    assert _reconcile(orch) == []
    assert orch.registry.settled_assumes("agent_caller") == {"callee"}

    # Settled stays LOCAL_OK: the based_on=0 anchor still blocks GLOBAL_OK.
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    assert orch.registry.get("agent_caller").proof_state == ProofState.LOCAL_OK

    # Further reconciles stay quiet (debt still open but settled).
    orch._wakes.seed_cycle_count("agent_caller", 4)
    assert _reconcile(orch) == []


@pytest.mark.asyncio
async def test_assume_debt_budget_not_burned_without_cycle():
    """Re-reconciles while the agent never ran (suspended/waiting on the
    queue) must not burn budget: strikes stay at 1 until cycles advance."""
    orch = _orch()
    await _publish_callee(orch)
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)

    for _ in range(5):
        reasons = _reconcile(orch)
        assert len(reasons) == 1
        assert "(attempt 1/3)" in reasons[0].details
    assert orch.registry.settled_assumes("agent_caller") == set()


@pytest.mark.asyncio
async def test_settled_assume_cleared_on_discharge():
    """Ratchet: when the caller later re-verifies without the assumption, the
    debt disappears, the settled mark is cleared, and GLOBAL_OK is reachable."""
    orch = _orch()
    await _publish_callee(orch)
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)

    for attempt in (1, 2, 3):
        _reconcile(orch)
        orch._wakes.seed_cycle_count("agent_caller", attempt)
    _reconcile(orch)
    assert orch.registry.settled_assumes("agent_caller") == {"callee"}

    # Discharge: republish WITHOUT assume (based_on advances to the real v1).
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\result >= 1;"],
            assumed_callees=[],
            status=SpecStatus.PUBLISHED,
        ),
    )
    _reconcile(orch)
    assert orch.registry.settled_assumes("agent_caller") == set()
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")
    assert orch.registry.get("agent_caller").proof_state == ProofState.GLOBAL_OK


@pytest.mark.asyncio
async def test_settled_unit_rests_but_first_class_reasons_fire():
    """A settled LOCAL_OK unit is no longer actionable (the generic
    "continue" filler must not re-wake it forever), but a pending
    first-class reason (e.g. demand_satisfied) still fires so a late
    resolution can discharge the settled assumption."""
    orch = _orch()
    await _publish_callee(orch)
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)
    for attempt in (1, 2, 3):
        _reconcile(orch)
        orch._wakes.seed_cycle_count("agent_caller", attempt)
    _reconcile(orch)
    assert orch.registry.settled_assumes("agent_caller") == {"callee"}
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")

    caller = orch._agents_by_id["agent_caller"]
    orch._pending_reasons = {}
    assert orch._is_actionable(caller) is False

    orch._pending_reasons = {
        "agent_caller": [WakeReason(reason="demand_satisfied", details="x")]
    }
    assert orch._is_actionable(caller) is True


@pytest.mark.asyncio
async def test_settled_assumes_surface_in_snapshots():
    """Snapshots carry settled_assumes; the run summary derives the
    CONDITIONAL_OK count from them."""
    from verdict.orchestrator import reporting

    orch = _orch()
    await _publish_callee(orch)
    orch.registry.pop_publish_events()
    await _publish_caller_with_assume(orch)
    for attempt in (1, 2, 3):
        _reconcile(orch)
        orch._wakes.seed_cycle_count("agent_caller", attempt)
    _reconcile(orch)
    recompute_proof_state(orch.registry, orch._agents_by_id, "global", "agent_caller")

    snaps = reporting.snapshot_batch(
        orch._agents_by_id, orch._wakes, ["agent_caller"], registry=orch.registry
    )
    assert snaps[0].settled_assumes == ["callee"]
    assert reporting.compute_summary(snaps)["conditional_ok"] == 1


# ── actionability ordering: demands rescue suspended/FAILED targets ──


@pytest.mark.asyncio
async def test_open_incoming_demand_rescues_suspended_and_failed():
    """Mutual demands must not deadlock: a suspended (or FAILED) target with
    an open INCOMING demand stays schedulable — the demand check precedes
    the suspension and FAILED gates."""
    orch = _orch()
    await _publish_callee(orch)
    callee = orch._agents_by_id["agent_callee"]

    await orch.registry.raise_proof_demand(
        "agent_caller", "callee", "postcondition", "ensures \\result >= 5;",
    )
    # Suspended target with an open incoming demand → still actionable.
    orch._wakes.suspend("agent_callee", {"caller"}, 1)
    orch._pending_reasons = {}
    assert orch._is_actionable(callee) is True
    orch._wakes.clear_suspension("agent_callee")

    # FAILED target with an open incoming demand → still actionable.
    orch.registry.set_proof_state("agent_callee", ProofState.FAILED)
    assert orch._is_actionable(callee) is True


@pytest.mark.asyncio
async def test_failed_unit_stays_schedulable():
    """FAILED records a (often transient) runner error; the unit must stay
    schedulable so the next clean cycle can clear it. Cycle limit bounds
    persistent failures."""
    orch = _orch()
    orch.registry.set_proof_state("agent_caller", ProofState.FAILED)
    orch._pending_reasons = {}
    assert orch._is_actionable(orch._agents_by_id["agent_caller"]) is True


@pytest.mark.asyncio
async def test_local_ok_demand_resolution_rescue_without_debts():
    """A LOCAL_OK unit with NO assume debts still wakes on a resolution to
    a demand it submitted (satisfied-without-republish would otherwise rot)."""
    orch = _orch()
    await _publish_callee(orch)
    # caller LOCAL_OK without assume (global scope, callee global_ok → the
    # local_ok state needs a non-trustworthy sibling; force the state).
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\result >= 1;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    orch.registry.set_proof_state("agent_caller", ProofState.LOCAL_OK)
    caller = orch._agents_by_id["agent_caller"]
    orch._pending_reasons = {}
    assert orch._is_actionable(caller) is False
    orch._pending_reasons = {
        "agent_caller": [WakeReason(reason="demand_satisfied", details="x")]
    }
    assert orch._is_actionable(caller) is True
