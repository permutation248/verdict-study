"""Internal mechanical layer: small utilities that stay in one place.

Larger subsystems have been extracted to:
- ``_cil_builder``: CIL file assembly
- ``_wp_runner``: Frama-C WP invocation & result parsing
- ``_worklist``: Worklist-based verification algorithm
- ``_vo_reverifier``: VO re-verification after weakening

This module keeps tightly-coupled helpers: loop name injection,
context extraction, syntax checking, semantic filtering, and
weakening graph effects.
"""

from __future__ import annotations

import asyncio
from typing import Any


# ---------------------------------------------------------------------------
# Annotation shape validation
# ---------------------------------------------------------------------------


def validate_annotations_shape(annotations: Any) -> list[str]:
    """Validate the structural shape of an annotations dict.

    Agents (especially non-Claude models) sometimes send the wrong JSON
    shape — e.g. ``loop_contracts`` as a list instead of an object — which
    used to crash deep inside the CIL builder with a cryptic
    AttributeError. Returns a list of clear error messages (empty = ok).
    """
    if not isinstance(annotations, dict):
        return [
            "annotations must be an object with preconditions/"
            "postconditions/assigns/loop_contracts"
        ]
    errors: list[str] = []
    for key in ("preconditions", "postconditions", "assigns"):
        v = annotations.get(key)
        if v is None:
            continue
        if not isinstance(v, list):
            errors.append(
                f"{key} must be an array of ACSL clause strings, "
                f"got {type(v).__name__}"
            )
        elif not all(isinstance(s, str) for s in v):
            errors.append(f"every {key} entry must be a string")
    lc = annotations.get("loop_contracts")
    if lc is not None:
        if not isinstance(lc, dict):
            errors.append(
                "loop_contracts must be an object "
                "{loop_name: [clause strings]}, got "
                f"{type(lc).__name__}"
            )
        else:
            for name, clauses in lc.items():
                if not isinstance(clauses, list) or not all(
                    isinstance(s, str) for s in clauses
                ):
                    errors.append(
                        f"loop_contracts[{name!r}] must be an array of "
                        f"clause strings"
                    )
    return errors


# ---------------------------------------------------------------------------
# Loop name comment injection
# ---------------------------------------------------------------------------


def _inject_loop_name_comments(node: Any) -> str:
    """Inject //loop_name comments before each loop in function source code.

    Follows the reference_implementation pattern: bottom-to-top insertion
    of ``//function_name_loop_N`` comments before loop statements so the
    agent can see and reference the loop names.
    """
    source = node.source_code
    if not source or not getattr(node, "loops", None):
        return source or ""

    extent = node.extent
    if not extent:
        return source

    func_start_line = extent[0]

    # Sort loops bottom-to-top to avoid line-number shifts
    sorted_loops = sorted(
        node.loops,
        key=lambda lp: lp.get("loop_extent", (0,))[0],
        reverse=True,
    )

    lines = source.split("\n")
    for loop_info in sorted_loops:
        loop_name = loop_info.get("loop_name", "")
        loop_extent = loop_info.get("loop_extent")
        if not loop_name or not loop_extent:
            continue

        loop_start_line, loop_start_col = loop_extent[0], loop_extent[1]
        relative_line = loop_start_line - func_start_line

        # Build indented comment
        indent = " " * max(loop_start_col - 1, 0)
        comment = f"{indent}//{loop_name}"

        if 0 <= relative_line <= len(lines):
            lines.insert(relative_line, comment)

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Context extraction
# ---------------------------------------------------------------------------


def extract_context(
    call_graph_dict: dict[str, Any],
    func_name: str,
    depth: int = 1,
    include_callers: bool = False,
) -> dict[str, Any]:
    """Extract structured function context using ContextExtractor.

    Returns a dict with keys: function_name, source_code, loops,
    existing_annotations, callee_layers, callers, global_vars.
    """
    from verdict.analysis.context_extractor import ContextExtractor

    node = call_graph_dict.get(func_name)
    if node is None:
        return {"error": f"Function '{func_name}' not found in call graph"}

    extractor = ContextExtractor(max_callee_depth=depth)
    k_context = extractor.extract_k_layer_callees(node, call_graph_dict)

    # Build structured result -- inject loop name comments into source_code
    source_with_loop_names = _inject_loop_name_comments(node)

    loops = []
    for loop in getattr(node, "loops", []):
        loops.append({
            "name": loop.get("loop_name", ""),
            "source_code": loop.get("loop_source_code", ""),
            "location": loop.get("loop_extent", ()),
        })

    existing_annotations = {
        "preconditions": list(getattr(node, "pre_conditions", [])),
        "postconditions": list(getattr(node, "post_conditions", [])),
        "loop_contracts": dict(getattr(node, "loop_contracts", {})),
        "assigns": list(getattr(node, "assigns", [])),
    }

    callee_layers = []
    for layer in k_context.layers:
        funcs = []
        for fn in layer.functions:
            entry: dict = {
                "name": fn.func_name,
                "source_code": fn.source_code,
                "existing_contracts": {
                    "preconditions": list(getattr(fn, "pre_conditions", [])),
                    "postconditions": list(getattr(fn, "post_conditions", [])),
                    "loop_contracts": dict(getattr(fn, "loop_contracts", {})),
                    "assigns": list(getattr(fn, "assigns", [])),
                },
            }
            funcs.append(entry)
        callee_layers.append({"depth": layer.depth, "functions": funcs})

    callers = None
    if include_callers and hasattr(node, "parents"):
        callers = [
            {
                "name": p.func_name,
                "source_code": p.source_code,
                "existing_contracts": {
                    "preconditions": list(getattr(p, "pre_conditions", [])),
                    "postconditions": list(getattr(p, "post_conditions", [])),
                    "loop_contracts": dict(getattr(p, "loop_contracts", {})),
                    "assigns": list(getattr(p, "assigns", [])),
                },
            }
            for p in node.parents
        ]

    global_vars = list(getattr(node, "global_vars_used", []))

    # Prepend global variable declarations as C code so the LLM sees
    # them in the same form it sees the function body — this makes it
    # natural to write correct ACSL (e.g. \valid_read(tbl + (0..N-1)))
    # without an extra JSON-to-ACSL translation step.
    global_decls = []
    for gv in global_vars:
        decl = gv.get("definition_source", "") if isinstance(gv, dict) else ""
        if decl:
            first_line = decl.split("\n")[0].rstrip()
            if first_line.endswith("="):
                first_line = first_line[:-1].rstrip()
            if not first_line.endswith(";"):
                first_line += ";"
            global_decls.append(first_line)
    if global_decls:
        header = "// Global variables used by this function:\n" + "\n".join(global_decls) + "\n\n"
        source_with_loop_names = header + source_with_loop_names

    return {
        "function_name": func_name,
        "source_code": source_with_loop_names,
        "loops": loops,
        "existing_annotations": existing_annotations,
        "callee_layers": callee_layers,
        "callers": callers,
        "global_vars": global_vars,
    }


# ---------------------------------------------------------------------------
# Syntax checking
# ---------------------------------------------------------------------------


def _run_syntax_check_sync(cil_file_path: str) -> tuple[bool, list[str]]:
    """Synchronous ACSL syntax check (called via asyncio.to_thread)."""
    from verdict.framac.wp_syntax_checker import WPSyntaxChecker

    checker = WPSyntaxChecker()
    checker.set_source_file_path(cil_file_path)
    has_errors, error_list = checker.check_syntax()
    return (not has_errors, error_list)  # invert: True = valid


async def run_syntax_check(cil_file_path: str) -> tuple[bool, list[str]]:
    """Check ACSL syntax asynchronously. Returns (valid, errors)."""
    return await asyncio.to_thread(_run_syntax_check_sync, cil_file_path)


# ---------------------------------------------------------------------------
# Semantic filtering
# ---------------------------------------------------------------------------


def extract_function_params(source_code: str) -> list[dict[str, Any]]:
    """Parse a C function signature into ``[{name, type}]`` parameter dicts.

    The call-graph node carries no structured parameter list — the only
    source of truth is the signature in ``source_code``. The semantic
    filter needs name/type pairs to detect e.g. ``&len`` on a by-value
    parameter, so we recover them here.
    """
    import re

    if not source_code:
        return []
    # Parameter list = text inside the first top-level (...) of the signature.
    open_paren = source_code.find("(")
    if open_paren < 0:
        return []
    depth = 0
    close = -1
    for i in range(open_paren, len(source_code)):
        if source_code[i] == "(":
            depth += 1
        elif source_code[i] == ")":
            depth -= 1
            if depth == 0:
                close = i
                break
    if close < 0:
        return []
    param_str = source_code[open_paren + 1 : close].strip()
    if not param_str or param_str == "void":
        return []

    # Split on top-level commas (respect nested parens/angle brackets).
    parts: list[str] = []
    buf: list[str] = []
    pd = bd = 0
    for ch in param_str:
        if ch == "," and pd == 0 and bd == 0:
            parts.append("".join(buf))
            buf = []
            continue
        if ch in "(":
            pd += 1
        elif ch == ")":
            pd -= 1
        elif ch == "<":
            bd += 1
        elif ch == ">":
            bd -= 1
        buf.append(ch)
    if buf:
        parts.append("".join(buf))

    params: list[dict[str, Any]] = []
    for part in parts:
        tokens = " ".join(part.split()).split()
        if len(tokens) < 2:
            continue
        last = tokens[-1]
        name = last.lstrip("*")
        if not name:  # the param name was a separate '*' token group
            name = tokens[-2]
            ptype = " ".join(tokens[:-1])
        else:
            ptype = " ".join(tokens[:-1])
            if last.startswith("*"):
                ptype += " *"
        params.append({"name": name, "type": ptype.strip()})
    return params


def run_semantic_filter(
    preconditions: list[str],
    func_params: list[dict[str, Any]],
) -> tuple[bool, list[str]]:
    """Run semantic filter on preconditions. Returns (valid, errors)."""
    from verdict.framac.annotation_semantic_filter import (
        check_annotations,
        param_names_from_params,
    )

    return check_annotations(
        preconditions, param_names_from_params(func_params), label="Precondition",
    )


def run_semantic_filter_all(
    annotations: dict[str, Any],
    func_params: list[dict[str, Any]],
) -> tuple[bool, list[str]]:
    """Run semantic filter on ALL annotation types. Returns (valid, errors)."""
    from verdict.framac.annotation_semantic_filter import (
        check_annotations,
        param_names_from_params,
    )

    names = param_names_from_params(func_params)
    all_errors: list[str] = []

    for key, label in [
        ("preconditions", "Precondition"),
        ("postconditions", "Postcondition"),
        ("assigns", "Assigns"),
    ]:
        clauses = annotations.get(key, [])
        if clauses:
            _, errors = check_annotations(clauses, names, label=label)
            all_errors.extend(errors)

    loop_contracts = annotations.get("loop_contracts", {})
    for loop_name, invariants in loop_contracts.items():
        if invariants:
            _, errors = check_annotations(
                invariants, names, label=f"Loop({loop_name})",
            )
            all_errors.extend(errors)

    return (not all_errors, all_errors)


# ---------------------------------------------------------------------------
# Weakening cascade effects
# ---------------------------------------------------------------------------


def compute_weakening_effects(
    call_graph_dict: dict[str, Any],
    func_name: str,
) -> dict[str, Any]:
    """Compute static graph effects of weakening a precondition.

    Returns affected callers/callees (no WP -- pure graph analysis).
    """
    node = call_graph_dict.get(func_name)
    if node is None:
        return {"error": f"Function '{func_name}' not found"}

    affected_callers = [p.func_name for p in getattr(node, "parents", [])]
    affected_callees = [c.func_name for c in getattr(node, "children", [])]

    return {
        "affected_callers": affected_callers,
        "affected_callees": affected_callees,
    }
