"""apply_precondition_weakening: weaken a precondition and re-verify the cascade."""

from __future__ import annotations

from dataclasses import asdict
from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.result_types import VerificationResult, WeakeningResult
from verdict.tools.verify._shared import (
    _draft_assumes,
    _resolve_callee_contracts_for_wp,
)


async def apply_precondition_weakening(
    ctx: ToolContext,
    original_precondition: str,
    weakened_precondition: str | None = None,
) -> WeakeningResult:
    """Apply precondition weakening and compute local cascade effects.

    After weakening a precondition, re-verifies the function's remaining
    specs (loop contracts → postconditions → assigns) in 3 isolated steps
    to detect which specs are no longer provable.

    Each step builds a CIL with only the relevant spec category (plus specs
    already verified in prior steps), mirroring the reference implementation's
    deepcopy fix that prevents the "already verified" filter from silently
    skipping re-verification.

    The proof context comes from your working draft: the raw
    `callee_contracts` hypothesis and the assume set (`assumed_callees`)
    are read from it and carried into the weakened draft unchanged —
    weakening edits one precondition, not the context. Published specs win
    for non-assumed callees, as everywhere.

    Cross-agent notification is the agent's job (via messaging tools).

    Falls back to no-op when no project_context.
    """
    if weakened_precondition:
        ctx.logger.info(
            "Weakening: '%s' -> '%s'",
            original_precondition[:50],
            weakened_precondition[:50],
        )
    else:
        ctx.logger.info("Removing precondition: '%s'", original_precondition[:50])

    trace = ctx.debugger.begin("apply_precondition_weakening") if ctx.debugger else None
    if trace:
        trace.save_input({
            "original_precondition": original_precondition,
            "weakened_precondition": weakened_precondition,
        })

    if ctx.project_context is None:
        result = WeakeningResult(success=True)
        if trace:
            trace.save_result(asdict(result))
        return result

    from verdict.tools._mechanical import compute_weakening_effects
    from verdict.tools._vo_reverifier import (
        VOReVerificationResult,
        reverify_internal_vos,
        reverify_specs_after_weakening,
    )

    # 1. Static graph analysis: affected callers/callees
    graph_effects = compute_weakening_effects(
        ctx.project_context.call_graph_dict,
        ctx.function_info.name,
    )

    if "error" in graph_effects:
        result = WeakeningResult(success=False)
        if trace:
            trace.save_result(asdict(result))
        return result

    # 2. Read current spec from registry
    spec = ctx.registry.read_working_spec(ctx.function_info.name)
    if not spec:
        # No spec registered yet — nothing to re-verify
        result = WeakeningResult(
            success=True,
            affected_callers=graph_effects["affected_callers"],
            affected_callees=graph_effects["affected_callees"],
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    # 3. Apply weakening: replace original precondition with weakened version
    new_preconditions: list[str] = []
    for pre in spec.preconditions:
        if pre == original_precondition:
            if weakened_precondition is not None:
                new_preconditions.append(weakened_precondition)
            # else: removed entirely (not added)
        else:
            new_preconditions.append(pre)

    weakened_annotations = {
        "preconditions": new_preconditions,
        "postconditions": list(spec.postconditions),
        "loop_contracts": dict(spec.loop_contracts),
        "assigns": list(spec.assigns),
    }

    # 4. Write weakened draft to registry so VO tracker and registry stay
    #    consistent, and subsequent registry-driven tools see the new
    #    preconditions. The proof-context bookkeeping (callee_contracts
    #    hypothesis + assumed_callees) carries over unchanged: weakening
    #    edits one precondition, and dropping the assume record here would
    #    publish without the based_on=0 debt anchor.
    from verdict.core.types import FunctionSpec, SpecStatus
    weakened_spec = FunctionSpec(
        function_name=ctx.function_info.name,
        preconditions=new_preconditions,
        postconditions=list(spec.postconditions),
        loop_contracts=dict(spec.loop_contracts),
        assigns=list(spec.assigns),
        callee_contracts={
            name: dict(cc) for name, cc in spec.callee_contracts.items()
        },
        assumed_callees=list(spec.assumed_callees),
        status=SpecStatus.DRAFT,
    )
    await ctx.registry.write_spec(ctx.agent_id, weakened_spec)
    ctx.logger.info("Saved weakened draft to registry")

    # Exception safety: the draft just changed the proof context, so every
    # proof established under the old precondition is stale RIGHT NOW. Reset
    # VOs and invalidate the spec VO BEFORE re-verification — if any step
    # below raises, the publish gate then blocks on PENDING state instead of
    # shipping old-world proofs under the weakened draft. The reverification
    # steps re-mark whatever still holds.
    from verdict.tools.verify._shared import _reset_vos_on_context_change
    _reset_vos_on_context_change(ctx.vo_tracker, spec, weakened_spec)
    if ctx.vo_tracker is not None:
        ctx.vo_tracker.invalidate_spec(
            f"precondition weakened ('{original_precondition[:60]}') — "
            f"contract re-established by apply_precondition_weakening"
        )

    # 5. Resolve callee contracts via the shared helper. Uses the agent's
    #    raw hypothesis from the draft, merged with callee.published_spec;
    #    the assume set is the draft's — one proof context per draft.
    agent_cc = spec.callee_contracts if spec else {}
    assumed = _draft_assumes(spec)
    callee_contracts, overridden, _hyp, dropped = _resolve_callee_contracts_for_wp(
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
        "apply_precondition_weakening %s: agent_cc=%d resolved=%d "
        "(overridden=%s, assumed=%s)",
        ctx.function_info.name,
        len(agent_cc),
        len(callee_contracts),
        overridden or "-",
        assumed or "-",
    )

    # 6. Re-verify specs (worklist: loop → post → assigns) with weakened pre
    ctx.logger.info("Re-verifying specs after weakening...")
    try:
        reverify = await reverify_specs_after_weakening(
            ctx.project_context,
            ctx.function_info.name,
            weakened_annotations,
            callee_contracts,
            ctx.agent_id,
            timeout=ctx.wp_timeout,
        )
    except Exception as exc:
        ctx.logger.exception("reverify_specs_after_weakening failed")
        result = WeakeningResult(
            success=False,
            affected_callers=graph_effects["affected_callers"],
            affected_callees=graph_effects["affected_callees"],
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    # 7. Save worklist trace
    wl_result = reverify.get("worklist_result")
    if trace:
        if wl_result:
            trace.save_worklist_log(wl_result.worklist_log)
        trace.save_result({
            "removed_loop_contracts": reverify["removed_loop_contracts"],
            "removed_postconditions": reverify["removed_postconditions"],
            "removed_assigns": reverify["removed_assigns"],
        })

    # 7b. Keep the draft truthful to what re-verification established:
    #     when clauses were removed, rewrite the draft with the surviving
    #     set (context bookkeeping unchanged). Publish ships the draft
    #     verbatim, so it must not contain clauses that just failed.
    removed_any = (
        reverify["removed_postconditions"]
        or reverify["removed_loop_contracts"]
        or reverify["removed_assigns"]
    )
    if wl_result is not None and removed_any:
        surviving_spec = FunctionSpec(
            function_name=ctx.function_info.name,
            preconditions=new_preconditions,
            postconditions=list(wl_result.verified_postconditions),
            loop_contracts=dict(wl_result.verified_loop_contracts),
            assigns=list(wl_result.verified_assigns),
            callee_contracts={
                name: dict(cc) for name, cc in spec.callee_contracts.items()
            },
            assumed_callees=list(spec.assumed_callees),
            status=SpecStatus.DRAFT,
        )
        await ctx.registry.write_spec(ctx.agent_id, surviving_spec)
        ctx.logger.info(
            "Saved post-reverification draft (surviving specs only)"
        )
    # The worklist completed: the draft now holds exactly the clause set it
    # verified (full set if nothing was removed, surviving set otherwise) —
    # the spec VO invalidated at the top is re-established.
    if wl_result is not None and ctx.vo_tracker is not None:
        ctx.vo_tracker.mark_spec_verified()

    # 8. Re-verify internal VOs (RTE + call-site)
    ctx.logger.info("Re-verifying internal VOs after weakening...")
    vo_reverify = VOReVerificationResult()  # empty default
    if ctx.vo_tracker is not None and wl_result is not None:
        surviving_annotations = {
            "preconditions": weakened_annotations["preconditions"],
            "postconditions": wl_result.verified_postconditions,
            "loop_contracts": wl_result.verified_loop_contracts,
            "assigns": wl_result.verified_assigns,
        }
        try:
            vo_reverify = await reverify_internal_vos(
                ctx.project_context, ctx.function_info.name,
                surviving_annotations, callee_contracts, ctx.agent_id,
                ctx.vo_tracker, ctx.registry,
                timeout=ctx.wp_timeout,
                logger=ctx.logger,
            )
        except Exception as exc:
            ctx.logger.exception("reverify_internal_vos failed")

    if trace and vo_reverify.log:
        trace.save_vo_reverification_log(vo_reverify.log)

    all_vo_transitions = {**vo_reverify.rte_transitions, **vo_reverify.callsite_transitions}
    affected_vo_nodes = [
        vo_id for vo_id, info in all_vo_transitions.items()
        if info["status_after"] == "failed"
    ]
    total_still = vo_reverify.total_reverified - vo_reverify.rte_newly_failed - vo_reverify.callsite_newly_failed
    total_failed = vo_reverify.rte_newly_failed + vo_reverify.callsite_newly_failed
    vo_summary = (
        f"{total_still} VOs still verified, {total_failed} newly failed"
        if vo_reverify.total_reverified > 0
        else "No VOs to re-verify"
    )

    result = WeakeningResult(
        success=True,
        removed_postconditions=reverify["removed_postconditions"],
        removed_loop_contracts=reverify["removed_loop_contracts"],
        removed_assigns=reverify["removed_assigns"],
        affected_callers=graph_effects["affected_callers"],
        affected_callees=graph_effects["affected_callees"],
        verified_loop_contracts=wl_result.verified_loop_contracts if wl_result else {},
        verified_postconditions=wl_result.verified_postconditions if wl_result else [],
        verified_assigns=wl_result.verified_assigns if wl_result else [],
        worklist_log=wl_result.worklist_log if wl_result else None,
        affected_vo_nodes=affected_vo_nodes,
        affected_vo_details=all_vo_transitions,
        vo_reverification_summary=vo_summary,
    )
    if trace:
        trace.save_result(asdict(result))
    return result


# ---------------------------------------------------------------------------
# Tool 5: verify_rte_assertions
# ---------------------------------------------------------------------------


