"""Proof-state derivation — the sole authority for all proof-state transitions
except EMPTY (bootstrap) and FAILED (runner-owned).

Pure function of (published_spec, VOs, dependency snapshot, callee states) →
one of {DRAFT, DIRTY, LOCAL_OK, GLOBAL_OK}. The two exceptions:
  - EMPTY is set by registry bootstrap (no spec exists yet).
  - FAILED is set by the runner when a cycle produces no usable spec.
"""

from __future__ import annotations

from typing import TYPE_CHECKING

from verdict.core.types import ProofState

if TYPE_CHECKING:
    from verdict.agent.registry import AgentRegistry
    from verdict.agent.runner import FunctionAgent


def is_callee_published(registry: "AgentRegistry", callee_name: str) -> bool:
    """Whether a callee has a published spec (for is_locally_complete)."""
    entry = registry.get_by_function(callee_name)
    return entry is not None and entry.published_spec is not None


def untrusted_callees(registry: "AgentRegistry", function_name: str) -> list[str]:
    """Direct callees NOT in a trustworthy (LOCAL_OK/GLOBAL_OK) state, as
    ``name (state)`` strings. The single callee-state enumeration, shared by
    the GLOBAL_OK gate (``_callees_trustworthy``) and finalize's blocked-reason
    rendering — so the two cannot diverge.
    """
    entry = registry.get_by_function(function_name)
    if entry is None:
        return []
    untrusted: list[str] = []
    for callee_name in entry.function_info.callees:
        callee = registry.get_by_function(callee_name)
        if callee is None:
            untrusted.append(f"{callee_name} (unregistered)")
        elif callee.proof_state not in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}:
            untrusted.append(f"{callee_name} ({callee.proof_state.value})")
    return untrusted


def _callees_trustworthy(registry: "AgentRegistry", function_name: str) -> bool:
    """True iff every direct callee is registered and LOCAL_OK/GLOBAL_OK."""
    entry = registry.get_by_function(function_name)
    if entry is None:
        return False
    return not untrusted_callees(registry, function_name)


def has_real_dependency_drift(
    registry: "AgentRegistry", function_name: str
) -> bool:
    """True if any non-assumed callee version drifted — genuine staleness.

    Assumed callees carry ``based_on=0`` by design to block GLOBAL_OK. That is
    an open hypothesis debt, not proof-invalidating drift — it must NOT count
    as staleness for LOCAL_OK. Only real (non-assumed) version mismatches make
    the proof potentially invalid.
    """
    mismatches = registry.dependency_mismatches(function_name)
    if not mismatches:
        return False
    entry = registry.get_by_function(function_name)
    assumed = set(
        entry.published_spec.assumed_callees
        if entry is not None and entry.published_spec is not None
        else []
    )
    return any(callee not in assumed for callee in mismatches)


def undischarged_assumptions(
    registry: "AgentRegistry", function_name: str
) -> list[str]:
    """Assumed callees that are published but still at version 0 — open debt.

    Publishing under ``assume_callee`` records ``based_on_versions[callee] = 0``
    by design (to block GLOBAL_OK). When such a callee is itself published, the
    assumption is an open debt: the caller must discharge it (re-verify without
    the assumption, or demand the callee) to reach GLOBAL_OK.

    A callee that is assumed but NOT yet published is not a debt — it is an
    ordinary "waiting for the callee" case handled by the publish-event reconcile.
    """
    mismatches = registry.dependency_mismatches(function_name)
    if not mismatches:
        return []
    entry = registry.get_by_function(function_name)
    assumed = set(
        entry.published_spec.assumed_callees
        if entry is not None and entry.published_spec is not None
        else []
    )
    debts: list[str] = []
    for callee in mismatches:
        if callee not in assumed:
            continue
        callee_entry = registry.get_by_function(callee)
        if callee_entry is not None and callee_entry.published_spec is not None:
            debts.append(callee)
    return debts


def recompute_proof_state(
    registry: "AgentRegistry",
    agents_by_id: dict[str, "FunctionAgent"],
    scope_kind: str,
    agent_id: str,
) -> None:
    """Sole authority for proof-state derivation.

    Covers ALL agents (including unpublished DRAFT). The state is a pure
    function of current facts:

      EMPTY      no spec at all (bootstrap, untouched)
      FAILED     runner error (untouched, runner-owned)
      DRAFT      has spec but not published, or published but not complete
      DIRTY      published + (¬locally_complete ∨ stale)
      LOCAL_OK   published + locally_complete + ¬stale
      GLOBAL_OK  LOCAL_OK + deps_current + callees_trustworthy
    """
    entry = registry.get(agent_id)
    if entry is None:
        return
    if entry.proof_state == ProofState.FAILED:
        return
    if entry.proof_state == ProofState.EMPTY and entry.spec is None:
        return

    # Unpublished agent: stays DRAFT (has a working spec or is mid-cycle
    # but never successfully published). Recompute cannot promote without
    # a published spec — the publish gate in update_my_spec is the
    # gateway to LOCAL_OK.
    if entry.published_spec is None:
        if entry.proof_state != ProofState.DRAFT:
            registry.set_proof_state(agent_id, ProofState.DRAFT)
        return

    agent = agents_by_id.get(agent_id)
    tracker = agent.vo_tracker if agent else None
    fname = entry.function_info.name

    locally_complete = tracker is None or tracker.is_locally_complete(
        lambda name: is_callee_published(registry, name)
    )
    stale = has_real_dependency_drift(registry, fname)

    if not locally_complete or stale:
        if entry.proof_state not in {ProofState.DRAFT, ProofState.DIRTY}:
            registry.set_proof_state(agent_id, ProofState.DIRTY)
        return

    if (
        scope_kind == "global"
        and registry.dependencies_current(fname)
        and _callees_trustworthy(registry, fname)
    ):
        if entry.proof_state != ProofState.GLOBAL_OK:
            registry.mark_global_ok(agent_id)
    else:
        registry.set_proof_state(agent_id, ProofState.LOCAL_OK)
