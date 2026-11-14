"""Tests for orchestrator VO invalidation on callee spec changes."""

import pytest

from verdict.core.types import (
    FunctionInfo,
    FunctionSpec,
    ProofState,
    SpecPublished,
    SpecStatus,
    VOSummary,
)
from verdict.core.spec_dep import SpecDep
from verdict.core.vo_tracker import VOEntry, VOStatus
from verdict.orchestrator.orchestrator import Orchestrator


def _setup_two_fn_orchestrator() -> Orchestrator:
    """Return an orchestrator with main → callee wired up."""
    funcs = [
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
        FunctionInfo(
            name="callee",
            source_file="callee.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
    ]
    return Orchestrator(funcs, timeout=10)


def _seed_main_vos(orch: Orchestrator) -> None:
    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    tracker.mark_verified("rte_1")
    tracker.mark_verified("cs_0")


def test_precondition_change_invalidates_only_callsite_vo_for_that_callee():
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["precondition"],
        precondition_changed=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED  # untouched
    assert tracker.call_site_vos["cs_0"].status == VOStatus.PENDING  # reset


def test_postcondition_additive_does_not_invalidate():
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["postcondition"],
        postcondition_additive_only=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["cs_0"].status == VOStatus.VERIFIED


def test_postcondition_modified_invalidates_all_vos():
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["postcondition"],
        postcondition_weakened_or_modified=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    assert tracker.rte_vos["rte_1"].status == VOStatus.PENDING
    assert tracker.call_site_vos["cs_0"].status == VOStatus.PENDING


def test_assigns_change_invalidates_all_vos():
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["assigns"],
        assigns_changed=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    assert tracker.rte_vos["rte_1"].status == VOStatus.PENDING
    assert tracker.call_site_vos["cs_0"].status == VOStatus.PENDING


def test_loop_contract_change_does_not_invalidate_caller_vos():
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["loop_contract"],
        loop_contract_changed=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["cs_0"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_global_ok_blocked_by_pending_vo():
    """LOCAL_OK agent with pending VO must not be promoted to GLOBAL_OK."""
    orch = _setup_two_fn_orchestrator()

    # Publish both contracts directly via registry (bypass tool gate).
    # Specs need non-empty content for proof_state to become LOCAL_OK.
    await orch.registry.write_spec(
        "agent_callee",
        FunctionSpec(
            function_name="callee",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    await orch.registry.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    # Seed main with a pending RTE VO
    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))

    results = orch._finalize_results()
    by_name = {r.function_name: r for r in results}

    # main: LOCAL_OK contract, but rte_1 PENDING → not GLOBAL_OK
    assert by_name["main"].proof_state != ProofState.GLOBAL_OK
    # callee: no VOs → vo_complete trivially True → GLOBAL_OK
    assert by_name["callee"].proof_state == ProofState.GLOBAL_OK


@pytest.mark.asyncio
async def test_global_ok_allows_bug_marked_vo():
    """Bug-marked VOs count as complete and don't block GLOBAL_OK."""
    orch = _setup_two_fn_orchestrator()

    await orch.registry.write_spec(
        "agent_callee",
        FunctionSpec(
            function_name="callee",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    await orch.registry.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.mark_bug("rte_1", "accepted known issue")

    results = orch._finalize_results()
    by_name = {r.function_name: r for r in results}
    assert by_name["main"].proof_state == ProofState.GLOBAL_OK

    # VOSummary reports it
    summary = by_name["main"].vo_summary
    assert summary is not None
    assert summary.rte_bug == 1
    assert summary.complete is True


@pytest.mark.asyncio
async def test_global_ok_blocked_by_failed_vo():
    """FAILED VOs block GLOBAL_OK (agent must decide: fix, weaken, or mark bug)."""
    orch = _setup_two_fn_orchestrator()

    await orch.registry.write_spec(
        "agent_callee",
        FunctionSpec(
            function_name="callee",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    await orch.registry.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.mark_failed("rte_1")

    results = orch._finalize_results()
    by_name = {r.function_name: r for r in results}
    assert by_name["main"].proof_state != ProofState.GLOBAL_OK

    summary = by_name["main"].vo_summary
    assert summary.rte_failed == 1
    assert summary.complete is False


def test_vo_summary_total_and_complete():
    s = VOSummary(rte_verified=2, rte_bug=1, callsite_verified=1)
    assert s.total == 4
    assert s.complete is True

    s2 = VOSummary(rte_verified=1, rte_pending=1)
    assert s2.total == 2
    assert s2.complete is False

    s3 = VOSummary(callsite_failed=1)
    assert s3.complete is False


def test_precondition_change_rebuilds_children():
    """When callee preconditions change, child VOs are rebuilt."""
    orch = _setup_two_fn_orchestrator()
    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
            call_site_line=10,
        )
    )
    # Simulate old children from previous publish
    tracker.rebuild_callsite_children("cs_0", ["requires old;"])
    tracker.mark_verified("cs_0_0")
    assert tracker.call_site_vos["cs_0"].status == VOStatus.VERIFIED

    # Callee publishes new spec with 2 preconditions
    from verdict.core.types import FunctionSpec, SpecStatus
    import asyncio

    asyncio.get_event_loop().run_until_complete(
        orch.registry.write_spec(
            "agent_callee",
            FunctionSpec(
                function_name="callee",
                preconditions=["requires x > 0;", "requires y >= 0;"],
                status=SpecStatus.PUBLISHED,
            ),
        )
    )

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=1,
        changed_kinds=["precondition"],
        precondition_changed=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    # Old child removed, new children created
    assert "cs_0_0" in tracker.call_site_vos  # new child 0
    assert "cs_0_1" in tracker.call_site_vos  # new child 1
    assert tracker.call_site_vos["cs_0_0"].precondition_text == "requires x > 0;"
    assert tracker.call_site_vos["cs_0_1"].precondition_text == "requires y >= 0;"
    # All reset to PENDING
    assert tracker.call_site_vos["cs_0"].status == VOStatus.PENDING
    assert tracker.call_site_vos["cs_0_0"].status == VOStatus.PENDING
    assert tracker.call_site_vos["cs_0_1"].status == VOStatus.PENDING


def test_first_publish_builds_children():
    """First callee publish creates children from scratch."""
    orch = _setup_two_fn_orchestrator()
    tracker = orch._agents_by_id["agent_main"].vo_tracker
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_0",
            vo_type="call_site",
            description="call to callee",
            related_function="callee",
        )
    )
    # No children yet
    assert tracker.call_site_vos["cs_0"].child_vo_ids == []

    # Callee publishes
    from verdict.core.types import FunctionSpec, SpecStatus
    import asyncio

    asyncio.get_event_loop().run_until_complete(
        orch.registry.write_spec(
            "agent_callee",
            FunctionSpec(
                function_name="callee",
                preconditions=["requires x >= 0;"],
                status=SpecStatus.PUBLISHED,
            ),
        )
    )

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=1,
        changed_kinds=["published"],
    )
    orch._invalidate_caller_vos("agent_main", event)

    # Children built
    assert tracker.call_site_vos["cs_0"].child_vo_ids == ["cs_0_0"]
    assert tracker.call_site_vos["cs_0_0"].precondition_text == "requires x >= 0;"


def test_precondition_change_ignores_unrelated_callsite_vos():
    orch = _setup_two_fn_orchestrator()
    tracker = orch._agents_by_id["agent_main"].vo_tracker
    # Add a call-site VO targeting a DIFFERENT (non-registered) callee
    tracker.add_call_site_vo(
        VOEntry(
            vo_id="cs_other",
            vo_type="call_site",
            description="unrelated",
            related_function="other_fn",
        )
    )
    tracker.mark_verified("cs_other")

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        precondition_changed=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    # Unrelated VO untouched
    assert tracker.call_site_vos["cs_other"].status == VOStatus.VERIFIED


def test_postcondition_modified_invalidates_spec_deps():
    """When callee guarantees change (postcondition weakened/modified), the
    caller's spec_deps entry for that callee must be cleared so stale CLAUSES
    don't let the dependency judge incorrectly advance on future publishes."""
    orch = _setup_two_fn_orchestrator()
    _seed_main_vos(orch)

    # Simulate: main had a CLAUSES dep on callee from a prior version
    entry = orch.registry.get("agent_main")
    entry.published_spec = FunctionSpec(
        function_name="main",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    entry.published_spec.spec_deps = {
        "callee": SpecDep(depends=True, clauses=frozenset(["ensures \\result >= 0;"]))
    }
    entry.spec = FunctionSpec(
        function_name="main",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    entry.spec.spec_deps = dict(entry.published_spec.spec_deps)

    event = SpecPublished(
        unit_id="agent_callee",
        function_name="callee",
        version=2,
        changed_kinds=["postcondition"],
        postcondition_weakened_or_modified=True,
    )
    orch._invalidate_caller_vos("agent_main", event)

    # spec_deps[callee] must be gone from both published and working spec
    assert "callee" not in entry.published_spec.spec_deps
    assert "callee" not in entry.spec.spec_deps


@pytest.mark.asyncio
async def test_precondition_removal_syncs_children_on_advance():
    """When a callee removes a precondition and the judge says 'unaffected'
    (advance path), stale child VOs for the removed precondition must be
    pruned, and existing children keep their verified status."""
    orch = _setup_two_fn_orchestrator()
    tracker = orch._agents_by_id["agent_main"].vo_tracker

    # Seed a callsite parent with 4 children (callee v1 had 4 preconditions)
    tracker.add_call_site_vo(
        VOEntry(vo_id="cs_0", vo_type="call_site", description="call",
                related_function="callee")
    )
    tracker.rebuild_callsite_children("cs_0", [
        "requires a;", "requires b;", "requires c;", "requires d;",
    ])
    # Mark all 4 children verified
    for i in range(4):
        tracker.mark_verified(f"cs_0_{i}")
    assert tracker.call_site_vos["cs_0"].status == VOStatus.VERIFIED

    # Publish callee v1 with 4 preconditions, then v2 with 3 (removed "d")
    await orch.registry.write_spec("agent_callee", FunctionSpec(
        function_name="callee",
        preconditions=["requires a;", "requires b;", "requires c;", "requires d;"],
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))
    # Publish main so it has based_on. Set spec_deps to NONE (proof does not
    # depend on callee's postconditions) so judge_dependency_change returns
    # UNAFFECTED → the reconcile takes the advance path (not invalidate).
    await orch.registry.write_spec("agent_main", FunctionSpec(
        function_name="main",
        postconditions=["ensures \\result >= 0;"],
        spec_deps={"callee": SpecDep(depends=False)},
        callsite_pre_deps={"callee": frozenset([
            "requires a;", "requires b;", "requires c;", "requires d;",
        ])},
        status=SpecStatus.PUBLISHED,
    ))
    # Drain callee v1 event
    orch.registry.pop_publish_events()

    # Callee publishes v2: removed "requires d;" (precondition_changed)
    await orch.registry.write_spec("agent_callee", FunctionSpec(
        function_name="callee",
        preconditions=["requires a;", "requires b;", "requires c;"],
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))

    # Run reconcile
    orch._pending_reasons = {}
    orch._reconcile_dependency_staleness()

    # Child cs_0_3 (the removed "requires d;") must be gone
    assert "cs_0_3" not in tracker.call_site_vos
    # Children 0-2 keep their VERIFIED status (proof unaffected)
    for i in range(3):
        assert tracker.call_site_vos[f"cs_0_{i}"].status == VOStatus.VERIFIED
    # Parent recomputed from children → still VERIFIED
    assert tracker.call_site_vos["cs_0"].status == VOStatus.VERIFIED
