"""Agent Registry: central store for agent entries, specs, and state."""

from __future__ import annotations

import asyncio
import copy
from dataclasses import dataclass, field
from typing import TYPE_CHECKING, Optional

from verdict.core.types import (
    DemandResolved,
    DemandStatus,
    FunctionInfo,
    FunctionSpec,
    ProofDemand,
    ProofState,
    SpecPublished,
    SpecStatus,
)
from verdict.core.spec_diff import (
    _changed_kinds,
    _published_equivalent,
    classify_spec_change,
)
from verdict.agent.proof_demands import ProofDemandStore

if TYPE_CHECKING:
    from verdict.core.run_store import RunStore


def _spec_has_content(spec: FunctionSpec) -> bool:
    return bool(
        spec.preconditions
        or spec.postconditions
        or spec.loop_contracts
        or spec.assigns
    )


def _initial_proof_state(spec: FunctionSpec | None) -> ProofState:
    """The BOOTSTRAP proof state derivable from spec content alone.

    Only ever EMPTY (no content) or DRAFT (has content). This is the
    *starting* state, not the final one: LOCAL_OK/GLOBAL_OK/DIRTY require
    VOs (VOTracker) and cross-agent dependency facts, so promotion/demotion
    is owned solely by ``Orchestrator._recompute_proof_state``. Keeping this
    function content-only is what makes that single-authority split hold.
    """
    if spec is None or not _spec_has_content(spec):
        return ProofState.EMPTY
    return ProofState.DRAFT


@dataclass
class AgentEntry:
    """Registry entry for a single Function Agent."""

    agent_id: str
    function_info: FunctionInfo
    proof_state: ProofState = ProofState.EMPTY
    published_spec: FunctionSpec | None = None
    # Assume debts whose wake budget is exhausted (see Orchestrator Part 2).
    # The based_on=0 anchor is untouched, so the unit stays LOCAL_OK; the
    # orchestrator just stops waking it for these and reports them as
    # residual hypotheses.
    settled_assumes: set[str] = field(default_factory=set)
    spec: FunctionSpec = field(init=False)

    def __post_init__(self) -> None:
        self.spec = FunctionSpec(function_name=self.function_info.name)


class AgentRegistry:
    """Central registry for all Function Agents (thread-safe via asyncio.Lock)."""

    def __init__(self, run_store: "RunStore | None" = None) -> None:
        self._entries: dict[str, AgentEntry] = {}
        self._func_to_agent: dict[str, str] = {}
        self._published_history: dict[str, list[FunctionSpec]] = {}
        self._publish_events: list[SpecPublished] = []
        self._demands = ProofDemandStore(run_store)
        self._lock = asyncio.Lock()
        self._run_store = run_store

    def register(self, entry: AgentEntry) -> None:
        self._entries[entry.agent_id] = entry
        self._func_to_agent[entry.function_info.name] = entry.agent_id
        self._published_history.setdefault(entry.function_info.name, [])

    def get(self, agent_id: str) -> Optional[AgentEntry]:
        return self._entries.get(agent_id)

    def get_by_function(self, function_name: str) -> Optional[AgentEntry]:
        aid = self._func_to_agent.get(function_name)
        return self._entries.get(aid) if aid else None

    def all_entries(self) -> list[AgentEntry]:
        return list(self._entries.values())

    # ── SWMR spec access ──

    def read_spec(self, function_name: str) -> Optional[FunctionSpec]:
        """Read the latest published spec snapshot for a function."""
        entry = self.get_by_function(function_name)
        if entry is None or entry.published_spec is None:
            return None
        return copy.deepcopy(entry.published_spec)

    def read_working_spec(self, function_name: str) -> Optional[FunctionSpec]:
        """Read the owner's current working spec, including drafts."""
        entry = self.get_by_function(function_name)
        if entry is None:
            return None
        return copy.deepcopy(entry.spec)

    def spec_history(self, function_name: str) -> list[FunctionSpec]:
        """Return published spec history for a function."""
        return [copy.deepcopy(spec) for spec in self._published_history.get(function_name, [])]

    def pop_publish_events(self) -> list[SpecPublished]:
        """Drain pending spec publication events."""
        events = list(self._publish_events)
        self._publish_events.clear()
        return events

    def open_proof_demands(self, function_name: str) -> list[ProofDemand]:
        """Return currently open proof demands for a function."""
        return self._demands.open_for(function_name)

    def open_proof_demands_submitted_by(self, agent_id: str) -> list[ProofDemand]:
        """Return open proof demands this agent submitted (still awaiting reply)."""
        return self._demands.open_submitted_by(agent_id)

    def pop_new_proof_demands(self) -> list[ProofDemand]:
        """Drain newly raised proof demands for orchestrator scheduling."""
        return self._demands.pop_new()

    async def raise_proof_demand(
        self,
        requester_id: str,
        target_function: str,
        property_kind: str,
        property_text: str,
        blocking_vo_ids: list[str] | None = None,
        rationale: str | None = None,
    ) -> ProofDemand:
        """Record a runtime-owned proof demand for the target owner."""
        async with self._lock:
            requester = self._entries.get(requester_id)
            if requester is None:
                raise KeyError(f"Unknown agent: {requester_id}")
            if self.get_by_function(target_function) is None:
                raise KeyError(f"Unknown function: {target_function}")

            requester_spec = requester.spec
            return self._demands.add(
                requester_id,
                target_function,
                property_kind,
                property_text,
                blocking_vo_ids,
                rationale,
                requester_spec.version if requester_spec is not None else None,
                copy.deepcopy(requester_spec.based_on_versions)
                if requester_spec is not None
                else {},
            )

    async def resolve_proof_demand(
        self,
        agent_id: str,
        demand_id: str,
        resolution: DemandStatus,
        message: str | None = None,
    ) -> ProofDemand:
        """Resolve an open proof demand (satisfied or rejected)."""
        async with self._lock:
            entry = self._entries.get(agent_id)
            if entry is None:
                raise KeyError(f"Unknown agent: {agent_id}")
            return self._demands.resolve(
                agent_id, entry.function_info.name, demand_id, resolution, message
            )

    def pop_resolved_demands(self) -> list[DemandResolved]:
        """Drain resolved demand events for orchestrator scheduling."""
        return self._demands.pop_resolved()

    async def write_spec(self, agent_id: str, spec: FunctionSpec) -> FunctionSpec:
        """Only the owner agent may write its own spec (Single-Writer)."""
        async with self._lock:
            entry = self._entries.get(agent_id)
            if entry is None:
                raise KeyError(f"Unknown agent: {agent_id}")
            if entry.function_info.name != spec.function_name:
                raise PermissionError(
                    f"Agent {agent_id} cannot write spec for {spec.function_name}"
                )

            normalized = copy.deepcopy(spec)
            if not isinstance(normalized.status, SpecStatus):
                try:
                    normalized.status = SpecStatus(normalized.status)
                except ValueError:
                    normalized.status = SpecStatus.DRAFT
            # Runtime owns based_on_versions for published specs (correctness anchor).
            # Drafts may keep agent-provided values for local exploration.
            if normalized.status == SpecStatus.PUBLISHED:
                normalized.based_on_versions = self.current_dependency_versions(
                    normalized.function_name
                )
                # Callees verified under optimistic hypothesis (assume_callee)
                # get version 0, blocking GLOBAL_OK until the agent re-verifies
                # without the assumption.
                for assumed in normalized.assumed_callees:
                    if assumed in normalized.based_on_versions:
                        normalized.based_on_versions[assumed] = 0
                # Extern callees never publish, so their assumptions can
                # never be discharged: they are standing environment
                # axioms. Settle them at birth — no debt wakes, but the
                # mark routes them into conditional reporting.
                extern_assumed = [
                    a
                    for a in normalized.assumed_callees
                    if a in entry.function_info.extern_callees
                ]
                if extern_assumed:
                    self.settle_assumes(agent_id, extern_assumed)
            elif not normalized.based_on_versions:
                normalized.based_on_versions = self.current_dependency_versions(
                    normalized.function_name
                )

            old_published = entry.published_spec

            emitted_event: SpecPublished | None = None
            if normalized.status == SpecStatus.PUBLISHED:
                if old_published and _published_equivalent(old_published, normalized):
                    # No-op republish: contract body unchanged. Keep the
                    # version, emit no event, wake no callers. Refresh the
                    # dependency bookkeeping in place so the staleness scan and
                    # GLOBAL_OK promotion see the unit as current, and the
                    # dependency judge reads up-to-date support — none of which
                    # constitutes a public change.
                    normalized.version = old_published.version
                    old_published.based_on_versions = dict(
                        normalized.based_on_versions
                    )
                    old_published.spec_deps = dict(normalized.spec_deps)
                    old_published.assumed_callees = list(
                        normalized.assumed_callees
                    )
                    old_published.callsite_pre_deps = dict(
                        normalized.callsite_pre_deps
                    )
                else:
                    normalized.version = (old_published.version if old_published else 0) + 1
                    classification = classify_spec_change(old_published, normalized)
                    emitted_event = SpecPublished(
                        unit_id=agent_id,
                        function_name=normalized.function_name,
                        version=normalized.version,
                        changed_kinds=_changed_kinds(old_published, normalized),
                        precondition_changed=classification.precondition_changed,
                        postcondition_additive_only=classification.postcondition_additive_only,
                        postcondition_weakened_or_modified=classification.postcondition_weakened_or_modified,
                        assigns_changed=classification.assigns_changed,
                        loop_contract_changed=classification.loop_contract_changed,
                    )
                    self._publish_events.append(emitted_event)
                    published_copy = copy.deepcopy(normalized)
                    entry.published_spec = published_copy
                    self._published_history[normalized.function_name].append(
                        copy.deepcopy(published_copy)
                    )
            else:
                normalized.version = old_published.version if old_published else 0

            entry.spec = copy.deepcopy(normalized)
            if normalized.status == SpecStatus.PUBLISHED and entry.published_spec is None:
                entry.published_spec = copy.deepcopy(normalized)

            entry.proof_state = _initial_proof_state(entry.spec)

            if self._run_store is not None:
                if emitted_event is not None:
                    self._run_store.append_event(
                        "spec_published",
                        data={
                            "function_name": emitted_event.function_name,
                            "version": emitted_event.version,
                            "changed_kinds": list(emitted_event.changed_kinds),
                            "precondition_changed": emitted_event.precondition_changed,
                            "postcondition_additive_only": emitted_event.postcondition_additive_only,
                            "postcondition_weakened_or_modified": emitted_event.postcondition_weakened_or_modified,
                            "assigns_changed": emitted_event.assigns_changed,
                            "loop_contract_changed": emitted_event.loop_contract_changed,
                            "spec": copy.deepcopy(dict(
                                preconditions=normalized.preconditions,
                                postconditions=normalized.postconditions,
                                loop_contracts=normalized.loop_contracts,
                                assigns=normalized.assigns,
                                based_on_versions=dict(normalized.based_on_versions),
                            )),
                        },
                        agent_id=agent_id,
                    )
                elif normalized.status == SpecStatus.PUBLISHED:
                    # Content-equivalent republish: useful for timeline, but
                    # separate type so subscribers can filter.
                    self._run_store.append_event(
                        "spec_republish_noop",
                        data={
                            "function_name": normalized.function_name,
                            "version": normalized.version,
                        },
                        agent_id=agent_id,
                    )
                elif normalized.status == SpecStatus.DRAFT:
                    self._run_store.append_event(
                        "spec_drafted",
                        data={
                            "function_name": normalized.function_name,
                            "preconditions": list(normalized.preconditions),
                            "postconditions": list(normalized.postconditions),
                            "assigns": list(normalized.assigns),
                        },
                        agent_id=agent_id,
                    )

            return copy.deepcopy(normalized)

    def current_dependency_versions(self, function_name: str) -> dict[str, int]:
        """Return the latest published version snapshot for direct callees.

        Extern callees (prototype-only environment functions) are not
        dependencies: nothing will ever publish for them, so including
        them would permanently block GLOBAL_OK. Their contracts are
        environment axioms handled by the assume/settle machinery instead.
        """
        entry = self.get_by_function(function_name)
        if entry is None:
            raise KeyError(f"Unknown function: {function_name}")

        extern = set(entry.function_info.extern_callees)
        versions: dict[str, int] = {}
        for callee_name in entry.function_info.callees:
            if callee_name in extern:
                continue
            callee_entry = self.get_by_function(callee_name)
            if callee_entry is None or callee_entry.published_spec is None:
                versions[callee_name] = 0
            else:
                versions[callee_name] = callee_entry.published_spec.version
        return versions

    def dependencies_current(
        self,
        function_name: str,
        spec: FunctionSpec | None = None,
    ) -> bool:
        """Check whether a spec still matches the latest direct callee versions."""
        current_versions = self.current_dependency_versions(function_name)
        if not current_versions:
            return True
        if any(version == 0 for version in current_versions.values()):
            return False
        return not self.dependency_mismatches(function_name, spec)

    def dependency_mismatches(
        self,
        function_name: str,
        spec: FunctionSpec | None = None,
    ) -> dict[str, tuple[int, int]]:
        """Return mismatched direct callee versions as {callee: (seen, current)}."""
        entry = self.get_by_function(function_name)
        if entry is None:
            raise KeyError(f"Unknown function: {function_name}")

        target = spec
        if target is None:
            target = entry.published_spec if entry.published_spec is not None else entry.spec
        if target is None:
            return {}

        current_versions = self.current_dependency_versions(function_name)
        if not current_versions:
            return {}

        mismatches: dict[str, tuple[int, int]] = {}
        for callee_name, version in current_versions.items():
            seen = target.based_on_versions.get(callee_name, 0)
            if seen != version:
                mismatches[callee_name] = (seen, version)
        return mismatches

    def advance_dependency_snapshot(
        self,
        function_name: str,
        callee: str,
        new_version: int,
    ) -> None:
        """Surgically advance one callee's recorded version on f's specs.

        Sets ``based_on_versions[callee] = new_version`` on the entry's
        ``published_spec`` (and ``entry.spec``) WITHOUT bumping the version,
        changing ``proof_state``, or emitting ``SpecPublished``.

        DESIGN NOTE (refines doc §1): doc §1 prescribes routing through
        ``write_spec``'s content-equivalent republish so ``based_on`` is
        recomputed canonically. We deliberately deviate and write the single
        named entry surgically instead. ``write_spec`` recomputes the WHOLE
        ``based_on_versions`` map from ``current_dependency_versions``, which
        would also advance OTHER, un-judged callees to their current versions
        — unsoundly hiding mismatches the judge never adjudicated. Advancing
        only the judged ``(callee, new_version)`` pair keeps every other
        callee's mismatch visible to the single fixpoint predicate.

        Synchronous and lock-free, matching the surrounding sync state
        mutators (``set_proof_state``/``mark_global_ok``);
        callers must not hold ``self._lock``.
        """
        if new_version == 0:
            raise ValueError(
                f"advance_dependency_snapshot: new_version must be > 0, got "
                f"{new_version} for {function_name} -> {callee}"
            )

        entry = self.get_by_function(function_name)
        if entry is None:
            raise KeyError(f"Unknown function: {function_name}")

        if entry.published_spec is not None:
            entry.published_spec.based_on_versions[callee] = new_version
        if entry.spec is not None:
            entry.spec.based_on_versions[callee] = new_version

        if self._run_store is not None:
            self._run_store.append_event(
                "dependency_advanced",
                data={
                    "function_name": function_name,
                    "callee": callee,
                    "new_version": new_version,
                },
                agent_id=entry.agent_id,
            )

    # ── State management ──

    def set_proof_state(self, agent_id: str, state: ProofState) -> None:
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        entry.proof_state = state

    def get_proof_state(self, agent_id: str) -> ProofState:
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        return entry.proof_state

    # ── Settled-assume bookkeeping (assume-debt cycle budget) ──

    def settled_assumes(self, agent_id: str) -> set[str]:
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        return set(entry.settled_assumes)

    def settle_assumes(self, agent_id: str, callees: list[str]) -> None:
        """Mark assume debts as settled.

        Two callers: the orchestrator after a debt's wake budget ran out,
        and ``write_spec`` for extern (environment) callees, whose
        assumptions are settled at birth because nothing can ever
        discharge them.

        Settling never promotes: an agent-callee debt keeps
        ``based_on_versions[callee] = 0``, capping the unit at LOCAL_OK
        (extern callees are not dependencies, so they don't cap anything).
        Either way the assumption is reported as a residual hypothesis.
        Publish-event wakes (reconcile Part 1) are NOT suppressed, so a
        later agent-callee republish can still discharge the debt — at
        which point ``clear_settled_assumes`` removes the mark.
        """
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        new = sorted(set(callees) - entry.settled_assumes)
        entry.settled_assumes.update(callees)
        if self._run_store is not None and new:
            self._run_store.append_event(
                "assume_settled",
                data={
                    "function_name": entry.function_info.name,
                    "callees": new,
                },
                agent_id=agent_id,
            )

    def clear_settled_assumes(self, agent_id: str, callees: set[str]) -> None:
        """Drop settled marks for debts that no longer exist (discharged)."""
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        cleared = sorted(entry.settled_assumes & set(callees))
        entry.settled_assumes -= set(callees)
        if self._run_store is not None and cleared:
            self._run_store.append_event(
                "assume_settle_cleared",
                data={
                    "function_name": entry.function_info.name,
                    "callees": cleared,
                },
                agent_id=agent_id,
            )

    def clear_failure(self, agent_id: str) -> None:
        """Reset a FAILED execution flag back to the spec-content bootstrap.

        The runner owns the FAILED flag: it sets it when a cycle throws and
        calls this when a cycle completes cleanly. Resetting to the bootstrap
        (EMPTY/DRAFT) lets ``Orchestrator._recompute_proof_state`` re-derive
        promotion afterwards. No-op unless the agent is currently FAILED.
        """
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        if entry.proof_state == ProofState.FAILED:
            entry.proof_state = _initial_proof_state(entry.spec)

    def mark_global_ok(self, agent_id: str) -> None:
        entry = self._entries.get(agent_id)
        if entry is None:
            raise KeyError(f"Unknown agent: {agent_id}")
        if entry.proof_state == ProofState.FAILED:
            return  # FAILED is sticky (runner-owned).
        entry.proof_state = ProofState.GLOBAL_OK
        if self._run_store is not None:
            self._run_store.append_event(
                "agent_global_ok",
                data={"function_name": entry.function_info.name},
                agent_id=agent_id,
            )
