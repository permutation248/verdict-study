"""Interactive step-through solve mode.

The user acts as orchestrator: runtime event processing is mechanical,
but the user decides which agents to wake each round and can inject
messages alongside the wake reasons.

Commands:
  @agent_name [message]   — wake that agent (with optional message)
  @a @b                   — wake multiple agents
  all                     — wake all pending agents
  status                  — show current state
  quit / exit             — stop
"""

from __future__ import annotations

import argparse
import asyncio
import logging
import sys

from verdict.cli._setup import build_solve_context
from verdict.cli.output import _C
from verdict.core.types import WakeReason
from verdict.orchestrator.orchestrator import Orchestrator, WakeSelection


def cmd_step(args: argparse.Namespace) -> None:
    """Run the interactive step-through orchestrator."""
    ctx = build_solve_context(args)
    orch = ctx.orch
    run_dir = ctx.run_dir
    sink = ctx.sink
    function_names = ctx.function_names
    # Build a name → agent_id map for input parsing
    agent_id_map = {name: f"agent_{name}" for name in function_names}

    print(f"Logs: {run_dir}")
    print(f"Specs: {run_dir}/current_specs.c")
    print(f"Step mode: {len(function_names)} functions")
    print(f"Functions: {', '.join(function_names)}")
    print()
    print("After initial batch, you control which agents wake.")
    print("Commands: @name [msg], all, status, quit")
    print("=" * 60)
    print()

    selector = InteractiveWakeSelector(agent_id_map, orch)

    sink.install_log_capture(
        level=logging.DEBUG if args.verbose else logging.INFO,
    )
    try:
        results = asyncio.run(orch.run(wake_selector=selector))
    except KeyboardInterrupt:
        print(f"\n{_C.SYSTEM}Interrupted.{_C.RESET}")
        results = [a.snapshot_result() for a in orch._agents]
    finally:
        sink.uninstall_log_capture()

    from verdict.cli.solve_mode import _print_solve_summary

    _print_solve_summary(results, scope=args.scope)


class InteractiveWakeSelector:
    """Async callable that pauses for user input between rounds."""

    def __init__(
        self,
        agent_id_map: dict[str, str],
        orch: Orchestrator,
    ) -> None:
        self._name_to_id = agent_id_map
        self._id_to_name = {v: k for k, v in agent_id_map.items()}
        self._orch = orch

    async def __call__(
        self,
        ready: dict[str, list[WakeReason]],
    ) -> WakeSelection:
        loop = asyncio.get_running_loop()

        while True:
            self._print_state(ready)

            try:
                raw = await loop.run_in_executor(
                    None,
                    lambda: input(f"{_C.USER}[step] > {_C.RESET}"),
                )
            except (EOFError, KeyboardInterrupt):
                return None

            stripped = raw.strip()
            if not stripped:
                continue
            if stripped.lower() in ("quit", "exit"):
                return None
            if stripped.lower() == "status":
                continue  # re-print
            if stripped.lower() == "all":
                return list(ready.keys()), {}

            # Parse @name commands
            selected_ids: list[str] = []
            user_messages: dict[str, str] = {}
            parts = stripped.split()
            current_name = None
            msg_parts: list[str] = []

            for part in parts:
                if part.startswith("@"):
                    # Flush previous
                    if current_name is not None:
                        aid = self._name_to_id.get(current_name)
                        if aid:
                            selected_ids.append(aid)
                            if msg_parts:
                                user_messages[aid] = " ".join(msg_parts)
                            msg_parts = []
                    name = part[1:]
                    if name in self._name_to_id:
                        current_name = name
                    else:
                        print(f"  Unknown: {name}. Available: {', '.join(sorted(self._name_to_id))}")
                        current_name = None
                else:
                    msg_parts.append(part)

            # Flush last
            if current_name is not None:
                aid = self._name_to_id.get(current_name)
                if aid:
                    selected_ids.append(aid)
                    if msg_parts:
                        user_messages[aid] = " ".join(msg_parts)

            if not selected_ids:
                print(f"  Use @name to select. Available: {', '.join(sorted(self._name_to_id))}")
                continue

            return selected_ids, user_messages

    def _print_state(self, ready: dict[str, list[WakeReason]]) -> None:
        print()
        print(f"{_C.SYSTEM}{'─' * 60}{_C.RESET}")

        # Pending wakes
        if ready:
            print(f"{_C.SYSTEM}Pending wakes:{_C.RESET}")
            for agent_id, reasons in ready.items():
                name = self._id_to_name.get(agent_id, agent_id)
                reason_strs = [
                    f"{r.reason}"
                    + (f" from {r.source_function}" if r.source_function else "")
                    for r in reasons
                ]
                print(f"  {_C.VERDICT_TOOL}{name}{_C.RESET}: {', '.join(reason_strs)}")
        else:
            print(f"{_C.SYSTEM}No pending wakes.{_C.RESET}")

        # VO status
        print(f"{_C.SYSTEM}Agent states:{_C.RESET}")
        for agent in self._orch._agents:
            name = agent.function_info.name
            proof_state = self._orch.registry.get_proof_state(agent.agent_id)
            cycle = self._orch._cycle_count.get(agent.agent_id, 0)
            vo_parts = []
            if agent.vo_tracker:
                rte = agent.vo_tracker.rte_vos
                cs_leaf = agent.vo_tracker.leaf_call_site_vos()
                from verdict.core.vo_tracker import VOStatus

                def _count(vos, st):
                    return sum(1 for v in vos.values() if v.status == st)

                r_v = _count(rte, VOStatus.VERIFIED)
                r_t = len(rte)
                c_v = _count(cs_leaf, VOStatus.VERIFIED)
                c_t = len(cs_leaf)
                vo_parts.append(f"rte {r_v}/{r_t}")
                vo_parts.append(f"cs {c_v}/{c_t}")
            ps = proof_state.value if proof_state else "?"
            vo_str = f"  VOs: {', '.join(vo_parts)}" if vo_parts else ""
            print(f"  {_C.VERDICT_TOOL}{name}{_C.RESET}: {ps} cycle={cycle}{vo_str}")

        print(f"{_C.SYSTEM}{'─' * 60}{_C.RESET}")
