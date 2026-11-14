"""Read-only inspection tools: ACSL syntax check and function context."""

from __future__ import annotations

from dataclasses import asdict
from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.result_types import (
    FunctionContext,
    SyntaxCheckResult,
    SyntaxError_,
)


async def check_acsl_syntax(
    ctx: ToolContext,
    annotations: dict[str, Any],
) -> SyntaxCheckResult:
    """Quick ACSL syntax check before full verification.

    Falls back to mock when no project_context.
    """
    ctx.logger.debug("check_acsl_syntax %s", ctx.function_info.name)

    from verdict.tools._mechanical import validate_annotations_shape
    shape_errors = validate_annotations_shape(annotations)
    if shape_errors:
        return SyntaxCheckResult(
            valid=False,
            errors=[SyntaxError_(message=m) for m in shape_errors],
        )

    if ctx.project_context is None:
        ctx.logger.info("ACSL syntax: valid (mock)")
        return SyntaxCheckResult(valid=True)

    from verdict.tools._cil_builder import build_annotated_cil_file
    from verdict.tools._mechanical import run_syntax_check

    trace = ctx.debugger.begin("check_acsl_syntax") if ctx.debugger else None

    try:
        if trace:
            trace.save_input({"annotations": annotations})

        cil_path = build_annotated_cil_file(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            {},
            ctx.agent_id,
        )
        if trace:
            trace.save_annotated_file(cil_path)
            trace.save_command(["-kernel-warn-key", "annot-error"])

        valid, errors = await run_syntax_check(cil_path)
        if valid:
            ctx.logger.info("ACSL syntax: valid")
        else:
            ctx.logger.info("ACSL syntax: %d error(s)", len(errors))
        result = SyntaxCheckResult(
            valid=valid,
            errors=[SyntaxError_(message=e) for e in errors],
        )
        if trace:
            trace.save_frama_c_output("\n".join(errors) if errors else "No errors", valid)
            trace.save_result(asdict(result))
        return result
    except Exception as exc:
        ctx.logger.exception("check_acsl_syntax failed")
        return SyntaxCheckResult(
            valid=False,
            errors=[SyntaxError_(message=str(exc))],
        )


# ---------------------------------------------------------------------------
# Tool 3: get_function_context
# ---------------------------------------------------------------------------


async def get_function_context(
    ctx: ToolContext,
    depth: int = 1,
    include_callers: bool = False,
) -> FunctionContext:
    """Get structured context for the agent's function.

    Falls back to minimal info from FunctionInfo when no project_context.
    """
    ctx.logger.debug(
        "get_function_context %s depth=%d", ctx.function_info.name, depth
    )

    trace = ctx.debugger.begin("get_function_context") if ctx.debugger else None
    if trace:
        trace.save_input({"depth": depth, "include_callers": include_callers})

    if ctx.project_context is None:
        result = FunctionContext(
            function_name=ctx.function_info.name,
            source_code=ctx.function_info.source_code,
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    from verdict.tools._mechanical import extract_context

    raw = extract_context(
        ctx.project_context.call_graph_dict,
        ctx.function_info.name,
        depth=depth,
        include_callers=include_callers,
    )

    if "error" in raw:
        result = FunctionContext(
            function_name=ctx.function_info.name,
            source_code=ctx.function_info.source_code,
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    result = FunctionContext(
        function_name=raw["function_name"],
        source_code=raw["source_code"],
        loops=raw.get("loops", []),
        existing_annotations=raw.get("existing_annotations", {}),
        callee_layers=raw.get("callee_layers", []),
        callers=raw.get("callers"),
        global_vars=raw.get("global_vars", []),
    )
    n_callees = sum(len(layer) for layer in result.callee_layers) if result.callee_layers else 0
    n_loops = len(result.loops) if result.loops else 0
    ctx.logger.info(
        "Context: %d callees, %d loops",
        n_callees, n_loops,
    )
    if trace:
        trace.save_result(asdict(result))
    return result


# ---------------------------------------------------------------------------
# Tool 4: apply_precondition_weakening
# ---------------------------------------------------------------------------


