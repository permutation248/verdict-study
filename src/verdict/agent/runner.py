"""Function Agent Runner: reusable solve/reactivate lifecycle for one unit."""

from __future__ import annotations

from typing import TYPE_CHECKING

from claude_agent_sdk import (
    ClaudeAgentOptions,
    ClaudeSDKClient,
    ResultMessage,
)

from verdict.agent.registry import AgentRegistry
from verdict.core.ablation import AblationConfig
from verdict.core.env import get_sdk_env, resolve_model
from verdict.core.types import (
    AgentMessage,
    AgentResult,
    FunctionInfo,
    ProofDemand,
    ProofState,
    SpecificationRequest,
    SpecificationResponse,
    SpecificationUpdate,
    CascadeNotification,
    ToolProfile,
    WakeReason,
)
from verdict.core.vo_tracker import VOEntry, VOTracker
from verdict.logging.logger import create_agent_logger, log_agent_state_change
from verdict.messaging.bus import AsyncQueueBus
from verdict.prompts.function_agent import build_function_agent_prompt
from verdict.tools.context import ToolContext
from verdict.tools.factory import create_agent_mcp_server

if TYPE_CHECKING:
    from verdict.cli.agent_stream import AgentOutputSink
    from verdict.core.project_context import ProjectContext
    from verdict.core.spec_tracker import SpecTracker
    from verdict.logging.logger import ToolDebugger


def _summarize_message(message: AgentMessage) -> str:
    """Build a short wake summary for pending inbox items."""
    if isinstance(message, SpecificationRequest):
        return (
            f"spec request from {message.requester_id}: "
            f"{message.request_type.value} {message.required_property}"
        )
    if isinstance(message, SpecificationResponse):
        return (
            f"spec response from {message.responder_id}: "
            f"{message.request_type.value} -> {message.status}"
        )
    if isinstance(message, SpecificationUpdate):
        changed = ", ".join(message.changed_kinds) if message.changed_kinds else message.spec_type.value
        return (
            f"spec update from {message.source_agent_id} on {message.function_name} "
            f"(v{message.version or 0}, {changed})"
        )
    if isinstance(message, CascadeNotification):
        return (
            f"weakening cascade from {message.source_agent_id} on {message.function_name}: "
            f"{message.weakened_precondition}"
        )
    if isinstance(message, ProofDemand):
        return (
            f"proof demand from {message.requester_id}: "
            f"{message.property_kind} {message.property_text}"
        )
    return str(message)


def _summarize_demand(demand: ProofDemand) -> str:
    """Build a short summary for an open runtime proof demand."""
    blocked = (
        f" blocking {', '.join(demand.blocking_vo_ids)}"
        if demand.blocking_vo_ids
        else ""
    )
    rationale = f" :: {demand.rationale}" if demand.rationale else ""
    prefix = f"[{demand.demand_id}] " if demand.demand_id else ""
    return (
        f"{prefix}{demand.property_kind} {demand.property_text}"
        f" (from {demand.requester_id}){blocked}{rationale}"
    )


def _summarize_submitted_demand(demand: ProofDemand) -> str:
    """Summary for a demand THIS agent submitted (still open)."""
    blocked = (
        f" blocking your VOs: {', '.join(demand.blocking_vo_ids)}"
        if demand.blocking_vo_ids
        else ""
    )
    prefix = f"[{demand.demand_id}] " if demand.demand_id else ""
    return (
        f"{prefix}-> {demand.target_function}: {demand.property_kind} "
        f"{demand.property_text}{blocked}"
    )


def _build_vo_status_section(vo_tracker) -> list[str]:
    """Render a compact VO status summary for the cycle prompt."""
    if vo_tracker is None:
        return []
    from verdict.core.vo_tracker import VOStatus

    rte = list(vo_tracker.rte_vos.values())
    cs = list(vo_tracker.leaf_call_site_vos().values())
    spec = vo_tracker.spec_vo

    if not rte and not cs and spec is None:
        return []

    def _count(entries, status):
        return sum(1 for e in entries if e.status == status)

    def _ids(entries, status, limit=5):
        ids = [e.vo_id for e in entries if e.status == status]
        if len(ids) <= limit:
            return ", ".join(ids)
        return ", ".join(ids[:limit]) + f" (+{len(ids) - limit} more)"

    lines = ["", "VO Status:"]

    if spec is not None and spec.status == VOStatus.PENDING:
        lines.append(
            f"  Spec: STALE — {spec.description}"
        )

    if rte:
        v = _count(rte, VOStatus.VERIFIED)
        p = _count(rte, VOStatus.PENDING)
        f = _count(rte, VOStatus.FAILED)
        b = _count(rte, VOStatus.BUG)
        lines.append(
            f"  RTE: {v} verified, {p} pending, {f} failed, {b} bug  (total {len(rte)})"
        )
        if p > 0:
            lines.append(f"    Pending: {_ids(rte, VOStatus.PENDING)}")
        if f > 0:
            lines.append(f"    Failed:  {_ids(rte, VOStatus.FAILED)}")

    if cs:
        v = _count(cs, VOStatus.VERIFIED)
        p = _count(cs, VOStatus.PENDING)
        f = _count(cs, VOStatus.FAILED)
        b = _count(cs, VOStatus.BUG)
        lines.append(
            f"  Call-site: {v} verified, {p} pending, {f} failed, {b} bug  (total {len(cs)})"
        )
        if p > 0:
            lines.append(f"    Pending: {_ids(cs, VOStatus.PENDING)}")
        if f > 0:
            lines.append(f"    Failed:  {_ids(cs, VOStatus.FAILED)}")

    return lines


class FunctionAgent:
    """Reusable owner-only solver for one verification unit."""

    def __init__(
        self,
        agent_id: str,
        function_info: FunctionInfo,
        registry: AgentRegistry,
        bus: AsyncQueueBus,
        project_context: ProjectContext | None = None,
        model: str | None = None,
        max_turns: int = 50,
        spec_tracker: SpecTracker | None = None,
        debugger: ToolDebugger | None = None,
        tool_profile: ToolProfile = "solve",
        output_sink: "AgentOutputSink | None" = None,
        key_index: int = 0,
        ablation: AblationConfig | None = None,
    ) -> None:
        self.agent_id = agent_id
        self.function_info = function_info
        self.registry = registry
        self.bus = bus
        self.logger = create_agent_logger(agent_id)
        self._model = model
        self._max_turns = max_turns
        self._tool_profile: ToolProfile = tool_profile
        self._session_id = "default"
        self._output_sink = output_sink
        self._key_index = key_index
        self._ablation = ablation or AblationConfig()

        self.total_cost: float = 0.0
        self.total_turns: int = 0
        self.vo_tracker = VOTracker()

        from verdict.tools._wp_runner import compute_wp_timeout
        wp_timeout = (
            compute_wp_timeout(project_context, function_info.name)
            if project_context is not None
            else 10
        )
        self._ctx = ToolContext(
            agent_id=agent_id,
            function_info=function_info,
            registry=registry,
            message_bus=bus,
            logger=self.logger,
            project_context=project_context,
            vo_tracker=self.vo_tracker,
            spec_tracker=spec_tracker,
            debugger=debugger,
            tool_profile=tool_profile,
            wp_timeout=wp_timeout,
            ablation=self._ablation,
        )
        self._mcp_server = create_agent_mcp_server(self._ctx, tool_profile=tool_profile)
        self._prompt = build_function_agent_prompt(
            function_info, tool_profile=tool_profile, ablation=self._ablation
        )
        if project_context is not None:
            self._init_vos(project_context)

    def _init_vos(self, project_context: ProjectContext) -> None:
        """Initialize precomputed RTE and call-site VOs."""
        fname = self.function_info.name
        for vo_id in project_context.rte_vos_by_function.get(fname, []):
            self.vo_tracker.add_rte_vo(
                VOEntry(
                    vo_id=vo_id,
                    vo_type="rte",
                    description=f"RTE assertion {vo_id} in {fname}",
                    related_function=fname,
                )
            )

        for cs_vo_node in project_context.call_sites_by_function.get(fname, []):
            callee_name = cs_vo_node.callee_extended_function_node.func_name
            cs_line = cs_vo_node.extent[0] if cs_vo_node.extent else None
            self.vo_tracker.add_call_site_vo(
                VOEntry(
                    vo_id=cs_vo_node.vo_id,
                    vo_type="call_site",
                    description=f"Call-site precondition check: {fname} -> {callee_name}",
                    related_function=callee_name,
                    call_site_line=cs_line,
                )
            )

        self.logger.info(
            "Initialized %d RTE VOs, %d call-site VOs",
            len(self.vo_tracker.rte_vos),
            len(self.vo_tracker.call_site_vos),
        )

    def _set_proof_state(self, new: ProofState) -> None:
        old = self.registry.get_proof_state(self.agent_id)
        self.registry.set_proof_state(self.agent_id, new)
        if old != new:
            log_agent_state_change(self.logger, self.agent_id, old.value, new.value)

    def _peek_bus_messages(self) -> list[AgentMessage]:
        """Inbox messages to surface in the wake prompt (debug profile only).

        Peeks without consuming; the check_inbox debug tool drains the bus when
        the agent explicitly reads its inbox. The bus IS the per-agent inbox —
        no separate reconciler queue is layered on top.
        """
        if self._tool_profile != "debug":
            return []
        return self.bus.peek_messages(self.agent_id)

    def _build_cycle_prompt(
        self,
        wake_reasons: list[WakeReason],
        inbox_messages: list[AgentMessage],
    ) -> str:
        """Build the user prompt for an initial solve or reactivation."""
        incoming_demands = self.registry.open_proof_demands(self.function_info.name)
        outgoing_demands = self.registry.open_proof_demands_submitted_by(self.agent_id)
        vo_section = _build_vo_status_section(self.vo_tracker)
        if not wake_reasons:
            prompt = (
                f"Initial solve for function `{self.function_info.name}`.\n"
                "Establish or refine a working ACSL draft, rely only on published callee specs, "
                "and publish only when your spec is locally useful and verified."
            )
            lines = [prompt]
            if self.function_info.extern_callees:
                lines.append(
                    f"Extern environment callees (prototype only, no agent, "
                    f"never publish): {sorted(self.function_info.extern_callees)}. "
                    f"WP uses their bare prototype by default; if your proof "
                    f"needs guarantees about them, state your hypothesis in "
                    f"callee_contracts[...] (postconditions/assigns) — it is "
                    f"recorded as a standing environment axiom, reported with "
                    f"your result. Never wait on or demand these."
                )
            if outgoing_demands:
                lines.extend(["", "Your open proof demands (still awaiting reply):"])
                for summary in (_summarize_submitted_demand(d) for d in outgoing_demands[:5]):
                    lines.append(f"- {summary}")
                if len(outgoing_demands) > 5:
                    lines.append(f"  ... and {len(outgoing_demands) - 5} more")
                if self._ablation.assume:
                    lines.append(
                        "  Until these are resolved, keep your draft and wait "
                        "for the callee's publish (optimistic hypotheses are "
                        "disabled in this run); do NOT drop postconditions "
                        "that rely on the demanded property."
                    )
                else:
                    lines.append(
                        "  Until these are resolved, verify under your hypothesis: "
                        "verify_function_contract with `assume_callee=[...]` (+ the "
                        "assumed clauses in `callee_contracts`) — the saved draft "
                        "then carries that context into verify_rte_assertions / "
                        "verify_callsite_preconditions automatically."
                    )
            if incoming_demands:
                lines.extend(["", "Demands targeting you (resolve via resolve_proof_demand or by publishing):"])
                for summary in (_summarize_demand(demand) for demand in incoming_demands[:5]):
                    lines.append(f"- {summary}")
                if len(incoming_demands) > 5:
                    lines.append(f"  ... and {len(incoming_demands) - 5} more")
            lines.extend(vo_section)
            return "\n".join(lines)

        lines = [f"Reactivated for function `{self.function_info.name}`.", "", "Wake reasons:"]
        for reason in wake_reasons:
            source = reason.source_function or "runtime"
            version = (
                f" v{reason.source_version}" if reason.source_version is not None else ""
            )
            changed = (
                f" [{', '.join(reason.changed_kinds)}]" if reason.changed_kinds else ""
            )
            details = f" :: {reason.details}" if reason.details else ""
            if reason.message_count:
                details = f"{details} ({reason.message_count} new inbox messages)".strip()
            lines.append(f"- {reason.reason} from {source}{version}{changed}{details}")

        if inbox_messages:
            lines.extend(["", "Pending inbox summaries:"])
            for summary in (_summarize_message(msg) for msg in inbox_messages[:5]):
                lines.append(f"- {summary}")
            if len(inbox_messages) > 5:
                lines.append(f"  ... and {len(inbox_messages) - 5} more")

        if outgoing_demands:
            lines.extend(["", "Your open proof demands (still awaiting reply):"])
            for summary in (_summarize_submitted_demand(d) for d in outgoing_demands[:5]):
                lines.append(f"- {summary}")
            if len(outgoing_demands) > 5:
                lines.append(f"  ... and {len(outgoing_demands) - 5} more")
            if self._ablation.assume:
                lines.append(
                    "  KEY: a demand pending here means the callee has NOT "
                    "yet delivered the property you asked for. Do NOT drop "
                    "your postconditions that rely on it — keep the draft "
                    "and wait for the callee's publish (optimistic "
                    "hypotheses are disabled in this run)."
                )
            else:
                lines.append(
                    "  KEY: a demand pending here means the callee has NOT yet "
                    "delivered the property you asked for. Do NOT drop your "
                    "postconditions that rely on it. Verify under your hypothesis "
                    "via verify_function_contract with `assume_callee=[name]` — "
                    "the other verify tools inherit that context from your draft "
                    "until the callee responds."
                )

        if incoming_demands:
            lines.extend(["", "Demands targeting you (resolve via resolve_proof_demand or by publishing):"])
            for summary in (_summarize_demand(demand) for demand in incoming_demands[:5]):
                lines.append(f"- {summary}")
            if len(incoming_demands) > 5:
                lines.append(f"  ... and {len(incoming_demands) - 5} more")

        lines.extend(vo_section)

        lines.extend(
            [
                "",
                "Resolve all PENDING VOs shown above (Spec, RTE, call-site) before "
                "re-publishing. Use the published callee contracts (not your old hypothesis).",
            ]
        )
        return "\n".join(lines)

    def _snapshot_result(
        self,
        wake_reasons: list[WakeReason],
        error: str | None = None,
    ) -> AgentResult:
        """Build a cycle result from current registry state.

        Proof-state OWNERSHIP (one authority per layer — do not duplicate):
          - ``registry.write_spec`` sets the EMPTY/DRAFT bootstrap (spec
            content only);
          - ``Orchestrator._recompute_proof_state`` is the SOLE authority for
            promotion/demotion among DRAFT/LOCAL_OK/GLOBAL_OK/DIRTY — it runs
            in ``_emit_cycle_ended`` right after this cycle;
          - the runner owns ONLY the FAILED execution flag: set it when the
            cycle throws, clear it when the cycle completes cleanly.
        We deliberately do NOT compute LOCAL_OK/DIRTY here; that derivation
        lives in exactly one place. This method only records the outcome.
        """
        working_spec = self.registry.read_working_spec(self.function_info.name)

        if error is not None:
            self._set_proof_state(ProofState.FAILED)
            return AgentResult(
                agent_id=self.agent_id,
                function_name=self.function_info.name,
                proof_state=ProofState.FAILED,
                spec=working_spec,
                wake_reasons=[reason.reason for reason in wake_reasons],
                error=error,
                session_id=self._session_id if self._session_id != "default" else None,
            )

        # Clean cycle: clear any prior FAILED flag so the orchestrator can
        # re-derive promotion (no-op unless currently FAILED). Reporting only.
        self.registry.clear_failure(self.agent_id)
        proof_state = self.registry.get_proof_state(self.agent_id)
        has_content = bool(
            working_spec
            and (
                working_spec.preconditions
                or working_spec.postconditions
                or working_spec.loop_contracts
                or working_spec.assigns
            )
        )
        return AgentResult(
            agent_id=self.agent_id,
            function_name=self.function_info.name,
            proof_state=proof_state,
            spec=working_spec,
            wake_reasons=[reason.reason for reason in wake_reasons],
            error=None if has_content else "Verification cycle produced no usable spec.",
            session_id=self._session_id if self._session_id != "default" else None,
        )

    def build_sdk_options(self) -> ClaudeAgentOptions:
        """Build SDK options for this agent's configured tool profile.

        On cycle 2+, passes ``resume=session_id`` so the CLI subprocess
        restores the full conversation history from the previous cycle.
        """
        resume_id = None
        if self._session_id and self._session_id != "default":
            resume_id = self._session_id
        def _on_stderr(line: str) -> None:
            self.logger.warning("CLI stderr: %s", line.rstrip())

        return ClaudeAgentOptions(
            system_prompt=self._prompt,
            mcp_servers={"verdict": self._mcp_server},
            permission_mode="bypassPermissions",
            max_turns=self._max_turns,
            model=resolve_model(self._model),
            env=get_sdk_env(self._key_index, model=self._model),
            resume=resume_id,
            stderr=_on_stderr,
        )

    async def _execute_cycle(
        self,
        wake_reasons: list[WakeReason],
        user_message: str | None = None,
    ) -> AgentResult:
        """Run one LLM tool loop, either initial or reactivated."""
        inbox_messages = self._peek_bus_messages()
        prompt = self._build_cycle_prompt(wake_reasons, inbox_messages)
        if user_message:
            prompt += f"\n\nOperator message: {user_message}"

        import time as _time

        self.logger.info("Session: id=%s (cycle start)", self._session_id)
        cycle_t0 = _time.monotonic()
        client = ClaudeSDKClient(options=self.build_sdk_options())
        try:
            self.logger.debug("SDK: connecting...")
            await client.connect()
            self.logger.debug("SDK: connected, sending query...")
            await client.query(prompt, session_id=self._session_id)
            self.logger.debug("SDK: query sent, awaiting response...")

            if self._output_sink is not None:
                # Route every block through the sink (chat or solve-mode
                # rendering).
                from verdict.cli.agent_stream import process_agent_stream

                await self._output_sink.on_cycle_start(self.agent_id)
                new_session_id, cost, turns = await process_agent_stream(
                    client,
                    agent_id=self.agent_id,
                    sink=self._output_sink,
                )
                if cost:
                    self.total_cost += cost
                if turns:
                    self.total_turns += turns
                if new_session_id is not None:
                    if new_session_id != self._session_id:
                        self.logger.info("Session: %s -> %s", self._session_id, new_session_id)
                    self._session_id = new_session_id
            else:
                async for msg in client.receive_response():
                    if isinstance(msg, ResultMessage):
                        self._session_id = getattr(msg, "session_id", self._session_id)
                        cost = getattr(msg, "total_cost_usd", None)
                        turns = getattr(msg, "num_turns", None)
                        if cost:
                            self.total_cost += cost
                        if turns:
                            self.total_turns += turns
                        break
        except Exception as exc:
            self.logger.exception("agent %s failed", self.agent_id)
            self._session_id = "default"
            return self._snapshot_result(wake_reasons, error=str(exc))
        finally:
            await client.disconnect()
            cycle_s = _time.monotonic() - cycle_t0
            self.logger.info("Cycle finished: %.1fs", cycle_s)

        return self._snapshot_result(wake_reasons)

    async def run(self) -> AgentResult:
        """Execute the initial solve cycle."""
        return await self._execute_cycle([])

    async def reactivate(
        self,
        wake_reasons: list[WakeReason],
        user_message: str | None = None,
    ) -> AgentResult:
        """Execute a wake-up cycle on an existing session."""
        return await self._execute_cycle(wake_reasons, user_message=user_message)

    def snapshot_result(self) -> AgentResult:
        """Return a snapshot result without running a new cycle."""
        return self._snapshot_result([])
