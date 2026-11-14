"""Shared CLI solve setup.

Both ``verdict solve`` (autonomous) and ``verdict step`` (interactive) need the
same wiring: preprocess → RunRecord/RunStore/SpecTracker → scope-resolve →
Orchestrator. That lives here once; each mode then runs the orchestrator its
own way (auto-run vs interactive wake selector).
"""

from __future__ import annotations

import argparse
import logging
from dataclasses import dataclass
from pathlib import Path
from typing import Callable

from verdict.cli.agent_stream import PrefixedSink
from verdict.cli.display import _build_function_info
from verdict.core.ablation import AblationConfig
from verdict.core.project_context import ProjectContext
from verdict.core.run_record import RunRecord
from verdict.core.run_store import RunStore
from verdict.core.spec_tracker import SpecTracker
from verdict.core.types import AgentResult
from verdict.logging.logger import create_run_dir, setup_logging
from verdict.orchestrator.orchestrator import Orchestrator
from verdict.preprocessing.pipeline import preprocess


@dataclass
class SolveContext:
    """Everything a solve/step mode needs after setup."""

    orch: Orchestrator
    run_dir: Path
    run_store: RunStore
    sink: PrefixedSink
    project_ctx: ProjectContext
    function_names: list[str]


def _parse_ablation(spec: str | None) -> AblationConfig:
    """Parse --ablate, converting bad values into a friendly CLI exit."""
    try:
        return AblationConfig.parse(spec)
    except ValueError as exc:
        raise SystemExit(str(exc)) from exc


def _resolve_scope_function_names(
    available_names,
    scope: str,
    requested_names: list[str] | None,
    call_graph: dict[str, list[str]] | None = None,
) -> list[str]:
    """Resolve the ordered function set for the requested solve scope.

    For function-scoped solve, the result includes the requested functions
    plus any transitive callees present in the call graph so that dependency
    specs can be produced.
    """
    all_names = list(available_names)
    available_set = set(all_names)
    if scope == "global":
        return all_names

    requested = list(requested_names or [])
    if not requested:
        raise ValueError("Function-scoped solve requires at least one --function entry.")

    unknown = [name for name in requested if name not in available_set]
    if unknown:
        raise ValueError(
            f"Unknown functions for scoped solve: {', '.join(unknown)}"
        )

    seen: set[str] = set()
    ordered: list[str] = []

    def _enqueue(name: str) -> None:
        if name in seen or name not in available_set:
            return
        seen.add(name)
        ordered.append(name)
        if call_graph is not None:
            for callee in call_graph.get(name, []):
                _enqueue(callee)

    for name in requested:
        _enqueue(name)
    return ordered


def _leaf_first_order(
    names: list[str],
    callee_map: dict[str, list[str]],
) -> list[str]:
    """Reorder functions so callees come before callers (reverse-topological,
    leaf-first).

    Launching callee agents first lets them publish real specs before their
    callers run, which cuts the optimistic-assumption demand cascade whenever
    fewer agents run concurrently than there are functions. Order never affects
    correctness — the fixpoint converges regardless — so this is a pure
    scheduling heuristic. DFS post-order; back-edges from recursive or mutually
    recursive parsers are skipped so cycles terminate.
    """
    name_set = set(names)
    seen: set[str] = set()
    on_stack: set[str] = set()
    order: list[str] = []

    def visit(name: str) -> None:
        if name in seen or name in on_stack:
            return  # already emitted, or a back-edge into the current cycle
        on_stack.add(name)
        for callee in callee_map.get(name, []):
            if callee in name_set:
                visit(callee)
        on_stack.discard(name)
        seen.add(name)
        order.append(name)

    for name in names:
        visit(name)
    return order


def build_solve_context(
    args: argparse.Namespace,
    on_batch_complete: Callable[[str, list[AgentResult]], None] | None = None,
) -> SolveContext:
    """Preprocess, wire run_store/spec_tracker, resolve scope, build the
    Orchestrator. Shared by solve and step modes."""
    run_dir = create_run_dir(args.file)
    # Printed FIRST and flushed: run_solve.sh captures the run dir by
    # parsing this exact line from solve.log ("capture by ownership").
    # Without the flush, block-buffered stdout loses the line on killed
    # runs — precisely the runs whose dir must still be identifiable.
    print(f"Logs: {run_dir}", flush=True)
    setup_logging(run_dir)
    if args.verbose:
        logging.getLogger("verdict").setLevel(logging.DEBUG)

    print(f"Preprocessing {args.file}...")
    project_ctx, _ = preprocess(
        args.file,
        header_files=args.header or None,
        cpp_extra_args=args.cpp_extra_args,
    )
    project_ctx.enable_ablation_deps = bool(getattr(args, "enable_dep_analysis", False))
    ablation = _parse_ablation(getattr(args, "ablate", None))

    run_record = RunRecord(
        id=run_dir.name,
        source_file=str(args.file),
        config={
            "scope": args.scope,
            "timeout": args.timeout,
            "model": args.model,
            "max_turns": args.max_turns,
            "max_parallel": args.max_parallel,
            "schedule": args.schedule,
            "max_cycles_per_agent": args.max_cycles,
            "requested_functions": list(args.function or []),
            "enable_ablation_deps": project_ctx.enable_ablation_deps,
            "ablation": ablation.active(),
            "wp_model": getattr(args, "wp_model", None),
        },
    )
    run_store = RunStore(run_dir, run_record)
    spec_tracker = SpecTracker(run_dir, project_ctx, run_store=run_store)
    sink = PrefixedSink()

    callee_map = {
        name: [c.func_name for c in node.children] if node.children else []
        for name, node in project_ctx.call_graph_dict.items()
    }
    # Extern prototypes are environment, not verification targets.
    agent_candidates = [
        name
        for name in project_ctx.call_graph_dict
        if name not in project_ctx.extern_functions
    ]
    try:
        function_names = _resolve_scope_function_names(
            available_names=agent_candidates,
            scope=args.scope,
            requested_names=args.function,
            call_graph=callee_map,
        )
    except ValueError as exc:
        raise SystemExit(str(exc)) from exc
    if project_ctx.extern_functions:
        logging.getLogger("verdict").info(
            "Extern environment functions (no agent): %s",
            sorted(project_ctx.extern_functions),
        )

    if "main" in function_names and args.scope == "global":
        function_names.remove("main")
        logging.getLogger("verdict").info("Excluded 'main' (test harness, not a verification target)")

    if args.schedule == "leaf-first":
        function_names = _leaf_first_order(function_names, callee_map)

    functions = [_build_function_info(name, project_ctx) for name in function_names]
    orch = Orchestrator(
        functions=functions,
        timeout=args.timeout,
        model=args.model,
        max_turns=args.max_turns,
        spec_tracker=spec_tracker,
        max_parallel=args.max_parallel,
        wp_procs=args.wp_procs,
        wp_model=getattr(args, "wp_model", None),
        ablation=ablation,
        wp_jobs=args.wp_jobs if args.wp_jobs is not None else args.max_parallel,
        project_context=project_ctx,
        scope_kind=args.scope,
        max_cycles_per_agent=args.max_cycles,
        on_batch_complete=on_batch_complete,
        run_store=run_store,
        output_sink=sink,
    )
    return SolveContext(
        orch=orch,
        run_dir=run_dir,
        run_store=run_store,
        sink=sink,
        project_ctx=project_ctx,
        function_names=function_names,
    )
