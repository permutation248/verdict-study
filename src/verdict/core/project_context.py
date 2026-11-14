"""ProjectContext: shared read-only data built once by Orchestrator."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any


@dataclass
class ProjectContext:
    """Heavy shared state built by Orchestrator, read-only for all agents.

    Holds the ExtendedCallGraph, function declarations, VO pre-computation,
    and working directory for temporary files.
    """

    source_file: str
    source_dir: str
    call_graph_dict: dict[str, Any]
    function_declarations_map: dict[str, str]
    include_paths: list[str]
    work_dir: str

    # CIL file assembly layers (used by build_annotated_cil_file)
    cil_headers: list[str] = field(default_factory=list)
    ordered_declarations: list[str] = field(default_factory=list)

    # Extra Frama-C preprocessor flags (passed through to -cpp-extra-args)
    cpp_extra_args: str | None = None

    # Functions declared but never defined in the project (extern
    # prototypes, e.g. hardware port layers). They stay in the call graph
    # for context/compilation but are environment: no agent is created
    # for them and WP reasons about calls to them via their (possibly
    # axiom-annotated) prototype.
    extern_functions: set[str] = field(default_factory=set)

    # Phase C AUTO ablation gate. When False (default) verify_* never runs
    # callee-support ablation, so existing tests/runs are unaffected.
    enable_ablation_deps: bool = False

    # Per-function filtered declarations from global-deps plugin.
    # Keys are function names, values are the minimal declarations needed.
    # Falls back to ordered_declarations when a function has no entry.
    filtered_declarations: dict[str, list[str]] = field(default_factory=dict)

    # VO pre-computation (Orchestrator builds these)
    vo_graph: dict[str, Any] = field(default_factory=dict)
    rte_vos_by_function: dict[str, list[str]] = field(default_factory=dict)
    call_sites_by_function: dict[str, list[Any]] = field(default_factory=dict)
