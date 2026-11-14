"""Run record and event dataclasses for structured solve observability.

Each `verdict solve` invocation produces one `RunRecord` plus an append-only
stream of `RunEvent`s, both persisted under the run's log directory. The
dashboard and offline analysis tools consume these as a structured
alternative to `pipeline.log`.

No pydantic dependency: Verdict's core is already dataclass-based, so we
serialize with `to_dict` helpers that unwrap enums explicitly.
"""

from __future__ import annotations

import enum
from dataclasses import asdict, dataclass, field
from datetime import datetime, timezone
from typing import Any

from verdict.core.types import AgentResult


class RunStatus(enum.Enum):
    """High-level lifecycle state of a solve run."""

    INIT = "init"
    RUNNING = "running"
    COMPLETED = "completed"
    CANCELLED = "cancelled"
    FAILED = "failed"


def _now_iso() -> str:
    return datetime.now(timezone.utc).isoformat(timespec="milliseconds")


@dataclass
class RunEvent:
    """One append-only event in the run's events.jsonl stream."""

    run_id: str
    type: str
    timestamp: str = field(default_factory=_now_iso)
    agent_id: str | None = None
    data: dict[str, Any] = field(default_factory=dict)

    def to_dict(self) -> dict[str, Any]:
        return {
            "run_id": self.run_id,
            "type": self.type,
            "timestamp": self.timestamp,
            "agent_id": self.agent_id,
            "data": self.data,
        }


def _spec_to_json_dict(spec: Any) -> dict[str, Any]:
    """asdict(spec) with JSON-unsafe values normalized.

    ``FunctionSpec.spec_deps[callee].clauses`` is a ``frozenset`` (or
    ``None``), which ``json.dumps`` cannot encode. Convert each present
    clause set to a sorted list so run.json stays serializable.
    """
    data = asdict(spec)
    # Unwrap SpecStatus enum to string for JSON
    if hasattr(data.get("status"), "value"):
        data["status"] = data["status"].value
    spec_deps = data.get("spec_deps")
    if spec_deps:
        for descriptor in spec_deps.values():
            clauses = descriptor.get("clauses")
            if clauses is not None:
                descriptor["clauses"] = sorted(clauses)
    # callsite_pre_deps values are frozensets — convert to sorted lists
    cpd = data.get("callsite_pre_deps")
    if cpd:
        data["callsite_pre_deps"] = {k: sorted(v) for k, v in cpd.items()}
    return data


def _agent_result_to_dict(result: AgentResult) -> dict[str, Any]:
    """Serialize an AgentResult, unwrapping enum values to strings."""
    data: dict[str, Any] = {
        "agent_id": result.agent_id,
        "function_name": result.function_name,
        "proof_state": (
            result.proof_state.value if result.proof_state is not None else None
        ),
        "spec": _spec_to_json_dict(result.spec) if result.spec is not None else None,
        "wake_reasons": list(result.wake_reasons),
        "error": result.error,
        "cycle_count": result.cycle_count,
        "cycle_limit_reached": result.cycle_limit_reached,
        "vo_summary": asdict(result.vo_summary) if result.vo_summary is not None else None,
        "pending_wakes": result.pending_wakes,
        "callees": list(result.callees),
        "session_id": result.session_id,
        "settled_assumes": list(result.settled_assumes),
        "extern_axioms": list(result.extern_axioms),
    }
    return data


@dataclass
class RunRecord:
    """Structured snapshot of a solve run persisted to `run.json`."""

    id: str
    source_file: str
    started_at: str = field(default_factory=_now_iso)
    status: RunStatus = RunStatus.INIT
    finished_at: str | None = None
    config: dict[str, Any] = field(default_factory=dict)
    agents: dict[str, AgentResult] = field(default_factory=dict)
    summary: dict[str, Any] = field(default_factory=dict)

    def to_dict(self) -> dict[str, Any]:
        return {
            "id": self.id,
            "source_file": self.source_file,
            "started_at": self.started_at,
            "finished_at": self.finished_at,
            "status": self.status.value,
            "config": self.config,
            "agents": {
                name: _agent_result_to_dict(result)
                for name, result in self.agents.items()
            },
            "summary": self.summary,
        }
