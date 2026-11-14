"""Result dataclasses returned by agent-facing verification tools."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Optional


@dataclass
class VerificationResult:
    """Result from Frama-C WP verification (worklist-based)."""

    success: bool
    feedback: str = ""
    wp_warnings: list[str] = field(default_factory=list)
    error: Optional[str] = None
    verified_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    failed_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    verified_postconditions: list[str] = field(default_factory=list)
    failed_postconditions: list[str] = field(default_factory=list)
    verified_assigns: list[str] = field(default_factory=list)
    failed_assigns: list[str] = field(default_factory=list)
    worklist_log: str | None = None
    filtered_annotations: list[str] | None = None
    # Draft persistence: when success=True the verified annotations and the
    # caller-supplied callee_contracts are written to the working draft so
    # subsequent registry-driven tools (verify_rte_assertions,
    # verify_callsite_preconditions) read the same proof context. The fields
    # below tell the agent what was saved.
    draft_saved: bool = False
    draft_version: int | None = None
    draft_callee_contracts_count: int = 0


@dataclass
class SyntaxCheckResult:
    """Result from ACSL syntax check."""

    valid: bool
    errors: list[SyntaxError_] = field(default_factory=list)


@dataclass
class SyntaxError_:
    """A single ACSL syntax error."""

    message: str
    line_hint: str | None = None


@dataclass
class FunctionContext:
    """Structured context for a function and its callees/callers."""

    function_name: str
    source_code: str
    loops: list[dict[str, Any]] = field(default_factory=list)
    existing_annotations: dict[str, Any] = field(default_factory=dict)
    callee_layers: list[dict[str, Any]] = field(default_factory=list)
    callers: list[dict[str, Any]] | None = None
    global_vars: list[dict[str, Any]] = field(default_factory=list)


@dataclass
class VOResult:
    """Result for a single verification obligation."""

    verified: bool
    obligation: str = ""


@dataclass
class VOUpdate:
    """Record of a VO state transition."""

    vo_id: str
    status: str  # "verified" | "failed" | "pending"


@dataclass
class RTEVerificationResult:
    """Result from targeted RTE assertion verification."""

    results: dict[str, VOResult] = field(default_factory=dict)
    all_verified: bool = False
    verified_count: int = 0
    total_count: int = 0
    summary: str = ""
    vo_updates: list[VOUpdate] = field(default_factory=list)


@dataclass
class PreconditionResult:
    """Result for a single (call-site, precondition) verification."""

    precondition: str = ""
    verified: bool = False
    obligation: str = ""
    call_site_line: int = 0  # source line of the physical call site
    precondition_index: int = 0  # index in callee's preconditions list


@dataclass
class CallsiteVerificationResult:
    """Result from call-site precondition verification."""

    callee_function: str = ""
    results: list[PreconditionResult] = field(default_factory=list)
    all_verified: bool = False
    verified_count: int = 0
    total_count: int = 0
    summary: str = ""
    vo_updates: list[VOUpdate] = field(default_factory=list)


@dataclass
class WeakeningResult:
    """Result from precondition weakening cascade."""

    success: bool
    removed_postconditions: list[str] = field(default_factory=list)
    removed_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    removed_assigns: list[str] = field(default_factory=list)
    affected_callers: list[str] = field(default_factory=list)
    affected_callees: list[str] = field(default_factory=list)
    # Surviving specs after worklist re-verification
    verified_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    verified_postconditions: list[str] = field(default_factory=list)
    verified_assigns: list[str] = field(default_factory=list)
    worklist_log: str | None = None
    # VO re-verification results
    affected_vo_nodes: list[str] = field(default_factory=list)
    affected_vo_details: dict[str, dict[str, Any]] = field(default_factory=dict)
    vo_reverification_summary: str = ""
