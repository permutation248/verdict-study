"""Seeding: restore registry, VO state, and proof state from a previous run.

Lets a resumed solve skip already-verified units. Operates on explicit state
(registry, agents, wake queue) so the orchestrator stays the thin owner.
"""

from __future__ import annotations

import logging
from typing import TYPE_CHECKING, Any

from verdict.core.spec_dep import SpecDep
from verdict.core.types import FunctionSpec, ProofState, SpecStatus

if TYPE_CHECKING:
    from verdict.agent.registry import AgentRegistry
    from verdict.agent.runner import FunctionAgent
    from verdict.orchestrator.wake_queue import WakeQueue

logger = logging.getLogger("verdict.orchestrator")


async def seed_from_previous_run(
    registry: "AgentRegistry",
    agents_by_id: dict[str, "FunctionAgent"],
    wakes: "WakeQueue",
    prev_run: dict[str, Any],
    restore_sessions: bool = True,
) -> None:
    """Seed registry, VO states, and proof state from a previous run.

    For each agent in the previous run:
    - Writes the spec to the registry (published specs available to others)
    - Restores VO states (VERIFIED VOs skip re-verification)
    - Restores spec_vo state (VERIFIED spec skips re-verify)
    - Restores proof_state (local_ok agents skip initial batch)
    - Optionally restores session_id (for conversation history resume)
    """
    prev_agents = prev_run.get("agents", {})
    seeded = 0
    skipped = 0
    for func_name, prev in prev_agents.items():
        agent_id = f"agent_{func_name}"
        agent = agents_by_id.get(agent_id)
        if agent is None:
            continue

        prev_spec = prev.get("spec")
        if prev_spec and prev_spec.get("version", 0) > 0:
            raw_deps = prev_spec.get("spec_deps", {})
            spec_deps = {}
            for callee, dep_data in raw_deps.items():
                if isinstance(dep_data, dict):
                    clauses = dep_data.get("clauses")
                    if clauses is not None:
                        clauses = frozenset(clauses)
                    spec_deps[callee] = SpecDep(
                        depends=dep_data.get("depends", True),
                        clauses=clauses,
                    )
            spec = FunctionSpec(
                function_name=prev_spec.get("function_name", func_name),
                preconditions=prev_spec.get("preconditions", []),
                postconditions=prev_spec.get("postconditions", []),
                loop_contracts=prev_spec.get("loop_contracts", {}),
                assigns=prev_spec.get("assigns", []),
                version=prev_spec.get("version", 0),
                status=SpecStatus(prev_spec.get("status", "draft")),
                based_on_versions=prev_spec.get("based_on_versions", {}),
                spec_deps=spec_deps,
                # Proof-context bookkeeping must survive resume: write_spec
                # recomputes based_on_versions for published specs and zeroes
                # the assumed callees from THIS list — dropping it would
                # silently discharge every assume debt across a resume.
                callee_contracts={
                    name: dict(cc)
                    for name, cc in (prev_spec.get("callee_contracts") or {}).items()
                },
                assumed_callees=list(prev_spec.get("assumed_callees") or []),
                callsite_pre_deps={
                    k: frozenset(v)
                    for k, v in (prev_spec.get("callsite_pre_deps") or {}).items()
                },
            )
            await registry.write_spec(agent_id, spec)
            seeded += 1

        prev_state = prev.get("proof_state")
        vo_summary = prev.get("vo_summary", {})

        if prev_state in ("local_ok", "global_ok") and agent.vo_tracker is not None:
            vo_states = vo_summary.get("vo_states", {})
            for vo_id, status_str in vo_states.items():
                if status_str == "verified":
                    agent.vo_tracker.mark_verified(vo_id)
                elif status_str == "bug":
                    agent.vo_tracker.mark_bug(vo_id, "restored from previous run")

            spec_status = vo_summary.get("spec_status")
            if spec_status == "verified":
                agent.vo_tracker.mark_spec_verified()

            if prev_state == "local_ok":
                registry.set_proof_state(agent_id, ProofState.LOCAL_OK)
            elif prev_state == "global_ok":
                registry.set_proof_state(agent_id, ProofState.GLOBAL_OK)
            skipped += 1

        # Settled assume debts stay settled across resume — otherwise the
        # wake budget silently resets and the unit burns three more cycles
        # on a debt the previous run already gave up on. Extern axiom marks
        # ride the same registry set (the agent/extern split exists only at
        # result reporting).
        settled = list(prev.get("settled_assumes") or []) + list(
            prev.get("extern_axioms") or []
        )
        if settled:
            registry.settle_assumes(agent_id, settled)

        if restore_sessions:
            session_id = prev.get("session_id")
            if session_id:
                agent._session_id = session_id

        prev_cycles = prev.get("cycle_count", 0)
        if prev_cycles > 0:
            wakes.seed_cycle_count(agent_id, prev_cycles)

    logger.info(
        "Seeded %d specs, %d agents with restored VO state from previous run",
        seeded, skipped,
    )
