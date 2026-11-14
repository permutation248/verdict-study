"""verify_rte_assertions: targeted WP over a function's RTE obligations."""

from __future__ import annotations

from dataclasses import asdict
from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.result_types import RTEVerificationResult, VOResult, VOUpdate
from verdict.tools.verify._shared import (
    _draft_assumes,
    _resolve_callee_contracts_for_wp,
)


async def verify_rte_assertions(
    ctx: ToolContext,
    vo_ids: list[str],
) -> RTEVerificationResult:
    """Verify specific RTE assertions by VO ID via Frama-C WP -wp-prop.

    Targets individual RTE assertions (e.g. rte_1, rte_3) rather than
    verifying the whole function. Updates VOTracker per VO.

    The whole proof context comes from your working draft (saved by the
    last successful verify_function_contract): your own annotations, your
    raw `callee_contracts` hypothesis, and which callees are verified
    under that hypothesis (the draft's `assumed_callees`). Published specs
    win for every callee not in that assume set (soundness). To change
    the assume set, re-run verify_function_contract with `assume_callee`.

    Falls back to mock when no project_context.
    """
    ctx.logger.info("Verifying %d RTE assertion(s): %s", len(vo_ids), ", ".join(vo_ids))

    if ctx.project_context is None:
        # Mock path: all verified
        results = {
            vo_id: VOResult(verified=True, obligation=f"mock obligation for {vo_id}")
            for vo_id in vo_ids
        }
        # Update VOTracker if available
        if ctx.vo_tracker is not None:
            for vo_id in vo_ids:
                ctx.vo_tracker.mark_verified(vo_id)
        return RTEVerificationResult(
            results=results,
            all_verified=True,
            verified_count=len(vo_ids),
            total_count=len(vo_ids),
            summary=f"Mock: all {len(vo_ids)} RTE assertions verified.",
        )

    from verdict.tools._cil_builder import build_annotated_cil_file
    from verdict.tools._wp_runner import run_wp_rte_targeted, parse_wp_result

    trace = ctx.debugger.begin("verify_rte_assertions") if ctx.debugger else None

    try:
        if trace:
            trace.save_input({"vo_ids": vo_ids})

        # 1. Get current spec from registry → extract annotations
        spec = ctx.registry.read_working_spec(ctx.function_info.name)
        annotations: dict[str, Any] = {}
        if spec:
            annotations = {
                "preconditions": spec.preconditions,
                "postconditions": spec.postconditions,
                "loop_contracts": spec.loop_contracts,
                "assigns": spec.assigns,
            }

        # 2. Resolve callee contracts: agent's raw hypothesis from draft
        #    merged with callee.published_spec. Default: published wins.
        #    The assume set is the draft's — one proof context per draft.
        agent_cc = spec.callee_contracts if spec else {}
        assumed = _draft_assumes(spec)
        resolved_cc, overridden, _hyp, dropped = _resolve_callee_contracts_for_wp(
            agent_cc=agent_cc,
            callee_names=list(ctx.function_info.callees),
            registry=ctx.registry,
            assume_callee=assumed,
            allow_hypotheses=not ctx.ablation.assume,
            extern_callees=frozenset(ctx.function_info.extern_callees),
        )
        if dropped:
            ctx.logger.info(
                "assume ablation: callee hypotheses dropped for %s", dropped
            )
        ctx.logger.debug(
            "verify_rte_assertions %s: agent_cc=%d resolved=%d "
            "(overridden=%s, assumed=%s)",
            ctx.function_info.name,
            len(agent_cc),
            len(resolved_cc),
            overridden or "-",
            assumed or "-",
        )

        cil_path = build_annotated_cil_file(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            resolved_cc,
            ctx.agent_id,
        )
        if trace:
            trace.save_annotated_file(cil_path)

        # 3. Run targeted WP
        success, wp_output, command = await run_wp_rte_targeted(
            cil_path, ctx.project_context.include_paths, vo_ids,
            timeout=ctx.wp_timeout,
        )
        if trace:
            trace.save_command(command)
            trace.save_frama_c_output(wp_output, success)

        if not success:
            from verdict.tools._wp_runner import extract_wp_errors
            errors = extract_wp_errors(wp_output)
            result = RTEVerificationResult(
                summary=f"Frama-C WP execution failed: {'; '.join(errors)}",
            )
            if trace:
                trace.save_result(asdict(result))
            return result

        # 4. Parse WP results — annotation_status has per-vo_id
        parsed = parse_wp_result(wp_output)
        annotation_status = parsed.get("annotation_status", {})
        statuses = parsed.get("statuses", {})
        obligations = parsed.get("obligations", {})

        from verdict.tools._wp_runner import _REFUTED_STATUSES

        results: dict[str, VOResult] = {}
        verified_count = 0
        for vo_id in vo_ids:
            verified = annotation_status.get(vo_id, False)
            block = obligations.get(vo_id, "")
            # Core verdict is verified/failed; failed carries the full WP
            # obligation. Only a genuine refutation is flagged [Invalid].
            if verified:
                obligation = "verified"
            else:
                tag = " [Invalid]" if statuses.get(vo_id) in _REFUTED_STATUSES else ""
                obligation = f"failed{tag}" + (f": {block}" if block else "")
            results[vo_id] = VOResult(verified=verified, obligation=obligation)
            if verified:
                verified_count += 1

        # 5. Log per-VO results
        for vo_id, vo_result in results.items():
            tag = "VERIFIED" if vo_result.verified else "FAILED"
            ctx.logger.info("  %s: %s", vo_id, tag)

        # 6. Update VOTracker + collect VO updates
        vo_updates: list[VOUpdate] = []
        if ctx.vo_tracker is not None:
            for vo_id, vo_result in results.items():
                new_status = "verified" if vo_result.verified else "failed"
                if vo_result.verified:
                    ctx.vo_tracker.mark_verified(vo_id)
                else:
                    ctx.vo_tracker.mark_failed(vo_id)
                vo_updates.append(VOUpdate(vo_id=vo_id, status=new_status))

        all_verified = verified_count == len(vo_ids)
        summary = (
            f"All {len(vo_ids)} RTE assertions verified."
            if all_verified
            else f"{verified_count}/{len(vo_ids)} RTE assertions verified."
        )
        ctx.logger.info("RTE: %d/%d verified", verified_count, len(vo_ids))

        result = RTEVerificationResult(
            results=results,
            all_verified=all_verified,
            verified_count=verified_count,
            total_count=len(vo_ids),
            summary=summary,
            vo_updates=vo_updates,
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    except Exception as exc:
        ctx.logger.exception("verify_rte_assertions failed")
        return RTEVerificationResult(
            summary=f"Internal error: {exc}",
        )


# ---------------------------------------------------------------------------
# Tool 6: verify_callsite_preconditions
# ---------------------------------------------------------------------------


