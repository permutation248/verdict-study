"""Reporting: pure renderers that turn live orchestrator state into
AgentResult snapshots, VO summaries, and run-summary dicts.

No mutation, no proof-state derivation — these only read (registry via the
agents, the WakeQueue counters) and produce result objects for the CLI /
run_store. They take their inputs explicitly so the orchestrator stays the
thin owner of the live state.
"""

from __future__ import annotations

from typing import TYPE_CHECKING

from verdict.core.types import AgentResult, VOSummary

if TYPE_CHECKING:
    from verdict.agent.registry import AgentRegistry
    from verdict.agent.runner import FunctionAgent
    from verdict.orchestrator.wake_queue import WakeQueue


def build_vo_summary(agent: "FunctionAgent") -> VOSummary | None:
    """Snapshot the agent's VO state into a VOSummary, or None if no tracker."""
    from verdict.core.vo_tracker import VOStatus

    tracker = agent.vo_tracker
    if tracker is None:
        return None

    summary = VOSummary()
    # Use leaf call-site VOs to avoid double-counting parents.
    leaf_cs = tracker.leaf_call_site_vos()
    all_entries = list(tracker.rte_vos.values()) + list(leaf_cs.values())

    for vo in tracker.rte_vos.values():
        if vo.status == VOStatus.VERIFIED:
            summary.rte_verified += 1
        elif vo.status == VOStatus.PENDING:
            summary.rte_pending += 1
        elif vo.status == VOStatus.FAILED:
            summary.rte_failed += 1
        elif vo.status == VOStatus.BUG:
            summary.rte_bug += 1
    for vo in leaf_cs.values():
        if vo.status == VOStatus.VERIFIED:
            summary.callsite_verified += 1
        elif vo.status == VOStatus.PENDING:
            summary.callsite_pending += 1
        elif vo.status == VOStatus.FAILED:
            summary.callsite_failed += 1
        elif vo.status == VOStatus.BUG:
            summary.callsite_bug += 1

    for vo in all_entries:
        if vo.status == VOStatus.PENDING:
            summary.pending_ids.append(vo.vo_id)
        elif vo.status == VOStatus.FAILED:
            summary.failed_ids.append(vo.vo_id)
        elif vo.status == VOStatus.BUG:
            summary.bugs[vo.vo_id] = vo.bug_reason or ""

    if tracker.spec_vo is not None:
        summary.spec_status = tracker.spec_vo.status.value

    for vo_id, vo in tracker.rte_vos.items():
        summary.vo_states[vo_id] = vo.status.value
    for vo_id, vo in leaf_cs.items():
        summary.vo_states[vo_id] = vo.status.value

    return summary


def snapshot_batch(
    agents_by_id: dict[str, "FunctionAgent"],
    wakes: "WakeQueue",
    agent_ids: list[str],
    registry: "AgentRegistry | None" = None,
) -> list[AgentResult]:
    """Build lightweight AgentResult snapshots for a subset of agents (used by
    the on_batch_complete callback for mid-run progress)."""
    snapshots: list[AgentResult] = []
    for agent_id in agent_ids:
        agent = agents_by_id.get(agent_id)
        if agent is None:
            continue
        result = agent.snapshot_result()
        result.cycle_count = wakes.cycle_count(agent_id)
        result.cycle_limit_reached = wakes.is_cycle_limited(agent_id)
        result.vo_summary = build_vo_summary(agent)
        result.pending_wakes = wakes.pending_wakes(agent_id)
        result.callees = list(agent.function_info.callees)
        if registry is not None:
            result.settled_assumes = sorted(registry.settled_assumes(agent_id))
        snapshots.append(result)
    return snapshots


def snapshot_all(
    agents_by_id: dict[str, "FunctionAgent"],
    wakes: "WakeQueue",
    registry: "AgentRegistry | None" = None,
) -> list[AgentResult]:
    """Snapshot every agent's current state."""
    return snapshot_batch(
        agents_by_id, wakes, list(agents_by_id.keys()), registry=registry
    )


def compute_summary(snapshots: list[AgentResult]) -> dict:
    return {
        "total": len(snapshots),
        "global_ok": sum(
            1
            for s in snapshots
            if s.proof_state is not None and s.proof_state.value == "global_ok"
        ),
        "local_ok": sum(
            1
            for s in snapshots
            if s.proof_state is not None and s.proof_state.value == "local_ok"
        ),
        "dirty": sum(
            1
            for s in snapshots
            if s.proof_state is not None and s.proof_state.value == "dirty"
        ),
        "failed": sum(1 for s in snapshots if s.error),
        "cycle_limited": sum(1 for s in snapshots if s.cycle_limit_reached),
        # LOCAL_OK units whose assume debts were settled (wake budget spent):
        # verified modulo residual hypotheses. Subset of local_ok, reported
        # separately as the derived CONDITIONAL_OK label.
        "conditional_ok": sum(
            1
            for s in snapshots
            if s.proof_state is not None
            and s.proof_state.value == "local_ok"
            and s.settled_assumes
        ),
        # VOs accepted via mark_vo_as_bug — counted as discharged, so they
        # inflate global_ok/local_ok; track separately for transparency.
        "bugs": sum(
            s.vo_summary.rte_bug + s.vo_summary.callsite_bug
            for s in snapshots
            if s.vo_summary is not None
        ),
    }
