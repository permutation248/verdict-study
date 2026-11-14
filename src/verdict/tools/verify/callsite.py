"""verify_callsite_preconditions: verify a callee's preconditions hold at each call site."""

from __future__ import annotations

from dataclasses import asdict
from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.result_types import (
    CallsiteVerificationResult,
    PreconditionResult,
    VOUpdate,
)
from verdict.tools.verify._shared import (
    _align_callsite_children,
    _draft_assumes,
    _resolve_callee_contracts_for_wp,
    _update_child_vos,
)


async def _verify_single_callee(
    ctx: ToolContext,
    callee_function: str,
) -> CallsiteVerificationResult:
    """Verify a single callee's preconditions at call sites.

    This is the core logic extracted from the original
    verify_callsite_preconditions, handling one callee at a time.
    """
    ctx.logger.info("Verifying call-site preconditions for callee '%s'", callee_function)

    # Refuse to touch the VO tracker when the callee has no authoritative
    # published spec — the call-site VO should stay PENDING and the
    # runtime will wake us again via `spec_published` once the callee
    # publishes.
    callee_entry = ctx.registry.get_by_function(callee_function)
    if callee_entry is None or callee_entry.published_spec is None:
        if callee_function in ctx.function_info.extern_callees:
            summary = (
                f"callee {callee_function!r} is an extern environment "
                f"function: it has no body, no agent, and will never "
                f"publish preconditions, so there are no call-site "
                f"obligations for it — nothing to verify here, ever. "
                f"If your own proofs need guarantees about it, state them "
                f"as callee_contracts[{callee_function!r}] "
                f"(postconditions/assigns) in verify_function_contract."
            )
        else:
            summary = (
                f"callee {callee_function!r} has no published spec yet; "
                f"call-site VOs left PENDING. Wait for the spec_published "
                f"wake reason, then read_function_spec(callee_function) and "
                f"re-run verify_callsite_preconditions."
            )
        ctx.logger.info(
            "verify_callsite_preconditions skipped: %s not published",
            callee_function,
        )
        return CallsiteVerificationResult(
            callee_function=callee_function, summary=summary,
        )

    # Pull the AUTHORITATIVE callee contracts from the registry. The agent's
    # `callee_contracts` parameter is intentionally ignored for preconditions
    # (and for post/assigns too): a caller cannot dictate what a callee
    # requires, and the published spec is the only sound source.
    published = callee_entry.published_spec
    registry_callee_contracts = {
        "preconditions": list(published.preconditions),
        "postconditions": list(published.postconditions),
        "assigns": list(published.assigns),
    }
    preconditions = registry_callee_contracts["preconditions"]
    if not preconditions:
        summary = (
            f"callee {callee_function!r} has published spec v{published.version} "
            f"but its preconditions list is empty — nothing to verify at the "
            f"call site. Marking call-site VOs as verified."
        )
        ctx.logger.info(
            "verify_callsite_preconditions: %s has no preconditions; "
            "treating all call-site VOs as verified",
            callee_function,
        )
        if ctx.vo_tracker is not None:
            for vo_id, entry in list(
                ctx.vo_tracker.parent_call_site_vos().items()
            ):
                if entry.related_function == callee_function:
                    # Callee dropped ALL preconditions: remove any stale
                    # children (for the now-removed preconditions) before
                    # marking the call site verified, else they linger as
                    # orphans and the parent never settles.
                    ctx.vo_tracker.remove_children(vo_id)
                    ctx.vo_tracker.mark_verified(vo_id)
        return CallsiteVerificationResult(
            callee_function=callee_function,
            all_verified=True,
            verified_count=0,
            total_count=0,
            summary=summary,
        )

    if ctx.project_context is None:
        # Mock path: all verified — update child VOs if they exist.
        results = [
            PreconditionResult(
                precondition=pre,
                verified=True,
                obligation=f"mock obligation for {pre}",
                precondition_index=i,
            )
            for i, pre in enumerate(preconditions)
        ]
        count = len(results)
        if ctx.vo_tracker is not None:
            _align_callsite_children(ctx.vo_tracker, callee_function, preconditions)
            _update_child_vos(ctx.vo_tracker, callee_function, results)
        return CallsiteVerificationResult(
            callee_function=callee_function,
            results=results,
            all_verified=True,
            verified_count=count,
            total_count=count,
            summary=f"Mock: all {count} call-site preconditions verified.",
        )

    from verdict.tools._cil_builder import build_annotated_cil_file_with_labeled_callee
    from verdict.tools._wp_runner import run_wp_callsite_targeted, parse_wp_callsite_result

    trace = ctx.debugger.begin("verify_callsite_preconditions") if ctx.debugger else None

    try:
        if trace:
            trace.save_input({"callee_function": callee_function})

        # 1. Get current spec from registry → extract annotations for self
        spec = ctx.registry.read_working_spec(ctx.function_info.name)
        annotations: dict[str, Any] = {}
        if spec:
            annotations = {
                "preconditions": spec.preconditions,
                "postconditions": spec.postconditions,
                "loop_contracts": spec.loop_contracts,
                "assigns": spec.assigns,
            }

        # 2. Resolve sibling callee contracts so frame properties from
        #    OTHER callees (called earlier in this function) are visible
        #    when WP proves the target callee's call-site preconditions.
        #    The assume set is the draft's — one proof context per draft.
        agent_cc = spec.callee_contracts if spec else {}
        assumed = _draft_assumes(spec)
        sibling_callees = [
            n for n in ctx.function_info.callees if n != callee_function
        ]
        sibling_cc, sibling_overridden, _hyp, dropped = _resolve_callee_contracts_for_wp(
            agent_cc=agent_cc,
            callee_names=sibling_callees,
            registry=ctx.registry,
            assume_callee=[n for n in assumed if n != callee_function],
            allow_hypotheses=not ctx.ablation.assume,
            extern_callees=frozenset(ctx.function_info.extern_callees),
        )
        if dropped:
            ctx.logger.info(
                "assume ablation: sibling hypotheses dropped for %s", dropped
            )
        ctx.logger.debug(
            "verify_callsite_preconditions %s target=%s siblings=%d "
            "(overridden=%s, assumed=%s)",
            ctx.function_info.name,
            callee_function,
            len(sibling_cc),
            sibling_overridden or "-",
            assumed or "-",
        )

        # 3. Build CIL with labeled target callee preconditions + sibling cc.
        cil_path = build_annotated_cil_file_with_labeled_callee(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            callee_function,
            registry_callee_contracts,
            ctx.agent_id,
            extra_callee_contracts=sibling_cc,
        )
        if trace:
            trace.save_annotated_file(cil_path)

        # 3. Run targeted WP on precondition_test label
        success, wp_output, command = await run_wp_callsite_targeted(
            cil_path, ctx.project_context.include_paths,
            timeout=ctx.wp_timeout,
        )
        if trace:
            trace.save_command(command)
            trace.save_frama_c_output(wp_output, success)

        if not success:
            from verdict.tools._wp_runner import extract_wp_errors
            errors = extract_wp_errors(wp_output)
            result = CallsiteVerificationResult(
                summary=f"Frama-C WP execution failed: {'; '.join(errors)}",
            )
            if trace:
                trace.save_result(asdict(result))
            return result

        # 4. Parse per-(call_site_line, precondition_index) results.
        parsed_results = parse_wp_callsite_result(
            wp_output, expected_preconditions=preconditions
        )

        results: list[PreconditionResult] = []
        verified_count = 0
        for pr in parsed_results:
            verified = pr.get("verified", False)
            results.append(PreconditionResult(
                precondition=pr.get("precondition", ""),
                verified=verified,
                obligation=pr.get("obligation", ""),
                call_site_line=pr.get("call_site_line", 0),
                precondition_index=pr.get("precondition_index", 0),
            ))
            if verified:
                verified_count += 1

        # If no parsed results, map back to original preconditions
        if not results:
            for i, pre in enumerate(preconditions):
                results.append(PreconditionResult(
                    precondition=pre,
                    verified=False,
                    obligation="No WP result found for this precondition.",
                    precondition_index=i,
                ))

        # 5. Log per-result
        for r in results:
            tag = "VERIFIED" if r.verified else "FAILED"
            line_info = f"line {r.call_site_line} " if r.call_site_line else ""
            ctx.logger.info(
                "  %spre %d (%s): %s",
                line_info, r.precondition_index, r.precondition[:50], tag,
            )

        total = len(results)
        all_verified = verified_count == total and total > 0
        summary = (
            f"All {total} call-site preconditions verified."
            if all_verified
            else f"{verified_count}/{total} call-site preconditions verified."
        )
        ctx.logger.info("Call-site %s: %d/%d verified", callee_function, verified_count, total)

        # 6. Align child VOs to the callee's CURRENT published preconditions,
        #    then apply this run's per-index verdicts. Aligning FIRST prunes
        #    stale children left when the callee REMOVED a precondition (e.g.
        #    v1 had 4, v2 has 3): without it the dropped index's child lingers
        #    as an undischargeable orphan that strands the parent and forces
        #    the agent to mark_vo_as_bug. This is the same alignment the
        #    reconcile layer performs on a callee publish event — applied here
        #    too because the agent may verify before/after that fires. sync
        #    preserves verified status for unchanged preconditions;
        #    _update_child_vos then applies this run's WP verdicts.
        vo_updates: list[VOUpdate] = []
        if ctx.vo_tracker is not None:
            _align_callsite_children(ctx.vo_tracker, callee_function, preconditions)
            vo_updates = _update_child_vos(ctx.vo_tracker, callee_function, results)

        result = CallsiteVerificationResult(
            callee_function=callee_function,
            results=results,
            all_verified=all_verified,
            verified_count=verified_count,
            total_count=total,
            summary=summary,
            vo_updates=vo_updates,
        )
        if trace:
            trace.save_result(asdict(result))
        return result

    except Exception as exc:
        ctx.logger.exception("verify_callsite_preconditions failed")
        return CallsiteVerificationResult(
            callee_function=callee_function,
            summary=f"Internal error: {exc}",
        )


async def verify_callsite_preconditions(
    ctx: ToolContext,
    callee_function: str | list[str],
) -> CallsiteVerificationResult | dict[str, Any]:
    """Verify callee preconditions at call sites via labeled -wp-prop.

    Accepts either a single callee name (str) or a list of callee names.
    When a list is passed, iterates internally per-callee (sequential WP
    calls), collecting results into a dict keyed by callee name. This
    avoids consuming one agent turn per callee.

    The target's preconditions always come from its published spec; the
    sibling proof context (your annotations, callee hypothesis, assume
    set) comes from your working draft — see _verify_single_callee.

    Single callee: returns CallsiteVerificationResult (unchanged).
    Batch (list): returns dict {callee_name: CallsiteVerificationResult_as_dict}.
    """
    # Single callee path — returns the dataclass directly.
    if isinstance(callee_function, str):
        return await _verify_single_callee(ctx, callee_function)

    # Batch path — iterate per callee sequentially, return dict.
    callee_list: list[str] = callee_function
    ctx.logger.info(
        "Batch verify_callsite_preconditions: %d callees %s",
        len(callee_list), callee_list,
    )
    batch_results: dict[str, Any] = {}
    for callee_name in callee_list:
        result = await _verify_single_callee(ctx, callee_name)
        batch_results[callee_name] = asdict(result)

    return batch_results


