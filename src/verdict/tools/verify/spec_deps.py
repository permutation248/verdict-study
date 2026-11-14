"""build_spec_deps: declare per-callee spec dependencies and run the Layer-0
sufficiency check (ablate undeclared callee posts, verify caller targets)."""

from __future__ import annotations

from typing import Any

from verdict.tools.context import ToolContext
from verdict.tools.verify._shared import _resolve_callee_contracts_for_wp


async def build_spec_deps(
    ctx: ToolContext,
    dependencies: dict[str, list[str]],
    annotations: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Declare which callee postconditions your proof depends on.

    Your function's entire proof — postconditions, assigns, loop contracts,
    RTE assertions, and call-site obligations — may depend on guarantees
    provided by your callees. This tool lets you declare, per callee,
    exactly which of its postcondition clauses your proof relies on.

    **When to call:**
    1. After ``verify_function_contract`` succeeds (required — the tool
       rejects calls if your spec is not yet verified).
    2. The callees you declare must already be published.
    3. Call before ``publish`` to establish precise dependencies.

    **What happens if you don't call this tool:**
    All published callees are automatically assigned a conservative ALL
    dependency — any change to any callee's guarantees will wake you for
    full re-verification.

    **What happens if you do call it:**
    The harness verifies your declaration: it keeps only the declared
    postconditions for that callee (plus ALL callees' assigns, which are
    always treated as dependencies), ablates every other callee's
    postconditions to ``ensures \\true``, and re-checks all your proof
    targets in one WP run. If everything still passes, only changes to
    the specific clauses you declared will wake you — precise discharge.

    **assigns are not declared** — they are treated as a universal
    dependency. Only postconditions need explicit declaration.

    Args:
        dependencies: ``{callee_name: [postcondition_clause, ...]}``
            The postcondition clauses your proof depends on.
            - Non-empty list: "my proof relies on exactly these clauses."
            - Empty list ``[]``: "my proof does not depend on this callee's
              postconditions at all."
            Each clause must be an exact string from the callee's published
            postconditions.
        annotations: Your current ACSL annotations. If omitted, read from
            the working draft.

    Returns:
        Per-callee result:
        - ``"verified"``: declaration accepted. You will only be re-woken
          if the declared clauses are removed or modified in a future
          callee version.
        - ``"rejected"``: the declared clauses are not sufficient for your
          proof (WP found a target that fails without the other callees'
          postconditions). Falls back to ALL (any callee change wakes you).
        - ``"error"``: callee not published, verify_function_contract not
          yet run, or declared clauses not in the callee's published spec.
    """
    if ctx.project_context is None:
        return {c: {"success": False, "status": "error", "reason": "no project context", "message": "No project context available."} for c in dependencies}

    from verdict.core.vo_tracker import VOStatus
    spec_vo = ctx.vo_tracker.spec_vo if ctx.vo_tracker else None
    if spec_vo is None or spec_vo.status != VOStatus.VERIFIED:
        return {
            c: {
                "success": False,
                "status": "error",
                "reason": "spec not verified",
                "message": (
                    "verify_function_contract must succeed before declaring "
                    "spec dependencies. Run verify_function_contract first, "
                    "then call this tool."
                ),
            }
            for c in dependencies
        }

    from verdict.tools._spec_dep_check import verify_spec_dep
    from verdict.core.spec_dep import SpecDep

    if annotations is None:
        spec = ctx.registry.read_working_spec(ctx.function_info.name)
        if spec is None:
            return {c: {"status": "error", "reason": "no working draft"} for c in dependencies}
        annotations = {
            "preconditions": list(spec.preconditions),
            "postconditions": list(spec.postconditions),
            "loop_contracts": dict(spec.loop_contracts),
            "assigns": list(spec.assigns),
        }

    # Deliberately NOT the draft's assume context: spec deps measure which
    # PUBLISHED clauses the proof relies on (the dependency judge compares
    # against published changes), so the sufficiency check runs in the real
    # world. Assumed callees carry based_on=0 and are re-verified on
    # discharge regardless of what is measured here.
    resolved_cc, _, _, _ = _resolve_callee_contracts_for_wp(
        agent_cc={},
        callee_names=list(ctx.function_info.callees),
        registry=ctx.registry,
        assume_callee=None,
    )

    rte_target_ids = list(
        ctx.project_context.rte_vos_by_function.get(ctx.function_info.name, [])
    )

    # Pre-validate all declarations
    errors: dict[str, Any] = {}
    valid_deps: dict[str, list[str]] = {}
    for callee, declared_clauses in dependencies.items():
        callee_spec = ctx.registry.read_spec(callee)
        if callee_spec is None:
            errors[callee] = {
                "success": False,
                "status": "error",
                "reason": "callee not published",
                "message": (
                    f"{callee} has not published a spec yet. You can only "
                    f"declare dependencies on published callees. Wait for "
                    f"{callee} to publish, then retry."
                ),
            }
            continue
        missing = [c for c in declared_clauses if c not in callee_spec.postconditions]
        if missing:
            errors[callee] = {
                "success": False,
                "status": "error",
                "reason": f"declared clauses not in published spec: {missing}",
                "message": (
                    f"Clauses {missing} are not in {callee}'s published "
                    f"postconditions. Check the exact clause text against "
                    f"read_function_spec('{callee}')."
                ),
            }
            continue
        valid_deps[callee] = declared_clauses

    if not valid_deps:
        return errors

    # Build test environment:
    #   - declared callees' post kept (possibly partial); undeclared callees'
    #     post ablated to \true inside verify_spec_dep. Assigns always kept.
    #   - every published callee's preconditions are added back, labeled with
    #     'precondition_test', so the caller's call-site VOs become WP targets
    #     in the same sufficiency run. This catches the case where a call-site
    #     proof depends on a sibling callee's frame postcondition: ablating
    #     that sibling's post makes the call-site goal fail, so the sibling
    #     post is correctly pulled into the dependency.
    import copy
    from verdict.tools._cil_builder import label_preconditions
    test_cc = copy.deepcopy(resolved_cc)
    has_callsite_targets = False
    callee_preconditions: dict[str, list[str]] = {}
    for callee in ctx.function_info.callees:
        callee_spec = ctx.registry.read_spec(callee)
        if callee_spec is None:
            continue
        entry = test_cc.setdefault(callee, {})
        if callee in valid_deps and valid_deps[callee]:
            entry["postconditions"] = list(valid_deps[callee])
        entry["assigns"] = list(callee_spec.assigns)
        if callee_spec.preconditions:
            entry["preconditions"] = label_preconditions(callee_spec.preconditions)
            callee_preconditions[callee] = list(callee_spec.preconditions)
            has_callsite_targets = True

    # One joint sufficiency check for all declared callees
    declared_set = set(valid_deps.keys())
    try:
        dep_result = await verify_spec_dep(
            ctx.project_context,
            ctx.function_info.name,
            annotations,
            test_cc,
            declared_set,
            rte_target_ids,
            ctx.agent_id,
            ctx.logger,
            include_callsites=has_callsite_targets,
            callee_preconditions=callee_preconditions,
            timeout=ctx.wp_timeout,
        )
    except Exception as e:
        ctx.logger.warning("build_spec_deps %s failed: %s", ctx.function_info.name, e)
        results = dict(errors)
        for callee in valid_deps:
            results[callee] = {
                "success": False, "status": "error",
                "reason": str(e),
                "message": "Internal error. Fallback to ALL.",
            }
        return results

    results = dict(errors)
    new_deps: dict[str, SpecDep] = {}

    if dep_result.verified:
        for callee, declared_clauses in valid_deps.items():
            clauses = frozenset(declared_clauses) if declared_clauses else None
            dep = SpecDep(depends=bool(declared_clauses), clauses=clauses)
            new_deps[callee] = dep
            results[callee] = {
                "success": True,
                "status": "verified",
                "proved_goals": dep_result.proved,
                "total_goals": dep_result.total,
                "clauses": sorted(declared_clauses),
                "message": (
                    f"Joint dependency verified ({dep_result.proved}/"
                    f"{dep_result.total} goals). Your proof holds with the "
                    f"declared postconditions from {', '.join(sorted(declared_set))}. "
                    f"Proceed to update_my_spec(status='published')."
                ),
            }
        callee_str = "+".join(sorted(declared_set))
        ctx.logger.info(
            "Spec dep %s -> {%s}: VERIFIED (%d/%d goals)",
            ctx.function_info.name, callee_str,
            dep_result.proved, dep_result.total,
        )
    else:
        for callee in valid_deps:
            new_deps[callee] = SpecDep(depends=True, clauses=None)

        unpublished = [
            c for c in ctx.function_info.callees
            if c not in declared_set and ctx.registry.read_spec(c) is None
        ]
        undeclared_published = [
            c for c in ctx.function_info.callees
            if c not in declared_set and ctx.registry.read_spec(c) is not None
        ]
        if unpublished:
            hint = (
                f"Callees not yet published: {', '.join(unpublished)}. "
                f"Publish now and build deps next cycle when all are available. "
            )
        elif undeclared_published:
            hint = (
                f"You may also depend on undeclared callees: "
                f"{', '.join(undeclared_published)}. Try including them. "
            )
        else:
            hint = (
                f"Try declaring more postconditions from your callees. "
            )

        enriched_failures = []
        sf = dep_result.structured_failures
        if sf:
            for f in sf[:8]:
                if f.kind == "spec":
                    enriched_failures.append(f"[spec] {f.label}: {f.clause}")
                elif f.kind == "rte":
                    enriched_failures.append(f"[rte] {f.label}")
                elif f.kind == "callsite":
                    line_str = f" @ line {f.call_site_line}" if f.call_site_line else ""
                    enriched_failures.append(
                        f"[call-site] callee {f.callee} "
                        f"precondition #{f.precondition_index}: "
                        f"{f.clause}{line_str}"
                    )
            if len(sf) > 8:
                enriched_failures.append(f"... (+{len(sf) - 8} more)")
        else:
            for fg in dep_result.failed_goals[:5]:
                enriched_failures.append(fg)
            if len(dep_result.failed_goals) > 5:
                enriched_failures.append(f"... (+{len(dep_result.failed_goals) - 5} more)")
        failed_str = "; ".join(enriched_failures)

        for callee in valid_deps:
            results[callee] = {
                "success": False,
                "status": "rejected",
                "proved_goals": dep_result.proved,
                "total_goals": dep_result.total,
                "failed_goals": enriched_failures,
                "message": (
                    f"Joint declaration not sufficient: "
                    f"{dep_result.proved}/{dep_result.total} goals proved. "
                    f"Failed: [{failed_str}]. {hint}"
                    f"ALL dependency recorded. "
                    f"Proceed to update_my_spec(status='published')."
                ),
            }
        callee_str = "+".join(sorted(declared_set))
        ctx.logger.info(
            "Spec dep %s -> {%s}: REJECTED (fallback to ALL, %d/%d goals)",
            ctx.function_info.name, callee_str,
            dep_result.proved, dep_result.total,
            )

    if new_deps:
        spec = ctx.registry.read_working_spec(ctx.function_info.name)
        if spec is not None:
            spec.spec_deps = {**spec.spec_deps, **new_deps}
            await ctx.registry.write_spec(ctx.agent_id, spec)

    return results


