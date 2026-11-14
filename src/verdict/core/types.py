"""Core data types for the Verdict multi-agent framework."""

from __future__ import annotations

import enum
from dataclasses import dataclass, field
from typing import Any, Literal, Optional, Union

from verdict.core.spec_dep import SpecDep


class ProofState(enum.Enum):
    """Proof validity state for a verification unit — the single source of
    truth for a unit's status.

    Lifecycle/phase (running, idle, done, blocked) is NOT a stored state;
    derive it where needed for display: RUNNING = in the orchestrator's
    in-flight set, DONE = GLOBAL_OK, BLOCKED = FAILED, IDLE = otherwise.
    """

    EMPTY = "empty"
    DRAFT = "draft"
    LOCAL_OK = "local_ok"
    DIRTY = "dirty"
    GLOBAL_OK = "global_ok"
    FAILED = "failed"


class DemandStatus(enum.Enum):
    """Lifecycle status of a proof demand."""

    OPEN = "open"
    SATISFIED = "satisfied"
    REJECTED = "rejected"


class SpecType(enum.Enum):
    """Types of ACSL specifications."""

    POSTCONDITION = "postcondition"
    PRECONDITION = "precondition"
    LOOP_CONTRACT = "loop_contract"
    ASSIGNS = "assigns"


class SpecStatus(enum.Enum):
    """Lifecycle status of a function specification."""

    DRAFT = "draft"
    PUBLISHED = "published"


@dataclass
class FunctionSpec:
    """Specification snapshot for a single function."""

    function_name: str
    preconditions: list[str] = field(default_factory=list)
    postconditions: list[str] = field(default_factory=list)
    loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    assigns: list[str] = field(default_factory=list)
    version: int = 0
    status: SpecStatus = SpecStatus.DRAFT
    based_on_versions: dict[str, int] = field(default_factory=dict)
    # Optimistic hypothesis about callees — stored with the draft so
    # registry-driven tools (verify_rte, verify_callsite) can read it.
    callee_contracts: dict[str, dict[str, Any]] = field(default_factory=dict)
    # Specification dependency: callee_name -> SpecDep descriptor.
    # Absent entry == "not measured" == conservative (AFFECTED). Built via
    # sufficiency check after verification; read by the dependency judge.
    spec_deps: dict[str, SpecDep] = field(default_factory=dict)
    # Callees whose published spec was bypassed via assume_callee in the
    # last verify_function_contract call.  At publish time the runtime
    # forces based_on_versions[callee] = 0 for these, blocking GLOBAL_OK
    # until the agent re-verifies without the assumption.
    assumed_callees: list[str] = field(default_factory=list)
    # Reverse (precondition-direction) dependency: callee_name -> the set of
    # callee precondition clauses this caller's call-site proof was verified
    # against. Recorded at publish time when the caller's call-site VOs for
    # that callee are VERIFIED. The dependency judge keeps the call-site
    # discharged as long as the callee's published preconditions are
    # unchanged; if they change, the caller must re-verify the call site.
    # (Sibling-frame postcondition dependencies ride on spec_deps, since
    # sufficiency check now covers call-site VOs.)
    callsite_pre_deps: dict[str, frozenset[str]] = field(default_factory=dict)


@dataclass
class FunctionInfo:
    """Static information about a function to be verified."""

    name: str
    source_file: str
    source_code: str
    callees: list[str] = field(default_factory=list)
    callers: list[str] = field(default_factory=list)
    # Subset of `callees` that are extern prototypes (no body in the
    # project): environment functions. They get no agent and never
    # publish; a callee_contracts entry for one is a standing environment
    # axiom — exempt from dependency anchoring, auto-settled at publish,
    # and reported with the final result.
    extern_callees: list[str] = field(default_factory=list)


ToolProfile = Literal["solve", "debug"]


# ── Runtime Wake / Event Types ──


@dataclass
class WakeReason:
    """Reason why a unit is being scheduled again."""

    reason: str
    source_function: str | None = None
    source_version: int | None = None
    changed_kinds: list[str] = field(default_factory=list)
    message_count: int = 0
    details: str | None = None


@dataclass
class InitialSolveRequested:
    """Initial runtime request to solve a verification unit."""

    unit_id: str


@dataclass
class SpecPublished:
    """Authoritative spec publication event."""

    unit_id: str
    function_name: str
    version: int
    changed_kinds: list[str] = field(default_factory=list)
    precondition_changed: bool = False
    postcondition_additive_only: bool = False
    postcondition_weakened_or_modified: bool = False
    assigns_changed: bool = False
    loop_contract_changed: bool = False


@dataclass
class RuntimeWakeRequested:
    """Runtime wake-up request emitted by the orchestrator."""

    unit_id: str
    reason: WakeReason


@dataclass
class UnitIdle:
    """Unit finished a local cycle and went idle."""

    unit_id: str
    proof_state: ProofState


@dataclass
class UnitFailed:
    """Unit hit a hard failure."""

    unit_id: str
    reason: str


# ── Message Types ──


@dataclass
class SpecificationRequest:
    """Agent A asks Agent B to provide a specific property."""

    requester_id: str
    target_function: str
    request_type: SpecType
    required_property: str
    context: dict[str, Any] = field(default_factory=dict)


@dataclass
class SpecificationResponse:
    """Agent B replies with the status of a requested property."""

    target_function: str
    responder_id: str
    request_type: SpecType
    status: str  # "proven" | "failed" | "pending"
    specification: Optional[str] = None
    error_message: Optional[str] = None


@dataclass
class SpecificationUpdate:
    """Notification that an agent's spec has changed."""

    source_agent_id: str
    function_name: str
    spec_type: SpecType
    version: int | None = None
    changed_kinds: list[str] = field(default_factory=list)
    old_value: Optional[str] = None
    new_value: Optional[str] = None


@dataclass
class CascadeNotification:
    """Precondition weakening cascade notification."""

    source_agent_id: str
    function_name: str
    weakened_precondition: str
    affected_vos: list[str] = field(default_factory=list)


@dataclass
class ProofDemand:
    """Caller asks a callee owner to try covering a useful property."""

    requester_id: str
    target_function: str
    property_kind: str
    property_text: str
    blocking_vo_ids: list[str] = field(default_factory=list)
    rationale: str | None = None
    requester_spec_version: int | None = None
    requester_dependency_snapshot: dict[str, int] = field(default_factory=dict)
    demand_id: str = ""
    status: DemandStatus = DemandStatus.OPEN
    resolution_message: str | None = None


@dataclass
class DemandResolved:
    """Event emitted when a proof demand is satisfied or rejected."""

    demand: ProofDemand
    resolver_function: str


# Union of all inter-agent message types
AgentMessage = Union[
    SpecificationRequest,
    SpecificationResponse,
    SpecificationUpdate,
    CascadeNotification,
    ProofDemand,
]


@dataclass
class VOSummary:
    """Snapshot of a function agent's VO state at finalization time."""

    rte_verified: int = 0
    rte_pending: int = 0
    rte_failed: int = 0
    rte_bug: int = 0
    callsite_verified: int = 0
    callsite_pending: int = 0
    callsite_failed: int = 0
    callsite_bug: int = 0
    # Concrete VO IDs for non-OK states (for surfacing in reports).
    pending_ids: list[str] = field(default_factory=list)
    failed_ids: list[str] = field(default_factory=list)
    # vo_id → reason for BUG-marked VOs.
    bugs: dict[str, str] = field(default_factory=dict)
    # Spec VO status (None if no spec_vo exists).
    spec_status: str | None = None
    # Per-VO status for cache/resume: vo_id → status string.
    vo_states: dict[str, str] = field(default_factory=dict)

    @property
    def total(self) -> int:
        return (
            self.rte_verified + self.rte_pending + self.rte_failed + self.rte_bug
            + self.callsite_verified + self.callsite_pending
            + self.callsite_failed + self.callsite_bug
        )

    @property
    def complete(self) -> bool:
        """True iff no VO is PENDING or FAILED (everything VERIFIED or BUG)."""
        return (
            self.rte_pending == 0
            and self.rte_failed == 0
            and self.callsite_pending == 0
            and self.callsite_failed == 0
        )


@dataclass
class AgentResult:
    """Final result produced by a Function Agent."""

    agent_id: str
    function_name: str
    proof_state: ProofState | None = None
    spec: Optional[FunctionSpec] = None
    wake_reasons: list[str] = field(default_factory=list)
    error: Optional[str] = None
    cycle_count: int = 0
    cycle_limit_reached: bool = False
    vo_summary: Optional[VOSummary] = None
    pending_wakes: int = 0
    callees: list[str] = field(default_factory=list)
    session_id: Optional[str] = None  # persisted for resume across runs
    # Assume debts whose cycle budget ran out: the unit stays LOCAL_OK with
    # these residual hypotheses reported instead of being rewoken forever.
    settled_assumes: list[str] = field(default_factory=list)
    # Extern (environment) callees whose contracts were assumed: standing
    # axioms — no publisher exists, so they are settled at birth and do
    # not cap the proof state. Reported separately from settled_assumes.
    extern_axioms: list[str] = field(default_factory=list)
