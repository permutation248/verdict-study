"""ProofDemandStore: the runtime-owned proof-demand queue.

A self-contained store of open / newly-raised / resolved proof demands plus the
demand-id counter and the demand-lifecycle events. The AgentRegistry composes
one and delegates: the registry resolves agent/function identities against its
entry store (under its lock) and hands the resolved facts here, while this class
owns demand storage, id assignment, and event emission. It never touches the
registry's entries — callers pass in everything entry-derived (the requester's
spec version + dependency snapshot, the resolver's function name).
"""

from __future__ import annotations

import copy
from typing import TYPE_CHECKING

from verdict.core.types import DemandResolved, DemandStatus, ProofDemand

if TYPE_CHECKING:
    from verdict.core.run_store import RunStore


class ProofDemandStore:
    def __init__(self, run_store: "RunStore | None" = None) -> None:
        self._open: dict[str, list[ProofDemand]] = {}
        self._new: list[ProofDemand] = []
        self._resolved: list[DemandResolved] = []
        self._counter: int = 0
        self._run_store = run_store

    # ── reads / drains ──────────────────────────────────────────────────

    def open_for(self, function_name: str) -> list[ProofDemand]:
        """Currently open proof demands targeting *function_name*."""
        return [
            copy.deepcopy(demand)
            for demand in self._open.get(function_name, [])
        ]

    def open_submitted_by(self, agent_id: str) -> list[ProofDemand]:
        """Open proof demands *agent_id* submitted (still awaiting a reply)."""
        out: list[ProofDemand] = []
        for demands in self._open.values():
            for demand in demands:
                if demand.requester_id == agent_id:
                    out.append(copy.deepcopy(demand))
        return out

    def pop_new(self) -> list[ProofDemand]:
        """Drain newly raised demands for orchestrator scheduling."""
        demands = list(self._new)
        self._new.clear()
        return demands

    def pop_resolved(self) -> list[DemandResolved]:
        """Drain resolved-demand events for orchestrator scheduling."""
        events = list(self._resolved)
        self._resolved.clear()
        return events

    # ── lifecycle (registry passes entry-derived facts in) ──────────────

    def add(
        self,
        requester_id: str,
        target_function: str,
        property_kind: str,
        property_text: str,
        blocking_vo_ids: list[str] | None,
        rationale: str | None,
        requester_spec_version: int | None,
        requester_dependency_snapshot: dict[str, int],
    ) -> ProofDemand:
        """Assign an id, store, and emit the raised event for a new demand."""
        self._counter += 1
        demand = ProofDemand(
            requester_id=requester_id,
            target_function=target_function,
            property_kind=property_kind,
            property_text=property_text,
            blocking_vo_ids=list(blocking_vo_ids or []),
            rationale=rationale,
            requester_spec_version=requester_spec_version,
            requester_dependency_snapshot=requester_dependency_snapshot,
            demand_id=f"demand_{self._counter}",
            status=DemandStatus.OPEN,
        )
        self._open.setdefault(target_function, []).append(copy.deepcopy(demand))
        self._new.append(copy.deepcopy(demand))

        if self._run_store is not None:
            self._run_store.append_event(
                "proof_demand_raised",
                data={
                    "demand_id": demand.demand_id,
                    "requester_id": requester_id,
                    "target_function": target_function,
                    "property_kind": property_kind,
                    "property_text": property_text,
                    "blocking_vo_ids": list(demand.blocking_vo_ids),
                    "rationale": rationale,
                },
                agent_id=requester_id,
            )
        return copy.deepcopy(demand)

    def resolve(
        self,
        resolver_agent_id: str,
        resolver_function: str,
        demand_id: str,
        resolution: DemandStatus,
        message: str | None = None,
    ) -> ProofDemand:
        """Resolve an open demand the resolver owns (satisfied/rejected)."""
        open_demands = self._open.get(resolver_function, [])
        for i, demand in enumerate(open_demands):
            if demand.demand_id == demand_id:
                if demand.target_function != resolver_function:
                    raise PermissionError(
                        f"Agent {resolver_agent_id} does not own target "
                        f"{demand.target_function}"
                    )
                demand.status = resolution
                demand.resolution_message = message
                resolved = open_demands.pop(i)
                self._resolved.append(
                    DemandResolved(
                        demand=copy.deepcopy(resolved),
                        resolver_function=resolver_function,
                    )
                )
                if self._run_store is not None:
                    self._run_store.append_event(
                        "proof_demand_resolved",
                        data={
                            "demand_id": resolved.demand_id,
                            "requester_id": resolved.requester_id,
                            "target_function": resolved.target_function,
                            "resolution": resolved.status.value,
                            "message": resolved.resolution_message,
                            "resolver_function": resolver_function,
                        },
                        agent_id=resolver_agent_id,
                    )
                return copy.deepcopy(resolved)

        raise KeyError(f"Unknown demand_id: {demand_id}")
