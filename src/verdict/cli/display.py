"""CLI display helpers: function info, call-chain tree, interactive picker."""

from __future__ import annotations

from typing import Any

from verdict.core.project_context import ProjectContext
from verdict.core.types import FunctionInfo


def _build_function_info(func_name: str, project_ctx: ProjectContext) -> FunctionInfo:
    """Build a FunctionInfo from the call graph."""
    node = project_ctx.call_graph_dict[func_name]
    callees = [c.func_name for c in node.children] if node.children else []
    return FunctionInfo(
        name=func_name,
        source_file=project_ctx.source_file,
        source_code=node.source_code or "",
        callees=callees,
        callers=[p.func_name for p in node.parents] if node.parents else [],
        extern_callees=[
            c for c in callees if c in project_ctx.extern_functions
        ],
    )


def _build_call_chain_display(
    call_graph_dict: dict[str, Any],
    rte_vos_by_function: dict[str, list[str]],
) -> list[tuple[str, str]]:
    """Build call-chain display lines.

    Returns list of (func_name, display_line) tuples.
    """
    children_map: dict[str, list[str]] = {}
    for name, node in call_graph_dict.items():
        kids = []
        if node.children:
            kids = [c.func_name for c in node.children if c.func_name in call_graph_dict]
        children_map[name] = kids

    has_parent: set[str] = set()
    for name, node in call_graph_dict.items():
        if node.children:
            for c in node.children:
                if c.func_name in call_graph_dict:
                    has_parent.add(c.func_name)

    roots = [n for n in call_graph_dict if n not in has_parent]
    if not roots:
        roots = list(call_graph_dict.keys())

    lines: list[tuple[str, str]] = []
    visited: set[str] = set()

    def _stats(name: str) -> str:
        node = call_graph_dict[name]
        if not getattr(node, "is_definition", True):
            return "[extern — environment, no agent]"
        n_rte = len(rte_vos_by_function.get(name, []))
        n_loops = len(node.loops) if node.loops else 0
        n_callees = len(children_map.get(name, []))
        rte_s = "RTE" if n_rte == 1 else "RTEs"
        loop_s = "loop" if n_loops == 1 else "loops"
        callee_s = "callee" if n_callees == 1 else "callees"
        return f"({n_rte} {rte_s}, {n_loops} {loop_s}, {n_callees} {callee_s})"

    def _walk(name: str, prefix: str, is_last: bool, is_root: bool) -> None:
        if name in visited:
            return
        visited.add(name)

        stats = _stats(name)
        if is_root:
            lines.append((name, f"  {name:<36s} {stats}"))
        else:
            connector = "\u2514\u2500\u2500 " if is_last else "\u251c\u2500\u2500 "
            lines.append((name, f"  {prefix}{connector}{name:<28s} {stats}"))

        kids = children_map.get(name, [])
        for i, kid in enumerate(kids):
            if kid in visited:
                continue
            child_is_last = (i == len(kids) - 1)
            if is_root:
                child_prefix = "    "
            else:
                child_prefix = prefix + ("\u2502   " if not is_last else "    ")
            _walk(kid, child_prefix, child_is_last, False)

    for root in sorted(roots):
        _walk(root, "", True, True)

    for name in call_graph_dict:
        if name not in visited:
            _walk(name, "", True, True)

    return lines


def _interactive_picker(project_ctx: ProjectContext) -> list[str]:
    """Show interactive function picker using pick library."""
    from pick import pick

    display = _build_call_chain_display(
        project_ctx.call_graph_dict,
        project_ctx.rte_vos_by_function,
    )

    options = [line for _name, line in display]
    name_map = {line: name for name, line in display}

    title = "Select functions to start agents for (Space to select, Enter to confirm):"

    try:
        selected = pick(
            options,
            title,
            multiselect=True,
            min_selection_count=1,
        )
    except KeyboardInterrupt:
        return []

    return [name_map[option] for option, _index in selected]
