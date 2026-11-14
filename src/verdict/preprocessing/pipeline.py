"""Reusable preprocessing pipeline: raw .c -> ProjectContext.

Consolidates the pipeline logic previously duplicated in
Orchestrator._build_from_source() and tests/helpers/real_context_builder.py.
"""

from __future__ import annotations

import logging
import os
import shutil
import tempfile
from typing import Any

from verdict.analysis.extended_call_graph import ExtendedCallGraph
from verdict.analysis.rte_assertion_order_assigner import (
    RTE_LABEL_RE,
    RTEAssertionOrderAssigner,
)
from verdict.core.project_context import ProjectContext

logger = logging.getLogger(__name__)


def preprocess(
    source_file: str,
    header_files: list[str] | None = None,
    cpp_extra_args: str | None = None,
) -> tuple[ProjectContext, ExtendedCallGraph]:
    """Full pipeline: raw .c -> Frama-C -rte -> ECG -> RTE renumber -> ProjectContext.

    1. Copy source + headers to temp work_dir
    2. RTEAnnotatedCILPreprocessor -> CIL file
       (pipe cpp_extra_args to FramaCRunner)
    3. ExtendedCallGraph parse (raw RTE names)
    4. RTEAssertionOrderAssigner -> renumber in-place
    5. ExtendedCallGraph re-parse (numbered RTEs)
    6. Extract CIL headers + ordered declarations
    7. Extract RTE VOs per function
    8. Build and return ProjectContext

    Returns (ProjectContext, ExtendedCallGraph) for inspection.
    """
    abs_source = os.path.abspath(source_file)
    work_dir = tempfile.mkdtemp(prefix="verdict_")

    # 1. Copy source + headers to work dir
    src_copy = os.path.join(work_dir, os.path.basename(abs_source))
    shutil.copy2(abs_source, src_copy)

    # Auto-copy .h files from the same directory as the source file
    source_dir = os.path.dirname(abs_source)
    for entry in os.listdir(source_dir):
        if entry.endswith(".h"):
            src_h = os.path.join(source_dir, entry)
            dst_h = os.path.join(work_dir, entry)
            if not os.path.exists(dst_h):
                shutil.copy2(src_h, dst_h)

    if header_files:
        for hf in header_files:
            shutil.copy2(os.path.abspath(hf), os.path.join(work_dir, os.path.basename(hf)))

    # 2. Preprocess via Frama-C RTE
    cil_file = _preprocess_source(src_copy, cpp_extra_args)
    logger.info("CIL file generated: %s", cil_file)

    # 3. Initial ECG parse (raw RTE names)
    logger.info("Building initial extended call graph")
    ecg = ExtendedCallGraph(cil_file)
    call_graph_dict = ecg.generate_extended_call_graph()
    logger.info("ECG: %d functions", len(call_graph_dict))

    # 4. RTE order assignment (renumber in-place)
    assigner = RTEAssertionOrderAssigner(cil_file, call_graph_dict)
    success, count = assigner.assign_assertion_orders()
    if success:
        logger.info("Assigned orders to %d RTE assertions", count)
    else:
        logger.warning("RTE assertion order assignment failed")

    # 5. Re-parse ECG with numbered RTEs
    logger.info("Re-parsing extended call graph with numbered RTEs")
    ecg = ExtendedCallGraph(cil_file)
    call_graph_dict = ecg.generate_extended_call_graph()

    # 6. Extract CIL headers and ordered declarations
    cil_headers = _extract_cil_headers(cil_file)
    ordered_decls = _build_ordered_declarations(ecg)

    # 7. Extract RTE VOs per function
    rte_vos = _extract_rte_vos(call_graph_dict)

    # 7b. Extern prototypes (declared, never defined) are environment, not
    # verification targets: they keep their graph node for context and
    # compilation, but downstream no agent is created for them.
    extern_functions = {
        name
        for name, node in call_graph_dict.items()
        if not getattr(node, "is_definition", True)
    }
    if extern_functions:
        logger.info(
            "Extern environment functions (no body, no agent): %s",
            sorted(extern_functions),
        )

    # 8. Build ProjectContext
    project_ctx = ProjectContext(
        source_file=cil_file,
        source_dir=work_dir,
        call_graph_dict=call_graph_dict,
        function_declarations_map=ecg.function_declarations_map,
        include_paths=[work_dir],
        work_dir=work_dir,
        cil_headers=cil_headers,
        ordered_declarations=ordered_decls,
        rte_vos_by_function=rte_vos,
        cpp_extra_args=cpp_extra_args,
        extern_functions=extern_functions,
    )

    # 9. Global-deps: per-function filtered declarations
    filtered = _compute_filtered_declarations(cil_file, call_graph_dict, work_dir)
    if filtered:
        project_ctx.filtered_declarations = filtered
        logger.info("Global-deps: %d/%d functions have filtered declarations",
                    len(filtered), len(call_graph_dict))

    # Try VO pre-computation (optional — may not be available)
    _precompute_vos(project_ctx, call_graph_dict)

    logger.info(
        "Built ProjectContext: %d functions, %d with RTEs, work_dir=%s",
        len(call_graph_dict),
        len(rte_vos),
        work_dir,
    )
    return project_ctx, ecg


def _preprocess_source(source_file: str, cpp_extra_args: str | None = None) -> str:
    """Run Frama-C RTE preprocessing. Returns path to CIL file."""
    with open(source_file, "r") as f:
        first_line = f.readline()
    if first_line.strip().startswith("/* Generated by Frama-C"):
        logger.info("Source is already a CIL file, skipping preprocessing")
        return source_file

    from verdict.preprocessing.rte_annotated_cil_preprocessor import (
        RTEAnnotatedCILPreprocessor,
    )

    preprocessor = RTEAnnotatedCILPreprocessor(source_file)
    if cpp_extra_args:
        preprocessor.frama_c_runner.cpp_extra_args = cpp_extra_args
    if not preprocessor.generate_annotated_cil():
        raise RuntimeError(f"Frama-C preprocessing failed for {source_file}")
    return preprocessor.annotated_cil_file_path


def _extract_cil_headers(cil_file: str) -> list[str]:
    """Extract #include directives from CIL file."""
    headers: list[str] = []
    with open(cil_file, "r") as f:
        for line in f:
            stripped = line.strip()
            if stripped.startswith("#include"):
                headers.append(stripped)
            elif stripped and not stripped.startswith("/*") and not stripped.startswith("//"):
                if headers:
                    break
    return headers


def _build_ordered_declarations(ecg: ExtendedCallGraph) -> list[str]:
    """Build ordered declarations list from ECG data."""
    decls = []
    for d in ecg.ordered_declarations:
        src = d.get("source_code", "").strip()
        if src:
            decls.append(src)
    return decls


def _extract_rte_vos(call_graph_dict: dict[str, Any]) -> dict[str, list[str]]:
    """Extract RTE VO IDs per function from call graph nodes."""
    rte_vos: dict[str, list[str]] = {}
    for name, node in call_graph_dict.items():
        if node.rte_asserts:
            vo_ids = []
            for content, _extent in node.rte_asserts:
                match = RTE_LABEL_RE.search(content)
                if match:
                    vo_ids.append(match.group(1))
            if vo_ids:
                rte_vos[name] = vo_ids
    return rte_vos


def _precompute_vos(
    project_ctx: ProjectContext,
    call_graph_dict: dict[str, Any],
) -> None:
    """Pre-compute VO graph from call graph (optional)."""
    try:
        from verdict.analysis.verification_obligation_graph import (
            VerificationObligationGraph,
        )

        vo_graph_obj = VerificationObligationGraph()
        vo_graph_obj.generate_from_extended_call_graph(call_graph_dict)
        nodes = getattr(vo_graph_obj, "nodes_dict", {})

        rte_by_func: dict[str, list[str]] = {}
        cs_by_func: dict[str, list[Any]] = {}

        for vo_id, vo_node in nodes.items():
            fname = getattr(vo_node, "func_name", "")
            atype = getattr(vo_node, "assertion_type", None)
            atype_name = getattr(atype, "name", "") if atype else ""

            if atype_name == "RTE":
                rte_by_func.setdefault(fname, []).append(vo_id)
            elif atype_name == "CALL_SITE":
                cs_by_func.setdefault(fname, []).append(vo_node)

        project_ctx.vo_graph = nodes
        # Only override rte_vos_by_function if VO graph provides data
        if rte_by_func:
            project_ctx.rte_vos_by_function = rte_by_func
        project_ctx.call_sites_by_function = cs_by_func
        logger.info(
            "Pre-computed VOs: %d RTE functions, %d call-site functions",
            len(rte_by_func),
            len(cs_by_func),
        )
    except Exception:
        logger.warning(
            "VO pre-computation skipped (analysis modules unavailable)",
            exc_info=True,
        )


def _compute_filtered_declarations(
    cil_file: str,
    call_graph_dict: dict[str, Any],
    work_dir: str,
) -> dict[str, list[str]]:
    """Run frama-c global-deps plugin to get per-function minimal declarations.

    Returns a dict mapping function name -> list of filtered declaration strings.
    Returns empty dict on failure (caller falls back to full declarations).
    """
    import json
    import subprocess

    gd_dir = os.path.join(work_dir, "global_deps")
    os.makedirs(gd_dir, exist_ok=True)

    input_path = os.path.join(gd_dir, "input.json")
    output_path = os.path.join(gd_dir, "output.json")
    c_output_dir = os.path.join(gd_dir, "c_output")
    os.makedirs(c_output_dir, exist_ok=True)

    contexts = []
    for func_name, node in call_graph_dict.items():
        callees = [c.func_name for c in getattr(node, "children", [])]
        all_funcs = sorted(set([func_name] + callees))
        contexts.append({"id": func_name, "functions": all_funcs})

    with open(input_path, "w") as f:
        json.dump({"contexts": contexts}, f)

    # Build include args matching FramaCRunner
    cpp_parts = []
    source_dir = os.path.dirname(os.path.abspath(cil_file))
    cpp_parts.append(f"-I{source_dir}")
    parent = os.path.dirname(source_dir)
    if parent:
        cpp_parts.append(f"-I{parent}")
    cpp_args = " ".join(cpp_parts)

    cmd = [
        "frama-c",
        f"-cpp-extra-args={cpp_args}",
        cil_file,
        "-global-deps",
        "-gd-input", input_path,
        "-gd-output", output_path,
        "-gd-c-output", c_output_dir,
    ]

    try:
        logger.info("Running global-deps plugin for %d functions", len(contexts))
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
        if result.returncode != 0:
            logger.warning("global-deps plugin failed: %s", result.stderr[:500])
            return {}
    except FileNotFoundError:
        logger.warning("global-deps plugin not available (frama-c not found)")
        return {}
    except subprocess.TimeoutExpired:
        logger.warning("global-deps plugin timed out")
        return {}

    filtered: dict[str, list[str]] = {}
    for ctx in contexts:
        func_name = ctx["id"]
        c_file = os.path.join(c_output_dir, f"{func_name}.c")
        if os.path.exists(c_file):
            with open(c_file, "r") as f:
                content = f.read().strip()
            if content:
                filtered[func_name] = [content]

    logger.info("global-deps: %d/%d functions filtered", len(filtered), len(contexts))
    return filtered
