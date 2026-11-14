"""Per-callee specification dependency descriptor.

A ``SpecDep`` records whether a caller's proof depends on a given callee's
published guarantees (postconditions / assigns). Verified via sufficiency
check: keep only the callee's postconditions (+ all assigns as upper bound),
ablate other callees' postconditions to ``ensures \\true``, and check that
all of the caller's proof targets still pass.

Three representable states (a missing entry is a fourth, implicit state):

- NONE (independent):    ``depends=False`` — the caller's proof still holds
  when the callee's postconditions are ablated to TOP. A postcondition
  change to the callee cannot affect the proof.
- ALL (coarse-dependent): ``depends=True, clauses=None`` — the proof depends
  on the callee but sufficiency could not isolate which clauses (e.g. the
  check failed, or multiple callees needed jointly). Any guarantee change
  forces re-verify.
- CLAUSES (fine-dependent): ``depends=True, clauses={texts}`` — the proof
  depends on these specific postcondition texts, verified sufficient via
  Layer-0 sufficiency check. The dependency judge keeps the caller trusted
  as long as ``clauses`` remain a subset of the callee's published
  postconditions.
- (absent entry) "not measured" — conservative default, treated as AFFECTED.
"""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class SpecDep:
    """How a caller's proof depends on one callee's published guarantees."""

    depends: bool
    clauses: frozenset[str] | None = None
