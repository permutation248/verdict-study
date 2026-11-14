"""VO escape-hatch tool.

`mark_vo_as_bug` records a verification obligation as a known, accepted bug
(BUG VOs do not block GLOBAL_OK). It rejects deferral-disguised-as-bug (a
call-site VO whose callee has not published is a deferral, not a bug).
"""

from __future__ import annotations

from verdict.tools.context import ToolContext


def mark_vo_as_bug(
    ctx: ToolContext,
    vo_id: str,
    reason: str,
) -> dict:
    """Mark a VO as a known bug that cannot be proven.

    Use this as an escape hatch after multiple verification attempts have
    failed and the agent has decided the property is truly unprovable in
    the current code. BUG VOs do not block GLOBAL_OK promotion.

    IMPORTANT: this is NOT a way to defer work. A call-site VO whose
    related callee has not published yet is already allowed to stay
    PENDING — the runtime will re-verify it automatically via a
    `spec_published` wake once the callee publishes. Marking such a VO
    as BUG hides it forever and is a semantic error; the tool rejects
    that case explicitly.
    """
    from verdict.core.vo_tracker import VOStatus

    if ctx.vo_tracker is None:
        raise RuntimeError("No VOTracker bound to this agent context")
    if not reason or not reason.strip():
        raise ValueError("reason is required when marking a VO as bug")

    entry = ctx.vo_tracker._find(vo_id)
    if entry is None:
        raise KeyError(f"Unknown VO: {vo_id}")

    # The spec VO is the function's own contract, not an RTE/UB obligation.
    # Allowing BUG here would let a never-verified contract through the
    # publish gate (which only blocks a PENDING spec VO). The only way out
    # of a PENDING spec VO is a fresh verify_function_contract pass.
    if entry.vo_type == "spec" or vo_id == "spec":
        raise ValueError(
            "The spec contract VO cannot be marked as a bug — it represents "
            "your own published guarantees, and publishing them unverified "
            "is unsound. Re-run verify_function_contract (drop or weaken "
            "the failing clauses) instead."
        )

    # Sanity: can't mark an already-verified VO as bug.
    if entry.status == VOStatus.VERIFIED:
        raise ValueError(
            f"VO {vo_id} is already VERIFIED; marking it as bug would "
            f"lose the passing proof. If you need to reset it, let a "
            f"subsequent verify_* call downgrade it naturally."
        )

    # Reject marking a parent-with-children as bug — agent should mark
    # the specific child VO (call_site_N_I) that is unprovable.
    if entry.vo_type == "call_site" and entry.child_vo_ids:
        raise ValueError(
            f"VO {vo_id} is a parent call-site VO with {len(entry.child_vo_ids)} "
            f"child precondition VOs. Mark the specific child VO as bug instead "
            f"(e.g. {entry.child_vo_ids[0]})."
        )

    # Sanity: a call-site VO whose callee has not published is NOT a bug,
    # it is a deferral. The runtime will wake the agent on spec_published
    # and re-verify at that point. Reject with an instructive message.
    if entry.vo_type == "call_site":
        callee_entry = ctx.registry.get_by_function(entry.related_function)
        callee_unpublished = (
            callee_entry is None or callee_entry.published_spec is None
        )
        if callee_unpublished:
            demand_hint = (
                ""
                if ctx.ablation.demands
                else (
                    " If you need the callee to cover a specific property, "
                    "use submit_proof_demand instead."
                )
            )
            raise ValueError(
                f"Refusing to mark call-site VO {vo_id} as bug: callee "
                f"{entry.related_function!r} has not published yet. "
                f"Leave this VO PENDING — the runtime will wake you with "
                f"a spec_published reason once {entry.related_function} "
                f"publishes, and you can re-run verify_callsite_preconditions "
                f"then.{demand_hint}"
            )

    ctx.vo_tracker.mark_bug(vo_id, reason)
    ctx.logger.warning(
        "marked VO %s as BUG: %s",
        vo_id,
        reason,
    )
    return {
        "vo_id": vo_id,
        "status": "bug",
        "reason": reason,
    }


