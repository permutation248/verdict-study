"""WakeQueue: the orchestrator's worklist of agents pending re-activation.

Owns the wake-scheduling state and nothing else:
  - ``_ready``            — agents enqueued to run + their wake reasons
  - ``_waiting_on_demand``— agents suspended until a callee publishes
  - ``_cycle_count`` / ``_cycle_limited`` — per-agent cycle accounting and the
    runaway backstop

The orchestrator's wake-reason collectors enqueue here; the fixpoint loop
selects and pops from here. This class never runs an agent, touches the
registry, or derives proof state — it is purely the worklist.
"""

from __future__ import annotations

import logging
from typing import TYPE_CHECKING

from verdict.core.types import WakeReason

if TYPE_CHECKING:
    from verdict.core.run_store import RunStore

logger = logging.getLogger(__name__)


class WakeQueue:
    def __init__(
        self,
        max_cycles_per_agent: int,
        run_store: "RunStore | None",
    ) -> None:
        self._ready: dict[str, list[WakeReason]] = {}
        self._waiting_on_demand: dict[str, set[str]] = {}
        self._cycle_count: dict[str, int] = {}
        self._cycle_limited: set[str] = set()
        self._max_cycles = max_cycles_per_agent
        self._run_store = run_store

    @property
    def max_cycles(self) -> int:
        return self._max_cycles

    # ── enqueue ──────────────────────────────────────────────────────────

    def enqueue(self, agent_id: str, reason: WakeReason) -> None:
        """Queue a wake-up with coarse deduplication.

        Drops the agent once it reaches the cycle limit (the runaway backstop);
        otherwise appends the reason, coalescing an identical pending reason.
        """
        if self._cycle_count.get(agent_id, 0) >= self._max_cycles:
            if agent_id not in self._cycle_limited:
                logger.warning(
                    "agent %s hit cycle limit (%d); skipping further wake-ups: %s",
                    agent_id,
                    self._max_cycles,
                    reason.reason,
                )
                self._cycle_limited.add(agent_id)
                if self._run_store is not None:
                    self._run_store.append_event(
                        "agent_cycle_limited",
                        data={"limit": self._max_cycles},
                        agent_id=agent_id,
                    )
            return
        reasons = self._ready.setdefault(agent_id, [])
        for existing in reasons:
            same_identity = (
                existing.reason == reason.reason
                and existing.source_function == reason.source_function
                and existing.source_version == reason.source_version
                and existing.changed_kinds == reason.changed_kinds
                and existing.details == reason.details
            )
            if same_identity:
                existing.message_count = max(existing.message_count, reason.message_count)
                return
        reasons.append(reason)
        if self._run_store is not None:
            self._run_store.append_event(
                "agent_enqueued",
                data={
                    "reason": reason.reason,
                    "source_function": reason.source_function,
                    "details": reason.details,
                    "queue_depth": len(reasons),
                },
                agent_id=agent_id,
            )

    # ── selection / draining ─────────────────────────────────────────────

    def pending(self) -> dict[str, list[WakeReason]]:
        """The live ready map (handed to an interactive wake selector)."""
        return self._ready

    def actionable(self) -> set[str]:
        """Ready agents not currently suspended waiting on a proof demand."""
        return {aid for aid in self._ready if aid not in self._waiting_on_demand}

    def ensure_present(self, agent_id: str) -> None:
        """Add an agent to the ready map with no reasons (manual trigger)."""
        if agent_id not in self._ready:
            self._ready[agent_id] = []

    def select_only(self, selected_ids: set[str]) -> dict[str, list[WakeReason]]:
        """Keep only *selected_ids* in the ready map; return the removed rest
        so the caller can restore them after running the selection."""
        deferred = {
            aid: reasons
            for aid, reasons in self._ready.items()
            if aid not in selected_ids
        }
        self._ready = {
            aid: self._ready[aid] for aid in selected_ids if aid in self._ready
        }
        return deferred

    def restore(self, deferred: dict[str, list[WakeReason]]) -> None:
        """Merge previously-deferred reasons back into the ready map."""
        for aid, reasons in deferred.items():
            self._ready.setdefault(aid, []).extend(reasons)

    def pop_actionable(self) -> list[tuple[str, list[WakeReason]]]:
        """Remove and return every ready, non-suspended (agent_id, reasons)."""
        items = [
            (aid, reasons)
            for aid, reasons in self._ready.items()
            if aid not in self._waiting_on_demand
        ]
        for agent_id, _reasons in items:
            self._ready.pop(agent_id, None)
        return items

    # ── demand suspension ────────────────────────────────────────────────

    def suspend(self, agent_id: str, targets: set[str], demand_count: int) -> None:
        """Suspend *agent_id* until every function in *targets* publishes."""
        self._waiting_on_demand[agent_id] = targets
        if self._run_store is not None:
            self._run_store.append_event(
                "agent_suspended",
                data={"waiting_on": sorted(targets), "demand_count": demand_count},
                agent_id=agent_id,
            )

    def release(self, published_function: str) -> None:
        """Release agents waiting on *published_function* (once all their
        awaited targets have published)."""
        released: list[str] = []
        for aid, targets in list(self._waiting_on_demand.items()):
            if published_function in targets:
                targets.discard(published_function)
                if not targets:
                    del self._waiting_on_demand[aid]
                    released.append(aid)
        for aid in released:
            if self._run_store is not None:
                self._run_store.append_event(
                    "agent_released",
                    data={"released_by": published_function},
                    agent_id=aid,
                )

    def is_suspended(self, agent_id: str) -> bool:
        return agent_id in self._waiting_on_demand

    def suspended_agents(self) -> set[str]:
        return set(self._waiting_on_demand)

    def clear_suspension(self, agent_id: str) -> None:
        """Release a specific agent from demand suspension."""
        if agent_id in self._waiting_on_demand:
            del self._waiting_on_demand[agent_id]
            if self._run_store is not None:
                self._run_store.append_event(
                    "agent_released",
                    data={"released_by": "demand_resolved"},
                    agent_id=agent_id,
                )

    # ── cycle accounting ─────────────────────────────────────────────────

    def mark_cycle_started(self, agent_id: str) -> int:
        """Increment and return *agent_id*'s cycle count."""
        self._cycle_count[agent_id] = self._cycle_count.get(agent_id, 0) + 1
        return self._cycle_count[agent_id]

    def seed_cycle_count(self, agent_id: str, count: int) -> None:
        """Restore a cycle count from a previous run."""
        self._cycle_count[agent_id] = count

    def cycle_count(self, agent_id: str) -> int:
        return self._cycle_count.get(agent_id, 0)

    def is_cycle_limited(self, agent_id: str) -> bool:
        return agent_id in self._cycle_limited

    def pending_wakes(self, agent_id: str) -> int:
        return len(self._ready.get(agent_id, []))
