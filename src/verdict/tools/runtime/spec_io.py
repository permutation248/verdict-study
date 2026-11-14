"""Spec read/publish tools.

An agent reads its own working spec (`read_my_spec`) or a callee's published
spec (`read_function_spec`), and publishes its working draft via
`update_my_spec` — which runs the publish VO gate and records the publish-time
dependency snapshots. proof_state is NOT touched here; promotion is the
orchestrator's job (Orchestrator._recompute_proof_state).
"""

from __future__ import annotations

from verdict.core.types import FunctionSpec, SpecStatus
from verdict.core.vo_tracker import VOStatus
from verdict.tools.context import ToolContext


class PublishBlocked(Exception):
    """Raised when update_my_spec refuses to publish due to unverified VOs."""


def _annotations_differ(prev: FunctionSpec | None, new: FunctionSpec) -> bool:
    """Whether a manual draft changes the contract vs the existing draft.

    With no existing draft, any non-empty annotation content counts as a
    change — there is no verified contract it could be the image of.
    """
    if prev is None:
        return bool(
            new.preconditions or new.postconditions
            or new.loop_contracts or new.assigns
        )
    return (
        list(prev.preconditions) != list(new.preconditions)
        or list(prev.postconditions) != list(new.postconditions)
        or dict(prev.loop_contracts) != dict(new.loop_contracts)
        or list(prev.assigns) != list(new.assigns)
    )


def _set_default_spec_deps_on_publish(ctx: ToolContext, spec: FunctionSpec) -> None:
    """Fill in spec deps for published callees at publish time.

    For every published callee without an entry in spec.spec_deps,
    look for a finer dep in the working spec (set by build_spec_deps)
    or the previously published spec. Falls back to conservative ALL.
    """
    from verdict.core.spec_dep import SpecDep

    working = ctx.registry.read_working_spec(ctx.function_info.name)
    working_deps = working.spec_deps if working else {}

    existing = ctx.registry.get(ctx.agent_id)
    prev_deps = {}
    if existing and existing.published_spec:
        prev_deps = existing.published_spec.spec_deps or {}

    for callee in ctx.function_info.callees:
        if callee in spec.spec_deps:
            continue
        if ctx.registry.read_spec(callee) is None:
            continue
        for source in (working_deps, prev_deps):
            dep = source.get(callee)
            if dep is not None:
                spec.spec_deps[callee] = dep
                break
        else:
            spec.spec_deps[callee] = SpecDep(depends=True, clauses=None)


def _record_callsite_pre_deps_on_publish(ctx: ToolContext, spec: FunctionSpec) -> None:
    """Record the callee preconditions each call-site proof was verified
    against (reverse / precondition-direction dependency).

    For every callee whose published spec has preconditions, if the caller's
    call-site VOs targeting that callee are all VERIFIED/BUG, snapshot the
    callee's current published preconditions. The dependency judge later
    discharges the call site as long as those preconditions are unchanged.
    """
    tracker = ctx.vo_tracker
    if tracker is None:
        return
    from collections import defaultdict
    from verdict.core.vo_tracker import VOStatus

    by_callee: dict[str, list] = defaultdict(list)
    for vo in tracker.leaf_call_site_vos().values():
        by_callee[vo.related_function].append(vo)

    for callee in ctx.function_info.callees:
        callee_spec = ctx.registry.read_spec(callee)
        if callee_spec is None or not callee_spec.preconditions:
            continue
        vos = by_callee.get(callee, [])
        # VERIFIED only: a BUG call site was never actually proven, so
        # recording a pre-dep for it would tell the judge the call site is
        # discharged and suppress future precondition-direction re-wakes.
        if vos and all(v.status == VOStatus.VERIFIED for v in vos):
            spec.callsite_pre_deps[callee] = frozenset(callee_spec.preconditions)


def read_my_spec(ctx: ToolContext) -> dict:
    """Return this agent's own working spec, including callee_contracts.

    Useful after a wake-up: the agent can see its prior optimistic
    hypothesis (saved by the previous verify_function_contract call) and
    decide whether to keep it or update.
    """
    spec = ctx.registry.read_working_spec(ctx.function_info.name)
    if spec is None:
        return {
            "found": False,
            "function_name": ctx.function_info.name,
            "spec": None,
            "message": "no working spec yet — call verify_function_contract first",
        }
    return {
        "found": True,
        "function_name": spec.function_name,
        "spec": {
            "function_name": spec.function_name,
            "preconditions": list(spec.preconditions),
            "postconditions": list(spec.postconditions),
            "loop_contracts": dict(spec.loop_contracts),
            "assigns": list(spec.assigns),
            "callee_contracts": {
                name: dict(cc) for name, cc in spec.callee_contracts.items()
            },
            "assumed_callees": list(spec.assumed_callees),
            "version": spec.version,
            "status": spec.status,
            "based_on_versions": dict(spec.based_on_versions),
        },
    }


def read_function_spec(ctx: ToolContext, function_name: str) -> dict:
    """Read another function's latest published spec.

    Returns {found, function_name, spec | None}. `found=False` means the
    callee exists in the registry but has not published yet — distinct from
    a tool error.
    """
    spec = ctx.registry.read_spec(function_name)
    if spec is None:
        project_ctx = ctx.project_context
        if (
            project_ctx is not None
            and function_name in project_ctx.extern_functions
        ):
            message = (
                f"{function_name} is an extern environment function "
                f"(prototype only): it has no agent and will never "
                f"publish a spec. Do not wait on it. If your proof needs "
                f"guarantees about it, state them as a "
                f"callee_contracts[{function_name!r}] entry "
                f"(postconditions/assigns) — recorded as a standing "
                f"environment axiom."
            )
        else:
            message = (
                f"{function_name} has no published spec yet. Either it "
                f"is not registered, or its owner agent has not "
                f"published. Wait for a spec_published wake reason."
            )
        return {
            "found": False,
            "function_name": function_name,
            "spec": None,
            "message": message,
        }
    return {
        "found": True,
        "function_name": function_name,
        "spec": {
            "function_name": spec.function_name,
            "preconditions": list(spec.preconditions),
            "postconditions": list(spec.postconditions),
            "loop_contracts": dict(spec.loop_contracts),
            "assigns": list(spec.assigns),
            "version": spec.version,
            "status": spec.status,
            "based_on_versions": dict(spec.based_on_versions),
        },
    }


def _check_publish_vo_gate(ctx: ToolContext) -> None:
    """Raise PublishBlocked if the agent has unhandled VOs before publishing.

    An "unhandled" VO is one whose status is either PENDING (never verified)
    or FAILED (WP rejected it) — in both cases the agent must take further
    action before turning the spec into an authoritative version. BUG is an
    explicit agent acknowledgement that something can't be proven and is
    allowed through.

    Call-site VOs whose related callee has not published yet are an
    intentional exception for PENDING only: the loop will wake the agent
    again via spec_published once the callee publishes.
    """
    tracker = ctx.vo_tracker
    if tracker is None:
        return  # no VO tracking → nothing to gate

    blocked: list[str] = []

    for vo_id, vo in tracker.rte_vos.items():
        if vo.status == VOStatus.PENDING:
            blocked.append(f"RTE {vo_id} (pending)")
        elif vo.status == VOStatus.FAILED:
            blocked.append(
                f"RTE {vo_id} (failed — verify, weaken, or mark_vo_as_bug)"
            )

    # Check leaf call-site VOs only (children + childless parents).
    # Parents with children are containers; their status is derived.
    for vo_id, vo in tracker.leaf_call_site_vos().items():
        callee_entry = ctx.registry.get_by_function(vo.related_function)
        callee_published = (
            callee_entry is not None and callee_entry.published_spec is not None
        )
        if vo.status == VOStatus.PENDING:
            if callee_published:
                blocked.append(
                    f"call-site {vo_id} -> {vo.related_function} "
                    f"(pending, callee already published)"
                )
            # else: deferred until the callee publishes, allowed
        elif vo.status == VOStatus.FAILED:
            blocked.append(
                f"call-site {vo_id} -> {vo.related_function} "
                f"(failed — weaken your precondition or mark_vo_as_bug)"
            )

    # Spec contract VO: PENDING means a callee published changed guarantees
    # and this agent's contract was invalidated but not yet re-verified.
    # Publishing it would propagate an un-rechecked spec to callers.
    # (None = no contract VO yet, e.g. a pure-RTE function — allowed.)
    if tracker.spec_vo is not None and tracker.spec_vo.status == VOStatus.PENDING:
        blocked.append(
            "spec contract (stale — re-run verify_function_contract against "
            "the published callee specs before re-publishing)"
        )

    if blocked:
        raise PublishBlocked(
            "cannot publish while VOs are unhandled: "
            + "; ".join(blocked)
            + ". Run verify_rte_assertions / verify_callsite_preconditions, "
            + "or mark_vo_as_bug if truly unprovable."
        )


async def update_my_spec(ctx: ToolContext, spec: FunctionSpec) -> dict:
    """Write this agent's own working spec and publish if requested.

    Returns a confirmation dict: {success, function_name, status, version,
    callee_contracts_count, vo_summary, removed_postconditions, message}.
    The agent reads `success` to know the write went through; `version`
    to see what version was assigned; `vo_summary` to see if any VOs are
    still outstanding; `removed_postconditions` to spot accidental drops
    of previously-published guarantees that callers may depend on.
    """
    wants_publish = spec.status == SpecStatus.PUBLISHED
    if wants_publish:
        _check_publish_vo_gate(ctx)
        # Soundness invariant: publish the VERIFIED draft, not whatever the
        # agent passes in this call. verify_function_contract auto-saves
        # the exact annotations that WP checked; publishing those — and
        # only those — guarantees that published == verified.
        draft = ctx.registry.read_working_spec(ctx.function_info.name)
        if draft is None:
            return {
                "success": False,
                "function_name": ctx.function_info.name,
                "message": (
                    "No working draft found. Run verify_function_contract "
                    "first — it saves the verified annotations as a draft."
                ),
            }
        draft.status = SpecStatus.PUBLISHED
        spec = draft

    if not wants_publish:
        # The registry write path feeds later CIL builds (rte/callsite/
        # weakening lift the draft into annotations), so malformed shapes
        # must be rejected here too — not only at the verify-tool entries.
        from verdict.tools._mechanical import validate_annotations_shape
        shape_errors = validate_annotations_shape({
            "preconditions": spec.preconditions,
            "postconditions": spec.postconditions,
            "loop_contracts": spec.loop_contracts,
            "assigns": spec.assigns,
        })
        if shape_errors:
            return {
                "success": False,
                "function_name": ctx.function_info.name,
                "message": (
                    "Malformed draft annotations — fix the JSON shape and "
                    "retry: " + "; ".join(shape_errors)
                ),
            }
        # Drafts are working copies for ANNOTATIONS — accepted as-is. The
        # proof-context bookkeeping (callee_contracts hypothesis and
        # assumed_callees) is owned by verify_function_contract, so it
        # carries over from the existing draft: a manual draft write must
        # not be able to launder the assume record away (publishing without
        # the based_on=0 debt anchor) or drop the stored hypothesis.
        prev = ctx.registry.read_working_spec(ctx.function_info.name)
        if prev is not None:
            spec.callee_contracts = {
                name: dict(cc) for name, cc in prev.callee_contracts.items()
            }
            spec.assumed_callees = list(prev.assumed_callees)
            # A changed proof context invalidates RTE/call-site proofs
            # established against the previous draft — same rule as
            # verify_function_contract's draft save.
            from verdict.tools.verify._shared import (
                _reset_vos_on_context_change,
            )
            vos_reset = _reset_vos_on_context_change(
                ctx.vo_tracker, prev, spec
            )
            if vos_reset:
                ctx.logger.info(
                    "update_my_spec(draft): proof context changed — reset "
                    "%d RTE/call-site VO(s) to PENDING",
                    vos_reset,
                )
        # A manual draft is unverified by definition: if it changes the
        # contract, the spec VO goes PENDING so the publish gate forces a
        # verify_function_contract pass — publish must only ever ship a
        # verified draft.
        if ctx.vo_tracker is not None and _annotations_differ(prev, spec):
            ctx.vo_tracker.invalidate_spec(
                "draft annotations changed via update_my_spec without "
                "verification — re-run verify_function_contract before "
                "publishing"
            )
            ctx.logger.info(
                "update_my_spec(draft): annotations changed — spec VO "
                "invalidated until re-verified"
            )

    # Soft check: detect postconditions present in the previously-published
    # spec but missing from this submission. Don't block — surface the diff
    # so the agent can self-correct or use apply_precondition_weakening.
    old_published_posts: set[str] = set()
    existing = ctx.registry.get(ctx.agent_id)
    if existing is not None and existing.published_spec is not None:
        old_published_posts = set(existing.published_spec.postconditions)
    new_posts = set(spec.postconditions)
    removed_postconditions = sorted(old_published_posts - new_posts)
    if removed_postconditions:
        ctx.logger.warning(
            "update_my_spec: dropping %d previously-published postcondition(s) "
            "from spec %s: %s",
            len(removed_postconditions),
            spec.function_name,
            removed_postconditions,
        )

    if wants_publish:
        _set_default_spec_deps_on_publish(ctx, spec)
        _record_callsite_pre_deps_on_publish(ctx, spec)

    written = await ctx.registry.write_spec(ctx.agent_id, spec)
    # Note: proof_state promotion (DRAFT → LOCAL_OK → GLOBAL_OK) is NOT
    # done here. It is derived centrally by the orchestrator from
    # (published_spec, vo_tracker, deps) at cycle end — see
    # Orchestrator._recompute_proof_state.

    # SpecTracker is normally wired to the run store's event stream and
    # regenerates `current_specs.c` on spec_published events. Only call
    # record_update directly when no run_store is hooked up (unit tests).
    if ctx.spec_tracker and ctx.registry._run_store is None:
        ctx.spec_tracker.record_update(ctx.agent_id, written)
    ctx.logger.info(
        "Spec %s v%d (%s)",
        written.function_name,
        written.version,
        written.status,
    )

    vo_summary: dict | None = None
    if ctx.vo_tracker is not None:
        vo_summary = {
            "rte_pending": sum(
                1 for v in ctx.vo_tracker.rte_vos.values()
                if v.status == VOStatus.PENDING
            ),
            "rte_failed": sum(
                1 for v in ctx.vo_tracker.rte_vos.values()
                if v.status == VOStatus.FAILED
            ),
            "rte_verified": sum(
                1 for v in ctx.vo_tracker.rte_vos.values()
                if v.status == VOStatus.VERIFIED
            ),
            "rte_bug": sum(
                1 for v in ctx.vo_tracker.rte_vos.values()
                if v.status == VOStatus.BUG
            ),
            "callsite_pending": sum(
                1 for v in ctx.vo_tracker.leaf_call_site_vos().values()
                if v.status == VOStatus.PENDING
            ),
            "callsite_failed": sum(
                1 for v in ctx.vo_tracker.leaf_call_site_vos().values()
                if v.status == VOStatus.FAILED
            ),
            "callsite_verified": sum(
                1 for v in ctx.vo_tracker.leaf_call_site_vos().values()
                if v.status == VOStatus.VERIFIED
            ),
            "callsite_bug": sum(
                1 for v in ctx.vo_tracker.leaf_call_site_vos().values()
                if v.status == VOStatus.BUG
            ),
        }

    base_message = (
        f"Spec {written.function_name} v{written.version} written "
        f"as {written.status}."
    )
    if removed_postconditions:
        base_message += (
            f" WARNING: dropped {len(removed_postconditions)} "
            f"previously-published postcondition(s) that callers may depend "
            f"on. If this was intentional, use apply_precondition_weakening "
            f"so the cascade is computed; otherwise re-add them."
        )

    return {
        "success": True,
        "function_name": written.function_name,
        "status": written.status,
        "version": written.version,
        "callee_contracts_count": len(written.callee_contracts),
        "vo_summary": vo_summary,
        "removed_postconditions": removed_postconditions,
        "message": base_message,
    }
