"""verify_function_contract: worklist verification of a function's contract."""

from __future__ import annotations

from dataclasses import asdict
from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.result_types import VerificationResult
from verdict.tools.verify._shared import (
    _reset_vos_on_context_change,
    _resolve_callee_contracts_for_wp,
    _validate_callee_contracts,
)

# Appended to every failure feedback. WP failures that survive repeated
# goal rewordings are usually missing MEMORY facts, not logic — agents
# tend to iterate the failing clause's wording and never look at the
# hypothesis side (observed: 27 reformulation attempts on one joint bound
# whose real blocker was an absent \separated between two out-params).
_MEMORY_MODEL_HINT = (
    " Hint for persistent failures: WP needs the memory model pinned "
    "down, not just the logic — which regions are separate and which "
    "locations may change. If a clause you believe logically true keeps "
    "failing across rewordings, check the hypothesis side instead of the "
    "goal's wording: your preconditions and loop contracts may be missing "
    "a \\separated or assigns/loop-assigns constraint the proof silently "
    "relies on — and apply the same check to every callee contract you "
    "depend on (a callee without precise assigns/separation weakens all "
    "reasoning after the call)."
)


async def verify_function_contract(
    ctx: ToolContext,
    annotations: dict[str, Any],
    callee_contracts: dict[str, dict[str, Any]] | None = None,
    assume_callee: list[str] | None = None,
) -> VerificationResult:
    """Verify function contract via worklist-based Frama-C WP.

    Uses the worklist algorithm: apply all specs → test each with
    ``-wp-prop contract_id`` → remove failed → repeat until convergence.
    Three phases in order: loop contracts → postconditions → assigns.

    `callee_contracts` carries the agent's hypothesis (postconditions
    and assigns only) for each callee. By default these are merged with
    callee.published_spec (published wins, soundness). Pass
    `assume_callee=[name1, name2]` to OPT OUT of the override for those
    callees — useful when you have an open proof_demand and want to
    verify under the assumption that the demand will be satisfied,
    without dropping your own postconditions in the meantime.

    The agent's RAW `callee_contracts` (NOT the merged version) is
    persisted into your working draft on success, so subsequent
    registry-driven tools can decide for themselves whether to merge.

    Falls back to mock when no project_context (for tests).
    """
    callee_contracts = callee_contracts or {}
    _validate_callee_contracts(
        callee_contracts, demands_available=not ctx.ablation.demands
    )

    if ctx.ablation.assume and assume_callee:
        demand_hint = (
            " or use submit_proof_demand to request the guarantee"
            if not ctx.ablation.demands
            else ""
        )
        return VerificationResult(
            success=False,
            feedback=(
                f"assume_callee is disabled in this run (--ablate assume); "
                f"got {sorted(assume_callee)}. Verify against published "
                f"callee contracts only — for unpublished callees, wait "
                f"for their publish{demand_hint}."
            ),
        )

    from verdict.tools._mechanical import validate_annotations_shape
    shape_errors = validate_annotations_shape(annotations)
    if shape_errors:
        return VerificationResult(
            success=False,
            feedback=(
                "Malformed annotations — fix the JSON shape and retry: "
                + "; ".join(shape_errors)
            ),
        )

    resolved_cc, overridden, hypothesized, dropped = _resolve_callee_contracts_for_wp(
        agent_cc=callee_contracts,
        callee_names=list(ctx.function_info.callees),
        registry=ctx.registry,
        assume_callee=assume_callee,
        allow_hypotheses=not ctx.ablation.assume,
        extern_callees=frozenset(ctx.function_info.extern_callees),
    )
    # A hypothesis used for an UNPUBLISHED callee is an assumption with the
    # same standing as an explicit assume_callee: record it, so the publish
    # path anchors based_on=0 and the debt machinery forces a re-verify
    # against the real spec once the callee publishes. Without this, those
    # proofs live in a hypothesis world that nothing ever re-checks.
    # Extern callees are the permanent case of the same thing: no publisher
    # exists, so the assumption is a standing environment axiom — recorded
    # the same way, auto-settled at publish, surfaced in the final report.
    effective_assumes = sorted(set(assume_callee or []) | set(hypothesized))
    extern_axioms = sorted(
        set(effective_assumes) & set(ctx.function_info.extern_callees)
    )

    ctx.logger.debug(
        "verify_function_contract %s annotations=%s agent_cc=%d resolved=%d "
        "(overridden=%s, assumed=%s)",
        ctx.function_info.name,
        list(annotations.keys()),
        len(callee_contracts),
        len(resolved_cc),
        overridden or "-",
        assume_callee or "-",
    )

    if ctx.project_context is None:
        return VerificationResult(
            success=True,
            feedback="Mock: all goals proven.",
        )

    from verdict.tools._mechanical import run_semantic_filter_all
    from verdict.tools._worklist import worklist_verify_specs

    trace = ctx.debugger.begin("verify_function_contract") if ctx.debugger else None

    try:
        if trace:
            trace.save_input({"annotations": annotations, "callee_contracts": callee_contracts})

        # 1. Semantic pre-filter (automatic, agent unaware)
        node = ctx.project_context.call_graph_dict.get(
            ctx.function_info.name
        )
        if node:
            from verdict.tools._mechanical import extract_function_params
            params = extract_function_params(getattr(node, "source_code", ""))
            valid, errors = run_semantic_filter_all(annotations, params)
            if not valid:
                result = VerificationResult(
                    success=False,
                    feedback=f"Semantic filter rejected: {'; '.join(errors)}",
                    filtered_annotations=errors,
                )
                if trace:
                    trace.save_result(asdict(result))
                return result

        # 2. ACSL syntax check — catch annotation parse errors before WP.
        # Frama-C with bad annotations returns error code 1 and 0 goals,
        # which the worklist reports as "FAILED [0/0]" with no actionable
        # feedback. Checking syntax first gives the agent a precise error
        # message so it can fix the annotation instead of blindly retrying.
        from verdict.tools._mechanical import run_syntax_check
        from verdict.tools._cil_builder import build_annotated_cil_file

        syntax_cil = build_annotated_cil_file(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            resolved_cc,
            ctx.agent_id,
        )
        syntax_ok, syntax_errors = await run_syntax_check(syntax_cil)
        if not syntax_ok:
            error_text = "; ".join(syntax_errors[:5])
            result = VerificationResult(
                success=False,
                feedback=(
                    f"ACSL syntax error — fix before re-verifying. "
                    f"Errors: {error_text}"
                ),
            )
            if trace:
                trace.save_result(asdict(result))
            return result

        # 3. Worklist verification (replaces single-shot WP) — uses the
        # MERGED `resolved_cc` so callee published specs override the
        # agent's hypothesis (unless the agent opted out via assume_callee).
        wl = await worklist_verify_specs(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            resolved_cc,
            ctx.agent_id,
            timeout=ctx.wp_timeout,
            logger=ctx.logger,
        )

        # 3. Build result from worklist
        total_verified = (
            sum(len(v) for v in wl.verified_loop_contracts.values())
            + len(wl.verified_postconditions)
            + len(wl.verified_assigns)
        )
        total_failed = (
            sum(len(v) for v in wl.failed_loop_contracts.values())
            + len(wl.failed_postconditions)
            + len(wl.failed_assigns)
        )
        total = total_verified + total_failed
        success = total > 0 and total_failed == 0

        # Auto-persist verified annotations + caller's RAW callee_contracts
        # hypothesis (NOT the merged resolved_cc) into the working draft.
        # Storing the raw hypothesis lets later tools see what the agent
        # asserted; they inherit the assume set from this draft too — one
        # proof context per draft.
        draft_saved = False
        draft_version: int | None = None
        vos_reset = 0
        if success:
            from verdict.core.types import FunctionSpec, SpecStatus
            prev_draft = ctx.registry.read_working_spec(ctx.function_info.name)
            draft_spec = FunctionSpec(
                function_name=ctx.function_info.name,
                preconditions=list(annotations.get("preconditions", [])),
                postconditions=list(annotations.get("postconditions", [])),
                loop_contracts=dict(annotations.get("loop_contracts", {})),
                assigns=list(annotations.get("assigns", [])),
                callee_contracts={
                    name: dict(cc) for name, cc in callee_contracts.items()
                },
                status=SpecStatus.DRAFT,
                assumed_callees=effective_assumes,
            )
            written = await ctx.registry.write_spec(ctx.agent_id, draft_spec)
            draft_saved = True
            draft_version = written.version
            ctx.logger.info(
                "verify_function_contract persisted draft v%d (raw_cc=%d, "
                "wp_used resolved_cc=%d, overridden=%s, assumed=%s)",
                written.version,
                len(callee_contracts),
                len(resolved_cc),
                overridden or "-",
                effective_assumes or "-",
            )
            if ctx.vo_tracker is not None:
                ctx.vo_tracker.mark_spec_verified()
                # RTE/call-site VOs verified against the PREVIOUS draft were
                # proven in that context; a changed context (preconditions,
                # loop invariants, callee hypothesis, assume set) requires
                # re-establishing them.
                vos_reset = _reset_vos_on_context_change(
                    ctx.vo_tracker, prev_draft, draft_spec
                )
                if vos_reset:
                    ctx.logger.info(
                        "Proof context changed vs previous draft — reset "
                        "%d RTE/call-site VO(s) to PENDING",
                        vos_reset,
                    )

        # State the contract-resolution fact at the moment it applies, so the
        # agent knows which callee contracts the WP context actually used.
        # On success the published specs were sufficient — a plain fact. On
        # failure the distinction becomes decision-relevant, so name the
        # mechanism for verifying under one's own hypothesis.
        if success:
            override_note = (
                f" (published specs used for {overridden}; the "
                f"callee_contracts you passed for them were ignored)"
                if overridden
                else ""
            )
            assume_note = (
                f" (assumed_callees={effective_assumes})"
                if effective_assumes
                else ""
            )
            if extern_axioms:
                assume_note += (
                    f" Note: {extern_axioms} are extern environment "
                    f"functions — your callee_contracts for them are "
                    f"standing axioms (nothing will ever publish for them; "
                    f"do not wait on or demand them) and will be reported "
                    f"alongside your result."
                )
            reset_note = (
                f" Proof context changed vs your previous draft — "
                f"{vos_reset} previously-checked RTE/call-site VO(s) were "
                f"reset to PENDING; re-run verify_rte_assertions / "
                f"verify_callsite_preconditions before publishing."
                if vos_reset
                else ""
            )
            dropped_note = (
                f" Note: callee_contracts for unpublished callees {dropped} "
                f"were NOT used (assume ablation) — the proof ran against "
                f"the weak environment."
                if dropped
                else ""
            )
            feedback = (
                f"All {total_verified} contracts verified{assume_note}. "
                f"Draft saved (v{draft_version}, raw_cc={len(callee_contracts)})."
                f"{override_note}{reset_note}{dropped_note}"
            )
        elif total == 0:
            feedback = "No contracts to verify."
        else:
            if ctx.ablation.assume:
                remedy = (
                    "wait for the callee to publish a stronger guarantee"
                    if ctx.ablation.demands
                    else "submit_proof_demand requests a stronger guarantee"
                )
                override_note = (
                    f" (published specs were used for {overridden}, not your "
                    f"callee_contracts — {remedy})"
                    if overridden
                    else ""
                )
            else:
                remedy = (
                    "assume_callee verifies under your hypothesis meanwhile"
                    if ctx.ablation.demands
                    else "assume_callee verifies under your hypothesis "
                    "while submit_proof_demand requests it"
                )
                override_note = (
                    f" (published specs were used for {overridden}, not your "
                    f"callee_contracts — if a failure needs a stronger guarantee "
                    f"than the published spec provides, {remedy})"
                    if overridden
                    else ""
                )
            dropped_note = (
                f" Note: callee_contracts for unpublished callees {dropped} "
                f"were NOT used (assume ablation) — the proof ran against "
                f"the weak environment; wait for their publish."
                if dropped
                else ""
            )
            parts = []
            if wl.failed_loop_contracts:
                n = sum(len(v) for v in wl.failed_loop_contracts.values())
                parts.append(f"loop: {n} failed")
            if wl.failed_postconditions:
                parts.append(f"post: {len(wl.failed_postconditions)} failed")
            if wl.failed_assigns:
                parts.append(f"assigns: {len(wl.failed_assigns)} failed")
            feedback = (
                f"{total_verified}/{total} verified. "
                + "; ".join(parts)
                + override_note
                + dropped_note
                + _MEMORY_MODEL_HINT
            )

        result = VerificationResult(
            success=success,
            feedback=feedback,
            verified_loop_contracts=wl.verified_loop_contracts,
            failed_loop_contracts=wl.failed_loop_contracts,
            verified_postconditions=wl.verified_postconditions,
            failed_postconditions=wl.failed_postconditions,
            verified_assigns=wl.verified_assigns,
            failed_assigns=wl.failed_assigns,
            worklist_log=wl.worklist_log,
            draft_saved=draft_saved,
            draft_version=draft_version,
            draft_callee_contracts_count=len(callee_contracts),
        )
        if trace:
            trace.save_worklist_log(wl.worklist_log)
            trace.save_result(asdict(result))
        return result

    except Exception as exc:
        ctx.logger.exception("verify_function_contract failed")
        return VerificationResult(
            success=False,
            feedback=f"Internal error: {exc}",
            error=str(exc),
        )


# ---------------------------------------------------------------------------
# Tool 2: check_acsl_syntax
# ---------------------------------------------------------------------------


