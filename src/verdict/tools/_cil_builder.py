"""CIL file assembly for Frama-C WP verification.

Builds annotated CIL files from ExtendedCallGraphNodes and agent-provided
ACSL annotations. Deep copies all nodes to prevent shared state mutation.
"""

from __future__ import annotations

import copy
import os
from typing import Any


def _apply_annotations_to_node(
    node: Any,
    annotations: dict[str, Any],
) -> None:
    """Apply agent-provided annotations to a deep-copied ExtendedCallGraphNode."""
    # `or []` / `or {}`: tolerate explicit nulls in agent JSON (an agent
    # sending {"preconditions": null} reaches here on tools whose schema is
    # an untyped object).
    node.pre_conditions = list(annotations.get("preconditions") or [])
    node.post_conditions = list(annotations.get("postconditions") or [])
    node.assigns = list(annotations.get("assigns") or [])
    lc = annotations.get("loop_contracts") or {}
    if lc:
        node.loop_contracts = {}
        for loop_name, contracts in lc.items():
            node.update_loop_contracts({loop_name: list(contracts)})


def build_annotated_cil_file(
    project_ctx: Any,
    func_name: str,
    annotations: dict[str, Any],
    callee_contracts: dict[str, dict[str, Any]],
    agent_id: str,
) -> str:
    """Assemble an annotated CIL file for Frama-C WP verification.

    Returns the path to the temporary .c file.
    """
    from verdict.annotation.function_annotator import FunctionAnnotator
    from verdict.postprocessing.cil_assembler import AnnotatedCILFileAssembler

    call_graph = project_ctx.call_graph_dict
    node = call_graph.get(func_name)
    if node is None:
        raise ValueError(f"Function '{func_name}' not in call graph")

    # Deep copy to avoid mutating shared state
    node_copy = copy.deepcopy(node)
    _apply_annotations_to_node(node_copy, annotations)

    annotator = FunctionAnnotator(project_ctx.function_declarations_map)

    # Build annotated main function
    success, main_annotated = annotator.get_function_with_annotations(node_copy)
    if not success:
        raise RuntimeError(f"Failed to annotate function '{func_name}'")

    # Build callee stubs: declaration + empty body + contracts
    callee_stubs: list[str] = []
    for callee in sorted(
        getattr(node_copy, "children", []), key=lambda c: c.extent[0]
    ):
        callee_copy = copy.deepcopy(callee)
        cname = callee_copy.func_name
        if cname in callee_contracts:
            _apply_annotations_to_node(callee_copy, callee_contracts[cname])
        success, stub = annotator.get_callee_declarations_with_contracts(callee_copy)
        if success and stub:
            callee_stubs.append(stub)

    annotated_functions = callee_stubs + [main_annotated]

    output_path = os.path.join(
        project_ctx.work_dir, f".verdict_agent_{agent_id}.c"
    )
    assembler = AnnotatedCILFileAssembler()

    # Three-layer assembly (avoids static redefinition):
    #   1. Headers: #include directives from CIL file
    #   2. Ordered declarations: typedefs, globals, forward decls
    #   3. Annotated functions: target + callee stubs
    if project_ctx.cil_headers or project_ctx.ordered_declarations:
        headers = list(project_ctx.cil_headers)
        filtered = getattr(project_ctx, "filtered_declarations", {})
        if func_name in filtered:
            decls = list(filtered[func_name])
        else:
            decls = list(project_ctx.ordered_declarations)
    else:
        # Fallback: include the full source file (legacy behavior)
        headers = [f'#include "{project_ctx.source_file}"']
        decls = []

    assembler.assemble_annotated_cil(
        headers, decls, annotated_functions, output_path
    )
    return output_path


def label_preconditions(
    preconditions: list[str],
    label: str = "precondition_test",
) -> list[str]:
    """Inject label into requires clauses.

    'requires x > 0;' -> 'requires precondition_test: x > 0;'

    Follows reference_implementation/annotation/function_annotator.py:
    _assign_precondition_test_name.
    """
    labeled = []
    for pre in preconditions:
        idx = pre.find("requires")
        if idx >= 0:
            after = idx + len("requires")
            labeled.append(pre[:after] + f" {label}:" + pre[after:])
        else:
            labeled.append(pre)
    return labeled


def build_annotated_cil_file_with_labeled_callee(
    project_ctx: Any,
    func_name: str,
    annotations: dict[str, Any],
    callee_name: str,
    callee_contracts: dict[str, Any],
    agent_id: str,
    extra_callee_contracts: dict[str, dict[str, Any]] | None = None,
) -> str:
    """Like build_annotated_cil_file but labels the TARGET callee's
    preconditions with 'precondition_test' so WP can target them via
    -wp-prop.

    `extra_callee_contracts` carries contracts for the OTHER callees in
    the same function — needed when sibling callees produce frame
    properties the target call site relies on (e.g. parse_id_len calls
    get_identifier before get_length; verifying get_length's call-site
    precondition needs get_identifier's frame postcondition in scope).

    Returns the path to the temporary .c file.
    """
    labeled_contracts = dict(callee_contracts)
    if "preconditions" in labeled_contracts:
        labeled_contracts["preconditions"] = label_preconditions(
            labeled_contracts["preconditions"]
        )
    all_contracts: dict[str, dict[str, Any]] = {}
    if extra_callee_contracts:
        for name, cc in extra_callee_contracts.items():
            if name == callee_name:
                continue  # target wins below
            all_contracts[name] = cc
    all_contracts[callee_name] = labeled_contracts
    return build_annotated_cil_file(
        project_ctx, func_name, annotations, all_contracts, agent_id
    )
