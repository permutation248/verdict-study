"""Global Orchestrator: event-driven solve, wake-up, and convergence control."""

from __future__ import annotations

import asyncio
import logging
from dataclasses import asdict
from typing import TYPE_CHECKING, Any, Callable, Literal

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.agent.runner import FunctionAgent
from verdict.core.ablation import AblationConfig
from verdict.cli.display import _build_function_info
from verdict.core.project_context import ProjectContext
from verdict.core.run_record import RunStatus
from verdict.core.types import (
    AgentResult,
    FunctionInfo,
    ProofState,
    SpecPublished,
    WakeReason,
)
from verdict.logging.logger import setup_logging
from verdict.messaging.bus import AsyncQueueBus
from verdict.orchestrator import reporting, seeding
from verdict.orchestrator.proof_state import (
    recompute_proof_state,
    undischarged_assumptions,
    untrusted_callees,
)
from verdict.orchestrator.wake_queue import WakeQueue

if TYPE_CHECKING:
    from verdict.cli.agent_stream import AgentOutputSink
    from verdict.core.run_store import RunStore
    from verdict.core.spec_tracker import SpecTracker

logger = logging.getLogger("verdict.orchestrator")

DEFAULT_TIMEOUT = 3600
DEFAULT_MAX_CYCLES_PER_AGENT = 25
# Woken attempts an agent gets per assume debt before the debt is settled
# (LOCAL_OK with the assumption reported as a residual hypothesis).
ASSUME_DEBT_CYCLE_LIMIT = 3
SolveScope = Literal["global", "functions"]

# wake_selector: async callable that receives the pending ready dict
# and returns (selected_agent_ids, user_messages_per_agent).
# Return None to signal quit.
WakeSelection = tuple[list[str], dict[str, str]] | None


class Orchestrator:
    """Run initial solves, then auto-wake dirty units until quiescence."""

    def __init__(
        self,
        functions: list[FunctionInfo] | None = None,
        timeout: float = DEFAULT_TIMEOUT,
        source_file: str | None = None,
        header_files: list[str] | None = None,
        cpp_extra_args: str | None = None,
        model: str | None = None,
        max_turns: int = 50,
        spec_tracker: SpecTracker | None = None,
        max_parallel: int | None = 8,
        wp_procs: int = 8,
        wp_jobs: int | None = None,
        wp_model: str | None = None,
        ablation: "AblationConfig | None" = None,
        project_context: ProjectContext | None = None,
        scope_kind: SolveScope = "global",
        max_cycles_per_agent: int = DEFAULT_MAX_CYCLES_PER_AGENT,
        on_batch_complete: Callable[[str, list[AgentResult]], None] | None = None,
        run_store: "RunStore | None" = None,
        output_sink: "AgentOutputSink | None" = None,
    ) -> None:
        setup_logging()
        self.bus = AsyncQueueBus()
        self._run_store = run_store
        self.registry = AgentRegistry(run_store=run_store)
        self.timeout = timeout
        self._model = model
        self._max_turns = max_turns
        self._spec_tracker = spec_tracker
        self._max_parallel = max_parallel
        # WP resource knobs (decoupled from agent parallelism); the budget
        # policy lives in framac.wp_resources, applied once in run().
        self._wp_procs = wp_procs
        self._wp_jobs = wp_jobs
        self._wp_model = wp_model
        self._ablation = ablation or AblationConfig()
        if self._ablation.active():
            logger.warning(
                "ABLATION ACTIVE (RQ3): %s disabled",
                ",".join(self._ablation.active()),
            )
        self._header_files = header_files
        self._cpp_extra_args = cpp_extra_args
        self._scope_kind: SolveScope = scope_kind
        self._max_cycles_per_agent = max_cycles_per_agent
        self._on_batch_complete = on_batch_complete
        self._output_sink = output_sink
        self.project_context: ProjectContext | None = project_context

        self._agents: list[FunctionAgent] = []
        self._agents_by_id: dict[str, FunctionAgent] = {}
        self._wakes = WakeQueue(max_cycles_per_agent, run_store)
        # Per-agent assume-debt wake budget: agent_id -> (debt set, attempts
        # used, cycle count at the last counted attempt). See reconcile Part 2.
        self._assume_debt_strikes: dict[str, tuple[frozenset[str], int, int]] = {}

        # Cancel coordination: a background watcher polls cancel.requested
        # every second and sets _cancel_event so the batch race can pick it
        # up immediately. The orchestrator races each batch's gather() with
        # the event so cancel takes effect within ~1s + the time it takes
        # an in-flight agent task to honor task.cancel().
        self._cancel_event: asyncio.Event | None = None

        if source_file is not None:
            self._build_from_source(source_file, functions)
        elif functions is not None:
            self._build_from_functions(functions)
        else:
            raise ValueError("Either functions or source_file must be provided")

    def _register_agent(self, info: FunctionInfo, key_index: int = 0) -> None:
        aid = f"agent_{info.name}"
        entry = AgentEntry(agent_id=aid, function_info=info)
        self.registry.register(entry)
        agent = FunctionAgent(
            aid,
            info,
            self.registry,
            self.bus,
            project_context=self.project_context,
            model=self._model,
            max_turns=self._max_turns,
            spec_tracker=self._spec_tracker,
            output_sink=self._output_sink,
            key_index=key_index,
            ablation=self._ablation,
        )
        self._agents.append(agent)
        self._agents_by_id[aid] = agent

    def _build_from_functions(self, functions: list[FunctionInfo]) -> None:
        """Build directly from provided FunctionInfo objects."""
        from verdict.core.env import get_sdk_env_pool
        pool_size = max(1, len(get_sdk_env_pool()))
        for i, info in enumerate(functions):
            self._register_agent(info, key_index=i % pool_size)

    def _build_from_source(
        self,
        source_file: str,
        extra_functions: list[FunctionInfo] | None = None,
    ) -> None:
        """Build ProjectContext and agents from a source file."""
        from verdict.preprocessing.pipeline import preprocess

        self.project_context, _ecg = preprocess(
            source_file,
            header_files=self._header_files,
            cpp_extra_args=self._cpp_extra_args,
        )
        functions = self._functions_from_call_graph(self.project_context)
        if extra_functions:
            existing = {f.name for f in functions}
            for info in extra_functions:
                if info.name not in existing:
                    functions.append(info)

        from verdict.core.env import get_sdk_env_pool
        pool_size = max(1, len(get_sdk_env_pool()))
        for i, info in enumerate(functions):
            self._register_agent(info, key_index=i % pool_size)

    def _functions_from_call_graph(
        self,
        project_context: ProjectContext,
    ) -> list[FunctionInfo]:
        """Build FunctionInfo objects from the call graph with caller/callee edges.

        Extern prototypes (no body) are environment, not verification
        targets — they keep their graph node but get no FunctionInfo/agent.
        """
        return [
            _build_function_info(name, project_context)
            for name in project_context.call_graph_dict
            if name not in project_context.extern_functions
        ]

    async def seed_from_previous_run(
        self,
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
        await seeding.seed_from_previous_run(
            self.registry,
            self._agents_by_id,
            self._wakes,
            prev_run,
            restore_sessions,
        )

    def _publication_requires_recheck(self, event: SpecPublished) -> bool:
        if not event.changed_kinds:
            return True
        impactful = {"precondition", "postcondition", "assigns", "published"}
        return any(kind in impactful for kind in event.changed_kinds)

    # ── Three-phase fixpoint engine ─────────────────────────────────────
    #
    # Phase 1: reconcile — drain events, mutate facts (VOs, dep snapshots)
    # Phase 2: derive   — recompute all proof states from facts
    # Phase 3: schedule — enqueue every actionable non-terminal agent
    #
    # State computation (recompute) is the sole authority for proof_state.
    # Reconciliation only changes the inputs to that computation; it never
    # writes proof_state or enqueues agents. Scheduling reads the computed
    # states and decides who runs next.

    # ── Phase 1: reconcile events ────────────────────────────────────────

    def _reconcile_dependency_staleness(self) -> None:
        """Unified dependency-staleness reconcile: publish events + assume debt.

        Two parts, run sequentially in one pass:

        **Part 1 — event-driven (callee published a new version):**
        For each publish event × affected caller: discharge (advance snapshot)
        when the dependency judge proves the caller unaffected, or invalidate
        VOs + record a ``spec_published`` reason otherwise.

        **Part 2 — state-driven (assume_callee debt):**
        For every agent whose published spec has ``assumed_callees`` with
        ``based_on_versions[callee] = 0`` (the callee was verified under an
        explicit hypothesis, not its real spec) AND the callee IS published
        AND Part 1 did not already produce a reason for this agent: record
        an ``assume_callee_debt`` reason guiding the agent to discharge (re-
        verify without the assumption, or demand the callee).

        Each debt set carries a wake budget of ``ASSUME_DEBT_CYCLE_LIMIT``
        attempts (counted only when the agent actually ran a cycle since the
        previous attempt). An exhausted budget settles the debt: no further
        debt wakes, the unit stays LOCAL_OK, and the assumption is reported
        as a residual hypothesis. Publish-event wakes (Part 1) are not
        suppressed, so a later callee republish can still discharge a
        settled debt — the settled mark is then cleared here.

        Neither part writes proof_state or enqueues agents — reconcile only
        mutates facts and records reasons; derive + schedule do the rest.
        """
        from verdict.orchestrator.dependency_judge import (
            judge_dependency_change,
            judge_precondition_change,
        )

        # Track which agents already received a reason in Part 1, so Part 2
        # does not duplicate.
        touched_agents: set[str] = set()

        # ── Part 1: drain publish events ────────────────────────────────
        for event in self.registry.pop_publish_events():
            if not self._publication_requires_recheck(event):
                continue

            source_entry = self.registry.get_by_function(event.function_name)
            if source_entry is None:
                continue

            callee_pub = self.registry.read_spec(event.function_name)

            for caller_name in source_entry.function_info.callers:
                caller_entry = self.registry.get_by_function(caller_name)
                if caller_entry is None:
                    continue

                post_affected = judge_dependency_change(
                    caller_entry.published_spec, event.function_name, callee_pub
                )
                # Frame conditions are load-bearing for WP separation
                # reasoning, but measured spec_deps record postcondition
                # clauses only — the judge cannot see an assigns change.
                # Any assigns change is AFFECTED for measured dependants.
                if event.assigns_changed:
                    post_affected = True
                pre_affected = judge_precondition_change(
                    caller_entry.published_spec, event.function_name, callee_pub
                )
                if not post_affected and not pre_affected:
                    # Never advance an assume anchor (based_on == 0): the
                    # judge's verdict rests on spec_deps recorded under the
                    # caller's hypothesis world, so advancing would silently
                    # discharge the assume debt without any re-verification.
                    # Discharge is owned by the debt machinery (Part 2).
                    anchored = (
                        caller_entry.published_spec is not None
                        and caller_entry.published_spec.based_on_versions.get(
                            event.function_name
                        ) == 0
                    )
                    if not anchored:
                        self.registry.advance_dependency_snapshot(
                            caller_name, event.function_name, event.version
                        )
                    # The call-site proof is still valid (unaffected), but the
                    # callee's precondition COUNT may have changed (e.g. a
                    # precondition was removed). Sync the child VOs so stale
                    # children for removed preconditions don't remain as
                    # permanently undischargeable orphans. Since the proof is
                    # unaffected, children whose precondition text is unchanged
                    # keep their verified status.
                    if event.precondition_changed:
                        caller_agent = self._agents_by_id.get(caller_entry.agent_id)
                        if caller_agent is not None and caller_agent.vo_tracker is not None:
                            self._sync_callsite_children(
                                caller_agent.vo_tracker, event.function_name
                            )
                    continue

                self._invalidate_caller_vos(caller_entry.agent_id, event)
                self._pending_reasons.setdefault(caller_entry.agent_id, []).append(
                    WakeReason(
                        reason="spec_published",
                        source_function=event.function_name,
                        source_version=event.version,
                        changed_kinds=event.changed_kinds,
                        details=(
                            f"{event.function_name} published v{event.version}"
                        ),
                    ),
                )
                touched_agents.add(caller_entry.agent_id)

        # ── Part 2: assume-callee debt (state-driven) ──────────────────
        for agent in self._agents:
            if agent.agent_id in touched_agents:
                continue
            debts = undischarged_assumptions(
                self.registry, agent.function_info.name
            )
            settled = self.registry.settled_assumes(agent.agent_id)
            # Ratchet: a settled debt that no longer shows up was discharged
            # (the callee republished something sufficient) — clear its mark.
            # Extern marks are permanent: an environment axiom has no
            # publisher, so absence from the debt scan means nothing.
            stale_marks = (
                settled
                - set(debts)
                - set(agent.function_info.extern_callees)
            )
            if stale_marks:
                self.registry.clear_settled_assumes(agent.agent_id, stale_marks)
                settled -= stale_marks
            open_debts = [d for d in debts if d not in settled]
            if not open_debts:
                continue

            # Wake budget: count an attempt only when the agent has run a
            # cycle since the previous one (re-appends while it is suspended
            # or waiting do not burn budget). A changed debt set resets it.
            key = frozenset(open_debts)
            cycles = self._wakes.cycle_count(agent.agent_id)
            prev = self._assume_debt_strikes.get(agent.agent_id)
            if prev is None or prev[0] != key:
                strikes, seen_cycles = 1, cycles
            elif cycles > prev[2]:
                strikes, seen_cycles = prev[1] + 1, cycles
            else:
                strikes, seen_cycles = prev[1], prev[2]
            self._assume_debt_strikes[agent.agent_id] = (key, strikes, seen_cycles)
            if strikes > ASSUME_DEBT_CYCLE_LIMIT:
                self.registry.settle_assumes(agent.agent_id, open_debts)
                self._assume_debt_strikes.pop(agent.agent_id, None)
                continue

            callee_versions = {
                name: self.registry.read_spec(name).version
                for name in open_debts
                if self.registry.read_spec(name) is not None
            }
            details_parts = []
            for name in open_debts:
                cv = callee_versions.get(name)
                details_parts.append(
                    f"{name} (your based_on=0, published v{cv})"
                )
            remedy = (
                "the property may be unprovable in this run"
                if self._ablation.demands
                else "submit a proof demand for the property you assumed"
            )
            self._pending_reasons.setdefault(agent.agent_id, []).append(
                WakeReason(
                    reason="assume_callee_debt",
                    details=(
                        f"published under assume_callee on "
                        f"{', '.join(details_parts)} — re-verify WITHOUT "
                        f"assume_callee against their real published specs; "
                        f"if insufficient, {remedy} "
                        f"(attempt {strikes}/{ASSUME_DEBT_CYCLE_LIMIT})"
                    ),
                ),
            )

    def _invalidate_caller_vos(
        self,
        caller_agent_id: str,
        event: SpecPublished,
    ) -> None:
        """Reset caller VOs and rebuild call-site children (fact mutation only)."""
        from verdict.core.vo_tracker import VOStatus

        caller_agent = self._agents_by_id.get(caller_agent_id)
        if caller_agent is None or caller_agent.vo_tracker is None:
            return
        tracker = caller_agent.vo_tracker

        invalidate_all = (
            event.postcondition_weakened_or_modified or event.assigns_changed
        )
        invalidate_callsite_for_callee = (
            event.precondition_changed
            or "published" in (event.changed_kinds or [])
        )

        if invalidate_all:
            for vo_id, vo in list(tracker.rte_vos.items()):
                if vo.status != VOStatus.PENDING:
                    tracker.mark_pending(vo_id)
            for vo_id, vo in list(tracker.call_site_vos.items()):
                if vo.status != VOStatus.PENDING:
                    tracker.mark_pending(vo_id)
            tracker.invalidate_spec(
                f"callee {event.function_name} published v{event.version} "
                f"with changed guarantees [{', '.join(event.changed_kinds or [])}] "
                f"— your postconditions, assigns, and loop contracts may no "
                f"longer hold; re-run verify_function_contract"
            )
            self._invalidate_spec_deps(caller_agent_id, event.function_name)
            self._rebuild_callsite_children(tracker, event.function_name)
            return

        if invalidate_callsite_for_callee:
            self._rebuild_callsite_children(tracker, event.function_name)

    def _invalidate_spec_deps(
        self,
        caller_agent_id: str,
        callee_function: str,
    ) -> None:
        """Clear the caller's spec_deps entry for a callee whose guarantees changed.

        When a callee's postconditions are weakened or modified, the caller's
        recorded dependency (spec_deps[callee]) — which clauses the caller's
        proof relied on — is stale: the clauses may no longer exist or may have
        changed meaning. Clearing forces the dependency judge to fall back to
        ALL (conservative: any future callee change wakes the caller) until the
        caller re-runs build_spec_deps after re-verification.
        """
        entry = self.registry.get(caller_agent_id)
        if entry is None:
            return
        for spec in (entry.published_spec, entry.spec):
            if spec is not None and callee_function in spec.spec_deps:
                del spec.spec_deps[callee_function]

    def _sync_callsite_children(
        self,
        tracker: Any,
        callee_function: str,
    ) -> None:
        """Sync child VOs for call-site parents targeting *callee_function*,
        preserving verified status (advance path — the proof is still valid).

        Twin of ``_rebuild_callsite_children``: both walk the caller's parent
        call-site VOs for *callee_function* and delegate to the tracker.
        ``rebuild`` resets every child to PENDING (invalidate path); ``sync``
        keeps the status of children whose precondition text is unchanged
        (advance path), so only children for removed/changed preconditions are
        reset or pruned.
        """
        callee_spec = self.registry.read_spec(callee_function)
        preconditions = (
            list(callee_spec.preconditions)
            if callee_spec and callee_spec.preconditions
            else []
        )

        for vo_id, vo in list(tracker.parent_call_site_vos().items()):
            if vo.related_function != callee_function:
                continue
            tracker.sync_callsite_children(vo_id, preconditions)

    def _rebuild_callsite_children(
        self,
        tracker: Any,
        callee_function: str,
    ) -> None:
        """Rebuild child VOs for call-site parents targeting *callee_function*."""
        callee_spec = self.registry.read_spec(callee_function)
        preconditions = (
            list(callee_spec.preconditions)
            if callee_spec and callee_spec.preconditions
            else []
        )

        for vo_id, vo in list(tracker.parent_call_site_vos().items()):
            if vo.related_function != callee_function:
                continue
            tracker.rebuild_callsite_children(vo_id, preconditions)

    def _reconcile_demand_protocol(self) -> None:
        """Drain demand events and maintain suspend/release state.

        Records wake reasons but does NOT enqueue — scheduling is
        state-driven. Suspend/release is derived from the current set
        of open demands (not from individual events).
        """
        # Target notification — DURABLE. ``_is_actionable`` keeps a target
        # actionable for as long as a demand targets it (the durable open
        # store); the wake reason must mirror that. ``pop_new`` only fires
        # once, at raise — so a target re-woken on a later batch (because the
        # demand is still open) would otherwise carry no demand reason and
        # fall through to the generic _diagnose_incomplete fallback. Instead
        # we re-derive a ``proof_demand`` reason every batch from the open
        # store. Idempotent: drop any prior proof_demand reason first, so a
        # target that is NOT scheduled this batch (e.g. it is itself
        # suspended) does not accumulate duplicates. Full per-demand detail
        # (property_text, rationale, blocking VOs) is rendered separately by
        # the cycle prompt from the same durable store.
        self.registry.pop_new_proof_demands()  # drain; durable store is the source
        for reasons in self._pending_reasons.values():
            reasons[:] = [r for r in reasons if r.reason != "proof_demand"]
        for agent in self._agents:
            incoming = self.registry.open_proof_demands(agent.function_info.name)
            if not incoming:
                continue
            requesters = sorted({
                (
                    self.registry.get(d.requester_id).function_info.name
                    if self.registry.get(d.requester_id) is not None
                    else d.requester_id
                )
                for d in incoming
            })
            details = (
                f"{len(incoming)} open demand(s) await your action (from "
                f"{', '.join(requesters)}) — resolve EACH via "
                f"resolve_proof_demand (satisfied if your published spec "
                f"already provides it, else add the clause + republish then "
                f"resolve). Requesters stay blocked until you do"
            )
            self._pending_reasons.setdefault(agent.agent_id, []).append(
                WakeReason(reason="proof_demand", details=details),
            )

        for event in self.registry.pop_resolved_demands():
            demand = event.demand
            requester = self.registry.get(demand.requester_id)
            if requester is None:
                continue
            status_str = demand.status.value
            details = f"{demand.property_kind}: {demand.property_text}"
            if demand.resolution_message:
                details += f" :: {demand.resolution_message}"
            self._pending_reasons.setdefault(demand.requester_id, []).append(
                WakeReason(
                    reason=f"demand_{status_str}",
                    source_function=event.resolver_function,
                    details=details,
                ),
            )

        # Derive suspend state from current open demands (single source).
        currently_waiting: set[str] = set()
        for agent in self._agents:
            aid = agent.agent_id
            outstanding = self.registry.open_proof_demands_submitted_by(aid)
            if outstanding:
                targets = {d.target_function for d in outstanding}
                self._wakes.suspend(aid, targets, len(outstanding))
                currently_waiting.add(aid)
        for aid in self._wakes.suspended_agents() - currently_waiting:
            self._wakes.clear_suspension(aid)

    # ── Phase 2: derive all proof states ─────────────────────────────────

    def _derive_all_states(self) -> None:
        """Recompute every agent's proof state to a fixpoint.

        Iterated because GLOBAL_OK propagates along caller→callee chains.
        """
        changed = True
        while changed:
            changed = False
            for agent in self._agents:
                before = self.registry.get_proof_state(agent.agent_id)
                recompute_proof_state(
                    self.registry, self._agents_by_id, self._scope_kind,
                    agent.agent_id,
                )
                if self.registry.get_proof_state(agent.agent_id) != before:
                    changed = True

    # ── Phase 3: state-driven scheduling ─────────────────────────────────

    def _is_actionable(self, agent: Any) -> bool:
        """True if *agent* should be given another cycle."""
        entry = self.registry.get(agent.agent_id)
        if entry is None:
            return False
        if self._wakes.is_cycle_limited(agent.agent_id):
            return False

        # An unresolved incoming proof demand wakes the target REGARDLESS of
        # its state — including suspended (mutual demands between A and B
        # would otherwise deadlock: both suspended waiting on each other) and
        # FAILED (the requester stays suspended forever on a dead target).
        # Keyed on the DURABLE demand store, not on transient wake reasons.
        if self.registry.open_proof_demands(entry.function_info.name):
            return True

        if self._wakes.is_suspended(agent.agent_id):
            return False

        ps = entry.proof_state

        if ps == ProofState.FAILED:
            # FAILED records a runner error (often transient: SDK death,
            # API blip). It is cleared by the next CLEAN cycle — so the
            # unit must stay schedulable or one transient error parks it
            # forever and wedges every caller's GLOBAL_OK. Persistent
            # failures are bounded by the cycle limit above.
            return True

        if ps == ProofState.GLOBAL_OK:
            # A resolution to a demand THIS unit submitted is a one-shot
            # event with follow-up work attached (e.g. it promised to add
            # a clause once the upstream guarantee landed). Open incoming
            # demands keep a GLOBAL_OK unit actionable via the durable
            # check above; the outgoing-resolution wake is transient and
            # must not be dropped by the state gate — otherwise a satisfied
            # upstream demand never completes its chain and the original
            # requester spins on an assumption whose key was already
            # published two levels up.
            return any(
                r.reason in ("demand_satisfied", "demand_rejected")
                for r in self._pending_reasons.get(agent.agent_id, [])
            )
        if self._scope_kind == "functions" and ps == ProofState.LOCAL_OK:
            return False
        if ps == ProofState.LOCAL_OK:
            # An undischarged assume_callee debt keeps a LOCAL_OK unit
            # actionable so it can be woken to discharge (re-verify without
            # assume, or demand the callee) and reach GLOBAL_OK. A unit that
            # already submitted a demand is suspended above, so this only
            # catches the assume-without-demand case.
            debts = undischarged_assumptions(
                self.registry, entry.function_info.name
            )
            settled = self.registry.settled_assumes(agent.agent_id)
            if set(debts) - settled:
                return True
            # A resolution to a demand THIS unit submitted carries follow-up
            # work even when no assume debt exists (e.g. LOCAL_OK because a
            # sibling callee is untrusted) — same one-shot rescue as the
            # GLOBAL_OK gate, or the satisfied-without-republish case rots.
            if any(
                r.reason in ("demand_satisfied", "demand_rejected")
                for r in self._pending_reasons.get(agent.agent_id, [])
            ):
                return True
            # Debts all settled (wake budget spent): the unit rests —
            # otherwise _schedule_actionable's generic "continue" filler
            # would re-wake it forever, defeating the settle. First-class
            # reasons recorded by reconcile (publish events) still fire, so
            # a late callee republish can discharge a settled assumption.
            return bool(debts and self._pending_reasons.get(agent.agent_id))

        return True

    def _diagnose_incomplete(self, agent: Any) -> str:
        """Last-resort fallback reason when no collector recorded anything.

        Describes the agent's own standing state so it can infer what is
        left to do. First-class signals (publish events, proof demands,
        demand resolutions) are surfaced upstream by the reconcile
        collectors; this only runs when none of them applied.
        """
        entry = self.registry.get(agent.agent_id)
        vt = agent.vo_tracker

        has_draft = entry is not None and entry.spec is not None
        spec_verified = (
            vt.spec_vo is not None and vt.spec_vo.status.value == "verified"
        ) if vt else False

        from verdict.core.vo_tracker import VOStatus
        rte_pending = sum(
            1 for v in vt.rte_vos.values() if v.status == VOStatus.PENDING
        ) if vt else 0
        cs_pending = sum(
            1 for v in vt.leaf_call_site_vos().values()
            if v.status == VOStatus.PENDING
        ) if vt else 0

        if has_draft and spec_verified and rte_pending == 0 and cs_pending == 0:
            return (
                "all VOs verified and spec drafted — call "
                "update_my_spec(status='published') to finish"
            )
        parts = []
        if not has_draft:
            parts.append("no draft yet")
        elif not spec_verified:
            parts.append("spec not verified")
        if rte_pending:
            parts.append(f"{rte_pending} RTE VOs pending")
        if cs_pending:
            parts.append(f"{cs_pending} call-site VOs pending")
        return "; ".join(parts) if parts else "incomplete work"

    def _schedule_actionable(self) -> None:
        """Enqueue every actionable agent, merging accumulated reasons."""
        for agent in self._agents:
            if not self._is_actionable(agent):
                continue
            reasons = self._pending_reasons.pop(agent.agent_id, None)
            if reasons:
                for r in reasons:
                    self._wakes.enqueue(agent.agent_id, r)
            if agent.agent_id not in self._wakes.pending():
                self._wakes.enqueue(
                    agent.agent_id,
                    WakeReason(
                        reason="continue",
                        details=self._diagnose_incomplete(agent),
                    ),
                )

    def _remaining_timeout(self, deadline: float) -> float:
        return max(0.0, deadline - asyncio.get_running_loop().time())

    async def _cancel_watcher(self) -> None:
        """Background task: poll cancel.requested and set _cancel_event."""
        if self._run_store is None or self._cancel_event is None:
            return
        try:
            while not self._cancel_event.is_set():
                if self._run_store.cancel_requested():
                    self._cancel_event.set()
                    return
                await asyncio.sleep(1.0)
        except asyncio.CancelledError:
            return

    async def _run_batch_with_cancel(
        self,
        tasks: list[asyncio.Task],
        deadline: float,
    ) -> bool:
        """Race batch tasks against the cancel event.

        Returns True if the batch finished normally, False if it was
        interrupted by a cancel. Either way, by the time this returns the
        batch tasks are done or cancelled.

        Raises asyncio.TimeoutError if the deadline elapses before either
        completion or cancellation.
        """
        if not tasks:
            return True

        timeout = self._remaining_timeout(deadline)
        # asyncio.gather returns a Future-like that asyncio.wait accepts.
        gather_future = asyncio.gather(*tasks, return_exceptions=True)
        wait_set: set = {gather_future}

        cancel_wait: asyncio.Task | None = None
        if self._cancel_event is not None:
            cancel_wait = asyncio.ensure_future(self._cancel_event.wait())
            wait_set.add(cancel_wait)

        done, _pending = await asyncio.wait(
            wait_set,
            timeout=timeout if timeout > 0 else None,
            return_when=asyncio.FIRST_COMPLETED,
        )

        if not done:
            # Outer timeout fired with no completion
            for task in tasks:
                task.cancel()
            gather_future.cancel()
            if cancel_wait is not None:
                cancel_wait.cancel()
            raise asyncio.TimeoutError()

        # Cancel was requested → tear down the batch tasks immediately
        if cancel_wait is not None and cancel_wait in done:
            for task in tasks:
                if not task.done():
                    task.cancel()
            try:
                await gather_future
            except (asyncio.CancelledError, BaseException):
                pass
            return False

        # Normal completion: drain any cancel waiter
        if cancel_wait is not None and not cancel_wait.done():
            cancel_wait.cancel()
            try:
                await cancel_wait
            except asyncio.CancelledError:
                pass
        return True

    def _batch_limit(self) -> int:
        if self._max_parallel is None:
            return max(1, len(self._agents))
        return max(1, self._max_parallel)

    def _emit_cycle_ended(self, agent_id: str, phase: str) -> None:
        recompute_proof_state(
            self.registry, self._agents_by_id, self._scope_kind, agent_id
        )
        snapshots = reporting.snapshot_batch(
            self._agents_by_id, self._wakes, [agent_id], registry=self.registry
        )
        if self._run_store is not None:
            for snap in snapshots:
                self._run_store.append_event(
                    "agent_cycle_ended",
                    data={
                        "cycle": snap.cycle_count,
                        "phase": phase,
                        "proof_state": snap.proof_state.value if snap.proof_state else None,
                        "error": snap.error,
                        "vo_summary": (
                            asdict(snap.vo_summary)
                            if snap.vo_summary is not None
                            else None
                        ),
                    },
                    agent_id=snap.agent_id,
                )
            self._persist_live_state()
        if self._on_batch_complete is not None:
            try:
                self._on_batch_complete(phase, snapshots)
            except Exception:
                logger.exception("on_batch_complete callback failed")

    async def _run_pool(
        self,
        starters: list[tuple[str, Callable[[], Any]]],
        phase: str,
        deadline: float,
    ) -> None:
        """Run a worker pool over (agent_id, coroutine_factory) pairs.

        At most `_batch_limit()` agents run concurrently. As soon as one
        finishes, the next pending agent is picked up — no batch barriers.
        Each agent emits its own cycle_ended event independently.
        """
        if not starters:
            return
        sem = asyncio.Semaphore(self._batch_limit())

        async def _run_one(agent_id: str, make_coro: Callable[[], Any]) -> None:
            async with sem:
                if self._cancel_event is not None and self._cancel_event.is_set():
                    return
                try:
                    await make_coro()
                finally:
                    self._emit_cycle_ended(agent_id, phase)

        tasks = [
            asyncio.create_task(_run_one(aid, factory))
            for aid, factory in starters
        ]
        await self._run_batch_with_cancel(tasks, deadline)

    async def _run_initial_batch(self, deadline: float) -> None:
        """Launch every agent under bounded concurrency for the initial solve."""
        if not self._agents:
            return
        logger.info("Starting initial solve for %d agents", len(self._agents))

        starters: list[tuple[str, Callable[[], Any]]] = []
        for agent in self._agents:
            entry = self.registry.get(agent.agent_id)
            if entry and entry.proof_state in (ProofState.LOCAL_OK, ProofState.GLOBAL_OK):
                logger.info(
                    "Skipping %s (restored %s from previous run)",
                    agent.agent_id, entry.proof_state.value,
                )
                continue
            cycle = self._wakes.mark_cycle_started(agent.agent_id)
            if self._run_store is not None:
                self._run_store.append_event(
                    "agent_cycle_started",
                    data={
                        "cycle": cycle,
                        "phase": "initial",
                        "wake_reasons": [],
                    },
                    agent_id=agent.agent_id,
                )
            starters.append((agent.agent_id, agent.run))

        await self._run_pool(starters, "initial", deadline)

    async def _run_reactivation_batch(
        self,
        deadline: float,
        user_messages: dict[str, str] | None = None,
    ) -> None:
        """Run all queued wake-ups under bounded concurrency."""
        if user_messages is None:
            user_messages = {}
        ready_items = self._wakes.pop_actionable()

        starters: list[tuple[str, Callable[[], Any]]] = []
        for agent_id, reasons in ready_items:
            agent = self._agents_by_id[agent_id]
            cycle = self._wakes.mark_cycle_started(agent_id)
            logger.info(
                "Reactivating %s for %d wake reasons (cycle %d/%d)",
                agent_id,
                len(reasons),
                cycle,
                self._max_cycles_per_agent,
            )
            if self._run_store is not None:
                self._run_store.append_event(
                    "agent_cycle_started",
                    data={
                        "cycle": cycle,
                        "phase": "reactivation",
                        "wake_reasons": [
                            {
                                "reason": r.reason,
                                "source_function": r.source_function,
                                "source_version": r.source_version,
                                "changed_kinds": list(r.changed_kinds),
                                "details": r.details,
                            }
                            for r in reasons
                        ],
                    },
                    agent_id=agent_id,
                )
            umsg = user_messages.get(agent_id)
            captured_reasons = reasons
            starters.append((
                agent_id,
                lambda a=agent, r=captured_reasons, m=umsg: a.reactivate(
                    r, user_message=m
                ),
            ))

        await self._run_pool(starters, "reactivation", deadline)

    def _persist_live_state(self) -> None:
        """Refresh run.json with the latest per-agent snapshots mid-run.

        This keeps run.json's per-agent state current mid-run instead of an
        empty agents map until finalize, so live consumers and post-run
        inspection see progress. Called after each initial and reactivation
        batch.
        """
        if self._run_store is None:
            return
        snapshots = reporting.snapshot_all(
            self._agents_by_id, self._wakes, registry=self.registry
        )
        self._run_store.record.agents = {
            snap.function_name: snap for snap in snapshots
        }
        self._run_store.record.summary = reporting.compute_summary(snapshots)
        self._run_store.save_record()

    def _emit_final_annotated_file(self, results: list[AgentResult]) -> None:
        """Assemble a complete annotated C file from final published specs.

        Includes headers, type declarations, and all function bodies with
        their verified ACSL contracts — ready for standalone Frama-C WP.
        """
        if self.project_context is None or self._run_store is None:
            return
        try:
            import copy
            from verdict.annotation.function_annotator import FunctionAnnotator
            from verdict.postprocessing.cil_assembler import AnnotatedCILFileAssembler
            from verdict.tools._cil_builder import _apply_annotations_to_node

            ctx = self.project_context
            call_graph = ctx.call_graph_dict
            annotator = FunctionAnnotator(ctx.function_declarations_map)

            annotated_functions: list[str] = []
            for func_name, node in sorted(call_graph.items(), key=lambda kv: kv[1].extent[0]):
                node_copy = copy.deepcopy(node)
                spec = self.registry.read_spec(func_name)
                if spec is not None:
                    _apply_annotations_to_node(node_copy, {
                        "preconditions": list(spec.preconditions),
                        "postconditions": list(spec.postconditions),
                        "assigns": list(spec.assigns),
                        "loop_contracts": dict(spec.loop_contracts),
                    })
                ok, body = annotator.get_function_with_annotations(node_copy)
                if ok and body:
                    annotated_functions.append(body)

            out_path = str(self._run_store.run_dir / "final_annotated.c")
            assembler = AnnotatedCILFileAssembler()
            assembler.assemble_annotated_cil(
                list(ctx.cil_headers),
                list(ctx.ordered_declarations),
                annotated_functions,
                out_path,
            )
            logger.info("Final annotated file: %s", out_path)
        except Exception:
            logger.warning("Failed to emit final annotated file", exc_info=True)

    def _finalize_results(self) -> list[AgentResult]:
        """Derive final proof states and return snapshots.

        Runs ``_derive_all_states`` one last time (GLOBAL_OK propagates along
        caller→callee chains). Then renders blocked reasons for LOCAL_OK
        agents as events (reporting only, not proof-state mutation).
        """
        self._derive_all_states()

        for agent in self._agents:
            if (
                self._scope_kind != "global"
                or self.registry.get_proof_state(agent.agent_id) != ProofState.LOCAL_OK
            ):
                continue
            untrusted = untrusted_callees(self.registry, agent.function_info.name)
            if untrusted:
                reason = "blocked at finalize: untrusted callees: " + ", ".join(untrusted)
                if self._run_store is not None:
                    self._run_store.append_event(
                        "agent_blocked",
                        data={"reason": reason},
                        agent_id=agent.agent_id,
                    )
            # FAILED needs no extra bookkeeping — proof_state is the source of
            # truth; "blocked" is derived (proof_state == FAILED).

        results: list[AgentResult] = []
        for agent in self._agents:
            result = agent.snapshot_result()
            result.cycle_count = self._wakes.cycle_count(agent.agent_id)
            result.cycle_limit_reached = self._wakes.is_cycle_limited(agent.agent_id)
            result.vo_summary = reporting.build_vo_summary(agent)
            result.pending_wakes = self._wakes.pending_wakes(agent.agent_id)
            result.callees = list(agent.function_info.callees)
            # Split settled assumes at the source: extern callees are
            # standing environment axioms (no publisher can ever exist),
            # everything else is an agent-callee debt whose wake budget
            # ran out. Reporters consume the split, never re-derive it.
            settled = sorted(self.registry.settled_assumes(agent.agent_id))
            extern = set(agent.function_info.extern_callees)
            result.settled_assumes = [a for a in settled if a not in extern]
            result.extern_axioms = [a for a in settled if a in extern]
            results.append(result)
        return results

    async def run(
        self,
        wake_selector: Callable[..., Any] | None = None,
    ) -> list[AgentResult]:
        """Run initial solve, then auto-wake until no more work remains.

        If *wake_selector* is provided (interactive mode), it is called
        after each event processing round with the pending ``_ready``
        dict. It must be an async callable returning
        ``(selected_ids, user_messages)`` or ``None`` to quit.
        When omitted, all pending agents are auto-selected (solve mode).
        """
        from verdict.framac import wp_resources
        procs, jobs = wp_resources.configure(
            self._wp_procs, self._wp_jobs, model=self._wp_model
        )
        logger.info(
            "WP resources: wp-procs=%d × wp-jobs=%d (≈%d cores peak); "
            "agent max_parallel=%s%s",
            procs, jobs, procs * jobs, self._max_parallel,
            f"; wp-model={self._wp_model}" if self._wp_model else "",
        )

        deadline = asyncio.get_running_loop().time() + self.timeout
        self._cancel_event = asyncio.Event()
        watcher_task: asyncio.Task | None = None
        if self._run_store is not None:
            # Start the cancel watcher only when we have a run_store to poll.
            watcher_task = asyncio.create_task(self._cancel_watcher())

        if self._run_store is not None:
            self._run_store.set_status(RunStatus.RUNNING)
            call_graph = {
                agent.function_info.name: list(agent.function_info.callees)
                for agent in self._agents
            }
            self._run_store.append_event(
                "run_started",
                data={
                    "agent_count": len(self._agents),
                    "agent_ids": [a.agent_id for a in self._agents],
                    "function_names": [a.function_info.name for a in self._agents],
                    "scope": self._scope_kind,
                    "timeout": self.timeout,
                    "max_cycles_per_agent": self._max_cycles_per_agent,
                    "call_graph": call_graph,
                },
            )
            # Seed the record with empty-state agents so run.json reflects
            # the full agent set before the first cycle completes.
            self._persist_live_state()

        cancelled = False
        timed_out = False
        aborted_error: BaseException | None = None
        try:
            try:
                self._pending_reasons: dict[str, list[WakeReason]] = {}
                await self._run_initial_batch(deadline)
                if self._cancel_event.is_set():
                    cancelled = True
                    if self._run_store is not None:
                        self._run_store.append_event(
                            "run_cancelled", data={"phase": "initial"}
                        )
                while not cancelled:
                    self._reconcile_dependency_staleness()
                    self._reconcile_demand_protocol()
                    self._derive_all_states()
                    self._schedule_actionable()
                    actionable = self._wakes.actionable()
                    if not actionable:
                        if wake_selector is None:
                            break
                        # Interactive mode: let the selector decide even
                        # when there are no pending wakes (user might want
                        # to manually trigger an agent or quit).
                        selection = await wake_selector(self._wakes.pending())
                        if selection is None:
                            break
                        selected_ids, user_messages = selection
                        if not selected_ids:
                            continue
                        # Manually enqueue selected agents (user-initiated)
                        for aid in selected_ids:
                            self._wakes.ensure_present(aid)
                        await self._run_reactivation_batch(
                            deadline, user_messages=user_messages,
                        )
                        continue

                    if wake_selector is not None:
                        selection = await wake_selector(self._wakes.pending())
                        if selection is None:
                            break
                        selected_ids, user_messages = selection
                        if not selected_ids:
                            continue
                        # Keep only the selected agents; restore the rest after.
                        deferred = self._wakes.select_only(set(selected_ids))
                        await self._run_reactivation_batch(
                            deadline, user_messages=user_messages,
                        )
                        self._wakes.restore(deferred)
                    else:
                        await self._run_reactivation_batch(deadline)

                    if self._cancel_event.is_set():
                        cancelled = True
                        logger.warning("Cancel requested — exiting solve loop")
                        if self._run_store is not None:
                            self._run_store.append_event(
                                "run_cancelled", data={"phase": "loop"}
                            )
                        break

            except asyncio.TimeoutError:
                timed_out = True
                logger.error("Orchestrator timed out after %.1fs", self.timeout)
                if self._run_store is not None:
                    self._run_store.append_event(
                        "run_timeout", data={"timeout": self.timeout}
                    )
            except (KeyboardInterrupt, asyncio.CancelledError) as exc:
                cancelled = True
                aborted_error = exc
                logger.warning("Solve aborted: %s", type(exc).__name__)
                if self._run_store is not None:
                    self._run_store.append_event(
                        "run_cancelled",
                        data={"phase": "abort", "reason": type(exc).__name__},
                    )
            except Exception as exc:
                aborted_error = exc
                logger.exception("Orchestrator failed with an unhandled exception")
                if self._run_store is not None:
                    self._run_store.append_event(
                        "run_failed",
                        data={"error": repr(exc)},
                    )
        finally:
            if watcher_task is not None:
                watcher_task.cancel()
                try:
                    await watcher_task
                except (asyncio.CancelledError, Exception):
                    pass

        # Always attempt to finalize — even on abort — so run.json lands
        # in a terminal state and agents get final snapshots.
        try:
            results = self._finalize_results()
        except Exception:
            logger.exception("Finalize failed; writing minimal state")
            results = [agent.snapshot_result() for agent in self._agents]

        if self._run_store is not None:
            try:
                self._run_store.finalize(results)
                # Replace the basic summary with the richer per-state breakdown.
                self._run_store.record.summary = reporting.compute_summary(results)
                if isinstance(aborted_error, Exception) and not isinstance(
                    aborted_error, (KeyboardInterrupt, asyncio.CancelledError)
                ):
                    terminal = RunStatus.FAILED
                elif timed_out:
                    terminal = RunStatus.FAILED
                elif cancelled:
                    terminal = RunStatus.CANCELLED
                else:
                    terminal = RunStatus.COMPLETED
                self._run_store.set_status(terminal)
                self._run_store.append_event(
                    "run_completed",
                    data={
                        "status": terminal.value,
                        "summary": dict(self._run_store.record.summary),
                    },
                )
                self._emit_final_annotated_file(results)
            except Exception:
                logger.exception("Failed to persist run terminal state")

        if isinstance(aborted_error, KeyboardInterrupt):
            # Propagate after cleanup so the shell shows the user their
            # Ctrl-C took effect, but run.json is already in terminal state.
            raise aborted_error

        logger.info("Finished: %d results", len(results))
        for result in results:
            marker = " [cycle limit]" if result.cycle_limit_reached else ""
            if result.settled_assumes:
                marker += (
                    " [conditional: assumes "
                    + ", ".join(result.settled_assumes)
                    + "]"
                )
            if result.extern_axioms:
                marker += (
                    " [env axioms: "
                    + ", ".join(result.extern_axioms)
                    + "]"
                )
            vo_part = ""
            if result.vo_summary is not None and result.vo_summary.total > 0:
                s = result.vo_summary
                rte_total = s.rte_verified + s.rte_pending + s.rte_failed + s.rte_bug
                cs_total = (
                    s.callsite_verified + s.callsite_pending
                    + s.callsite_failed + s.callsite_bug
                )
                vo_part = (
                    f" VOs: rte {s.rte_verified}/{rte_total}"
                    f" cs {s.callsite_verified}/{cs_total}"
                )
                if s.rte_bug or s.callsite_bug:
                    vo_part += f" (bugs: {s.rte_bug + s.callsite_bug})"
                if s.rte_failed or s.callsite_failed:
                    vo_part += (
                        f" (failed: {s.rte_failed + s.callsite_failed})"
                    )
                if s.complete:
                    vo_part += " ✓"
            logger.info(
                "  %s: %s cycles=%d%s%s",
                result.function_name,
                result.proof_state.value if result.proof_state else "unknown",
                result.cycle_count,
                vo_part,
                marker,
            )
        self._log_run_summary(results)
        return results

    def _log_run_summary(self, results: list) -> None:
        """Print a comprehensive end-of-run summary to the log."""
        import collections

        states = collections.Counter(
            r.proof_state.value if r.proof_state else "unknown"
            for r in results
        )
        total_cycles = sum(r.cycle_count for r in results)
        max_cycle = max((r.cycle_count for r in results), default=0)
        cycle_limited = [r for r in results if r.cycle_limit_reached]

        event_counts: dict[str, int] = collections.defaultdict(int)
        wall_min = 0.0
        if self._run_store is not None:
            events_list = self._run_store.read_events()
            for ev in events_list:
                event_counts[ev.type] += 1
            if len(events_list) >= 2:
                from datetime import datetime
                try:
                    t0 = datetime.fromisoformat(events_list[0].timestamp)
                    t1 = datetime.fromisoformat(events_list[-1].timestamp)
                    wall_min = (t1 - t0).total_seconds() / 60
                except Exception:
                    pass

        logger.info("=" * 60)
        logger.info("RUN SUMMARY")
        logger.info("=" * 60)
        logger.info(
            "Result: %d/%d global_ok  |  %s",
            states.get("global_ok", 0),
            len(results),
            ", ".join(f"{v} {k}" for k, v in states.most_common()),
        )
        conditional = [
            r for r in results
            if r.proof_state is not None
            and r.proof_state.value == "local_ok"
            and r.settled_assumes
        ]
        axiomatic = [r for r in results if r.extern_axioms]
        if conditional:
            logger.info(
                "Conditional OK (assume budget spent): %s",
                ", ".join(
                    f"{r.function_name} (assumes {', '.join(r.settled_assumes)})"
                    for r in conditional
                ),
            )
        if axiomatic:
            logger.info(
                "Environment axioms (extern callees, contracts assumed): %s",
                ", ".join(
                    f"{r.function_name} (axioms for {', '.join(r.extern_axioms)})"
                    for r in axiomatic
                ),
            )
        if wall_min > 0:
            logger.info("Wall clock: %.1f min", wall_min)
        logger.info(
            "Cycles: %d total, max %d/agent, %d cycle-limited",
            total_cycles, max_cycle, len(cycle_limited),
        )
        logger.info(
            "Fixpoint: %d publishes, %d republish-noops, "
            "%d demands (%d resolved)",
            event_counts.get("spec_published", 0),
            event_counts.get("spec_republish_noop", 0),
            event_counts.get("proof_demand_raised", 0),
            event_counts.get("proof_demand_resolved", 0),
        )
        if event_counts.get("agent_suspended", 0):
            logger.info(
                "Demand protocol: %d suspensions, %d releases",
                event_counts.get("agent_suspended", 0),
                event_counts.get("agent_released", 0),
            )

        # Per-batch breakdown
        if self._run_store is not None:
            from datetime import datetime
            events_list = self._run_store.read_events()
            in_flight: set[str] = set()
            batch_num = 0
            batch_info: dict[int, dict] = {}
            for ev in events_list:
                if ev.type == "agent_cycle_started":
                    if not in_flight:
                        batch_num += 1
                        batch_info[batch_num] = {"n": 0, "start": ev.timestamp, "end": ev.timestamp, "states": []}
                    in_flight.add(ev.agent_id or "")
                    batch_info[batch_num]["n"] += 1
                elif ev.type == "agent_cycle_ended":
                    in_flight.discard(ev.agent_id or "")
                    ps = (ev.data or {}).get("proof_state", "?")
                    batch_info[batch_num]["end"] = ev.timestamp
                    batch_info[batch_num]["states"].append(ps)
            if batch_info:
                logger.info("Per-batch convergence:")
                cumulative: dict[str, str] = {}
                for b in sorted(batch_info):
                    info = batch_info[b]
                    try:
                        dur = (datetime.fromisoformat(info["end"]) - datetime.fromisoformat(info["start"])).total_seconds() / 60
                    except Exception:
                        dur = 0
                    for aid_ps in zip(
                        [ev.agent_id for ev in events_list
                         if ev.type == "agent_cycle_ended"
                         and ev.timestamp <= info["end"]
                         and ev.timestamp >= info["start"]],
                        info["states"],
                    ):
                        cumulative[aid_ps[0] or ""] = aid_ps[1]
                    cum = collections.Counter(cumulative.values())
                    g = cum.get("global_ok", 0)
                    l = cum.get("local_ok", 0)
                    dr = cum.get("draft", 0)
                    di = cum.get("dirty", 0)
                    phase = "initial" if b == 1 else f"react-{b-1}"
                    logger.info(
                        "  batch %d (%s): %d agents, %.1fm → g_ok=%d l_ok=%d draft=%d dirty=%d",
                        b, phase, info["n"], dur, g, l, dr, di,
                    )

        # Per-agent cost/turns
        agents_with_cost = [
            (a.function_info.name, a.total_cost, a.total_turns,
             self._wakes.cycle_count(a.agent_id))
            for a in self._agents if a.total_cost > 0
        ]
        if agents_with_cost:
            total_cost = sum(c for _, c, _, _ in agents_with_cost)
            total_turns = sum(t for _, _, t, _ in agents_with_cost)
            logger.info("Token usage:")
            for name, cost, turns, cycles in sorted(agents_with_cost, key=lambda x: -x[1]):
                logger.info("  %s: $%.4f, %d turns, %d cycles", name, cost, turns, cycles)
            logger.info("  TOTAL: $%.2f, %d turns", total_cost, total_turns)

        logger.info("=" * 60)
