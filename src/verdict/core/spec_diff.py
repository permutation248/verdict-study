"""Spec diffing: pure comparison of two published FunctionSpecs.

Answers "are these the same public contract?" (``_published_equivalent``),
"which clause kinds changed?" (``_changed_kinds``), and a structured
classification (``classify_spec_change``) the orchestrator uses to decide
caller-side VO invalidation. No state, no I/O.
"""

from __future__ import annotations

from dataclasses import dataclass

from verdict.core.types import FunctionSpec


def _published_equivalent(left: FunctionSpec, right: FunctionSpec) -> bool:
    """Two published specs are equivalent iff their *public contract body* is
    identical. Dependency bookkeeping (``based_on_versions``, ``spec_deps``)
    is deliberately EXCLUDED: it is the caller's private record of how/against
    what it proved its contract, not part of the contract callers observe.

    Including it here was the engine of a self-perpetuating version-bump wave:
    a callee re-publishing identical content still bumps its version, the caller
    wakes and re-pins ``based_on_versions``, then re-publishes a byte-identical
    contract that — only because the snapshot moved — was judged "changed",
    bumping the caller's version and waking ITS callers, ad infinitum. Comparing
    contract body only lets a no-op re-verify be recognised as a fixpoint. The
    advanced snapshot is still persisted (see ``write_spec`` dedup branch), just
    without a public version bump or publish event.
    """
    return (
        left.preconditions == right.preconditions
        and left.postconditions == right.postconditions
        and left.loop_contracts == right.loop_contracts
        and left.assigns == right.assigns
        and left.status == right.status
    )


def _changed_kinds(
    old: FunctionSpec | None,
    new: FunctionSpec,
) -> list[str]:
    if old is None:
        kinds = []
        if new.preconditions:
            kinds.append("precondition")
        if new.postconditions:
            kinds.append("postcondition")
        if new.assigns:
            kinds.append("assigns")
        if new.loop_contracts:
            kinds.append("loop_contract")
        return kinds or ["published"]

    kinds: list[str] = []
    if old.preconditions != new.preconditions:
        kinds.append("precondition")
    if old.postconditions != new.postconditions:
        kinds.append("postcondition")
    if old.assigns != new.assigns:
        kinds.append("assigns")
    if old.loop_contracts != new.loop_contracts:
        kinds.append("loop_contract")
    return kinds


@dataclass
class SpecChangeClassification:
    """Structured classification of how a published spec changed.

    Used by the orchestrator to decide what caller-side VOs to invalidate.

    - precondition_changed: any change to preconditions (new/removed/modified)
    - postcondition_additive_only: new postconditions are a superset (string-wise)
      of the old ones — callers that relied on old guarantees are still sound
    - postcondition_weakened_or_modified: any other change — callers' VOs that
      leaned on removed/modified clauses may be invalid
    - assigns_changed: any change to assigns (conservatively invalidates all)
    - loop_contract_changed: change to loop contracts (internal to callee,
      does NOT affect callers)
    """

    precondition_changed: bool = False
    postcondition_additive_only: bool = False
    postcondition_weakened_or_modified: bool = False
    assigns_changed: bool = False
    loop_contract_changed: bool = False


def classify_spec_change(
    old: FunctionSpec | None,
    new: FunctionSpec,
) -> SpecChangeClassification:
    """Produce a structured change classification between two published specs."""
    result = SpecChangeClassification()

    old_pre = set(old.preconditions) if old else set()
    new_pre = set(new.preconditions)
    if old_pre != new_pre:
        result.precondition_changed = True

    old_post = set(old.postconditions) if old else set()
    new_post = set(new.postconditions)
    if old_post != new_post:
        if old_post.issubset(new_post):
            result.postcondition_additive_only = True
        else:
            result.postcondition_weakened_or_modified = True

    old_assigns = set(old.assigns) if old else set()
    new_assigns = set(new.assigns)
    if old_assigns != new_assigns:
        result.assigns_changed = True

    old_loops = old.loop_contracts if old else {}
    new_loops = new.loop_contracts
    if old_loops != new_loops:
        result.loop_contract_changed = True

    return result
