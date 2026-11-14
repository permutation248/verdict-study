"""The single dependency-discharge decision.

When a callee publishes a new authoritative version, the orchestrator must
decide, per caller, whether the caller's proof is *affected* (re-verify) or
*unaffected* (advance its dependency snapshot in place, no wake). This module
holds the one pure function that makes that call; both the event-driven
publication path and any residual staleness handling consult it, so the
decision logic lives in exactly one place.

Design (see docs/theoretical-foundation/07-spec-dep-architecture.md §4):
UNAFFECTED is only ever reached by a *positive* proof of independence — never
inferred from the kind of change. "additive vs modified" is not a concept here
(designed out): a fine-grained dependant is unaffected iff the specific clauses
it used are still present; a coarse dependant always re-verifies on a guarantee
change (and the re-verify succeeds when the change was harmless — wasted work,
never unsound).
"""

from __future__ import annotations

from verdict.core.types import FunctionSpec


def judge_dependency_change(
    caller_spec: FunctionSpec | None,
    callee_name: str,
    callee_pub: FunctionSpec | None,
) -> bool:
    """Return ``True`` if the caller is AFFECTED by the callee's new published
    spec (must re-verify), ``False`` if UNAFFECTED (safe to advance the
    snapshot without re-verifying).

    Conservative by construction: any uncertainty → AFFECTED. With no recorded
    ``spec_deps`` (the default until AUTO ablation is enabled), this always
    returns ``True``, so the wake behaviour is identical to a version-only
    backbone.
    """
    # Callee retracted or never really published → cannot trust anything.
    if callee_pub is None or callee_pub.version == 0:
        return True
    # No caller spec to consult → conservative.
    if caller_spec is None:
        return True

    support = caller_spec.spec_deps.get(callee_name)
    if support is None:
        return True  # not measured → conservative AFFECTED
    if not support.depends:
        return False  # NONE: proof holds with the callee ablated to TOP
    if support.clauses is None:
        return True  # ALL: coarse dependant re-verifies on any guarantee change

    # CLAUSES: unaffected iff every depended clause is still published.
    published = set(callee_pub.postconditions) | set(callee_pub.assigns)
    return not support.clauses.issubset(published)


def judge_precondition_change(
    caller_spec: FunctionSpec | None,
    callee_name: str,
    callee_pub: FunctionSpec | None,
) -> bool:
    """Reverse (precondition-direction) discharge for a caller's CALL-SITE
    proof. Return ``True`` if AFFECTED (the call site must be re-verified),
    ``False`` if UNAFFECTED.

    Keyed on ``callsite_pre_deps[callee_name]`` — the callee preconditions the
    caller's call-site proof was verified against. Unchanged → discharged.
    Any change, or no recorded proof while the callee has preconditions →
    conservative AFFECTED. (Sibling-frame postcondition dependencies are NOT
    handled here — they ride on the postcondition-direction judge above, since
    sufficiency check now covers call-site VOs.)
    """
    if callee_pub is None or callee_pub.version == 0:
        return True
    if caller_spec is None:
        return True
    recorded = caller_spec.callsite_pre_deps.get(callee_name)
    if recorded is None:
        # No recorded call-site proof. If the callee requires anything, the
        # caller must (re)prove its call site; if not, nothing to satisfy.
        return bool(callee_pub.preconditions)
    # Aligned with the postcondition judge's subset test, mirrored for the
    # satisfy-direction: UNAFFECTED iff every precondition the callee now
    # requires is one the caller already proved it satisfies. A removed or
    # relaxed precondition leaves the call site valid (the caller proved a
    # superset); an added or modified one the caller never proved → AFFECTED.
    # This is set containment on clause text (not logical monotonicity),
    # exactly as the postcondition direction.
    return not frozenset(callee_pub.preconditions).issubset(recorded)
