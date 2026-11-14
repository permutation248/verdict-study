"""Helpers shared across the verify tools: callee-contract resolution for WP,
callee-contract validation, the draft proof context, and call-site VO
child-update mapping."""

from __future__ import annotations

from typing import Any

from verdict.tools.result_types import PreconditionResult, VOUpdate


_ALLOWED_CALLEE_HYPOTHESIS_KEYS = {"postconditions", "assigns"}
_REJECTED_CALLEE_HYPOTHESIS_KEYS = {"preconditions", "loop_contracts"}


def _draft_assumes(spec: Any) -> list[str]:
    """The assume set inherited from the working draft.

    The draft is the single proof context: which callees are verified under
    the agent's own hypothesis is decided once, by verify_function_contract's
    `assume_callee` argument, and stored on the draft. Every other verify
    tool reads it from here so all proofs of one draft share one consistent
    world.

    Filters to callees that actually have a hypothesis entry: contract
    verification guarantees the pair, but a draft written through another
    path degrades to the sound published-spec resolution instead of
    erroring inside the resolver.
    """
    if spec is None:
        return []
    return [n for n in spec.assumed_callees if n in spec.callee_contracts]


def _proof_context(spec: Any) -> tuple | None:
    """The draft fields RTE/call-site proofs use as hypotheses.

    Own postconditions and assigns are proof GOALS, not hypotheses —
    changing them cannot invalidate an RTE/call-site proof, so they are
    excluded (the common strengthen-postconditions iteration causes no
    resets).
    """
    if spec is None:
        return None
    return (
        list(spec.preconditions),
        {k: list(v) for k, v in spec.loop_contracts.items()},
        {k: dict(v) for k, v in spec.callee_contracts.items()},
        sorted(spec.assumed_callees),
    )


def _reset_vos_on_context_change(
    tracker: Any,
    prev_spec: Any,
    new_spec: Any,
) -> int:
    """Reset non-PENDING RTE/call-site VOs when the proof context changed.

    A VERIFIED VO is a proof in the context of the draft current at the
    time it ran (own preconditions, loop invariants, callee hypothesis,
    assume set). When a new draft changes that context, those proofs are
    re-established from scratch — the own-draft counterpart of the
    reconcile layer's invalidation on callee publishes. Returns the number
    of VOs reset.
    """
    if tracker is None:
        return 0
    if _proof_context(prev_spec) == _proof_context(new_spec):
        return 0
    from verdict.core.vo_tracker import VOStatus

    reset = 0
    for vo_id, vo in list(tracker.rte_vos.items()):
        if vo.status != VOStatus.PENDING:
            tracker.mark_pending(vo_id)
            reset += 1
    for vo_id, vo in list(tracker.call_site_vos.items()):
        if vo.status != VOStatus.PENDING:
            tracker.mark_pending(vo_id)
            reset += 1
    return reset


def _resolve_callee_contracts_for_wp(
    agent_cc: dict[str, dict[str, Any]],
    callee_names: list[str],
    registry: Any,
    assume_callee: list[str] | None,
    *,
    allow_hypotheses: bool = True,
    extern_callees: frozenset[str] | set[str] = frozenset(),
) -> tuple[dict[str, dict[str, Any]], list[str], list[str], list[str]]:
    """Merge agent's hypothesis with callee's published spec for WP.

    Resolution rules per callee:
      • callee in `assume_callee` → use agent's `agent_cc[callee]` even if
        callee is published. This is the explicit "verify under my open
        hypothesis" path. Requires `agent_cc[callee]` to exist.
      • callee published (and not assumed) → use callee.published_spec
        (canonical, sound).
      • callee NOT published + agent provided cc → use agent's cc. This is
        an ASSUMPTION too — it must be recorded as one (see
        ``hypothesized``), or the proof silently lives in a hypothesis
        world with no debt anchor once the callee publishes.
      • callee NOT published + nothing provided → omit (stub gets no
        annotations; weak environment).

    Returns ``(resolved_cc, overridden, hypothesized, dropped)``:
    ``overridden`` lists callees whose agent-supplied cc was replaced by
    the published spec; ``hypothesized`` lists UNPUBLISHED callees whose
    agent cc was used — callers persisting a draft must union these into
    ``assumed_callees`` so the based_on=0 anchor exists when the callee
    eventually publishes; ``dropped`` lists unpublished callees whose cc
    was IGNORED because hypotheses are disabled (assume ablation) —
    always empty in normal operation.

    Extern callees (prototype-only environment functions) take the same
    unpublished path and land in ``hypothesized`` — but for them no
    anchor is created (they are not dependencies) and the assumption is
    settled at publish time as a standing environment axiom.

    ``allow_hypotheses=False`` (the ``--ablate assume`` arm) restricts
    resolution to published specs: cc entries for unpublished AGENT
    callees are dropped (weak environment). Extern callees in
    ``extern_callees`` are exempt — environment axioms are not peer
    optimism (no publisher exists for them).
    """
    assume_set = set(assume_callee or [])
    if not allow_hypotheses:
        non_extern_assumes = assume_set - set(extern_callees)
        if non_extern_assumes:
            raise ValueError(
                f"assume_callee is disabled in this run (--ablate assume); "
                f"got {sorted(non_extern_assumes)}. Verify against published "
                f"callee contracts only."
            )
    resolved: dict[str, dict[str, Any]] = {}
    overridden: list[str] = []
    hypothesized: list[str] = []
    dropped: list[str] = []

    for name in callee_names:
        if name in assume_set:
            if name not in agent_cc:
                raise ValueError(
                    f"assume_callee includes {name!r} but no callee_contracts "
                    f"entry was provided for it. Either remove {name!r} from "
                    f"assume_callee, or add a callee_contracts[{name!r}] "
                    f"entry stating your hypothesis."
                )
            resolved[name] = dict(agent_cc[name])
            continue

        callee_spec = registry.read_spec(name)
        if callee_spec is not None:
            published_cc = {
                "postconditions": list(callee_spec.postconditions),
                "assigns": list(callee_spec.assigns),
            }
            if name in agent_cc and agent_cc[name] != published_cc:
                overridden.append(name)
            resolved[name] = published_cc
        elif name in agent_cc:
            if allow_hypotheses or name in extern_callees:
                resolved[name] = dict(agent_cc[name])
                hypothesized.append(name)
            else:
                dropped.append(name)

    return resolved, overridden, hypothesized, dropped


def _validate_callee_contracts(
    callee_contracts: dict[str, dict[str, Any]],
    *,
    demands_available: bool = True,
) -> None:
    """Reject callee-contract entries that would let the caller dictate
    callee preconditions or internal loop contracts.

    A caller is only allowed to optimistically assume what a callee
    **guarantees** to it (postconditions, assigns). Hypothesizing a
    callee's `requires` is directionally wrong — it would add a new
    call-site obligation to the caller without giving it any useful
    guarantee in return. Loop contracts live in the callee body and
    are not visible through the stub we build for WP.

    ``demands_available=False`` (the ``--ablate demands`` arm) swaps the
    remedy sentence so the error never points at a removed tool.
    """
    remedy = (
        "If you need the callee to weaken a precondition or provide a "
        "specific postcondition, use `submit_proof_demand` with "
        "property_kind='postcondition' or 'assigns'."
        if demands_available
        else "You cannot request changes from the callee in this run — "
        "work with what it publishes."
    )
    for callee_name, cc in callee_contracts.items():
        if not isinstance(cc, dict):
            raise ValueError(
                f"callee_contracts[{callee_name!r}] must be an object "
                f"{{postconditions: [...], assigns: [...]}}, got "
                f"{type(cc).__name__}"
            )
        for bad_key in _REJECTED_CALLEE_HYPOTHESIS_KEYS:
            value = cc.get(bad_key)
            if value:
                raise ValueError(
                    f"callee_contracts[{callee_name!r}] cannot contain "
                    f"{bad_key!r}. A caller may only hypothesize a callee's "
                    f"postconditions and assigns — these are the guarantees "
                    f"it delivers to you. Preconditions are what the callee "
                    f"requires FROM you (you can't dictate them; you can "
                    f"only prove your call site satisfies whatever the "
                    f"callee actually publishes), and loop contracts are "
                    f"internal to the callee body. {remedy}"
                )



def _align_callsite_children(
    tracker: Any,
    callee_function: str,
    preconditions: list[str],
) -> None:
    """Align every parent call-site VO targeting *callee_function* to the
    callee's current *preconditions*: prune children for removed preconditions
    (preserving verified status for unchanged ones). Shared by the agent verify
    path; mirrors the reconcile layer's per-callee alignment so the child-VO set
    always tracks the published callee contract regardless of which side acts.
    """
    for parent_id, parent in list(tracker.parent_call_site_vos().items()):
        if parent.related_function == callee_function:
            tracker.sync_callsite_children(parent_id, preconditions)


def _update_child_vos(
    tracker: Any,
    callee_function: str,
    results: list[PreconditionResult],
) -> list[VOUpdate]:
    """Map per-(call_site_line, precondition_index) results to child VOs.

    Returns a list of VOUpdate records showing which VOs changed state.
    """
    # Build a lookup: (call_site_line, precondition_index) → verified
    result_lookup: dict[tuple[int, int], bool] = {}
    for r in results:
        key = (r.call_site_line, r.precondition_index)
        result_lookup[key] = r.verified

    # Per-precondition-index aggregated verdicts (fallback for call_site_line=0)
    per_index_verdicts: dict[int, bool] = {}
    for r in results:
        idx = r.precondition_index
        if idx not in per_index_verdicts:
            per_index_verdicts[idx] = r.verified
        else:
            per_index_verdicts[idx] = per_index_verdicts[idx] and r.verified

    updates: list[VOUpdate] = []

    for vo_id, vo in list(tracker.call_site_vos.items()):
        if vo.related_function != callee_function:
            continue

        new_status: str | None = None

        if vo.parent_vo_id is not None:
            # Child VO — match by (line, index)
            key = (vo.call_site_line, vo.precondition_index)
            if key in result_lookup:
                new_status = "verified" if result_lookup[key] else "failed"
            elif vo.precondition_index in per_index_verdicts:
                new_status = "verified" if per_index_verdicts[vo.precondition_index] else "failed"
        elif not vo.child_vo_ids:
            # Childless parent — aggregate
            if results and all(r.verified for r in results):
                new_status = "verified"
            elif results:
                new_status = "failed"

        if new_status == "verified":
            tracker.mark_verified(vo_id)
            updates.append(VOUpdate(vo_id=vo_id, status="verified"))
        elif new_status == "failed":
            tracker.mark_failed(vo_id)
            updates.append(VOUpdate(vo_id=vo_id, status="failed"))

    return updates
