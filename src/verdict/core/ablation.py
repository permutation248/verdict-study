"""Ablation switches for the coordination-mechanism experiment (RQ3).

An :class:`AblationConfig` names what is switched OFF. The default is
"nothing" — every consumer treats the default instance as the normal,
fully-enabled system, so threading this object through the stack has
zero effect unless a switch is set explicitly via ``--ablate``.

Design rule (see scripts/experiments/rq3/README.md): an ablation is the
*absence of a capability* (tools removed, parameters removed,
hypotheses not taken), never a behavioral fork inside the remaining
machinery. Downstream systems (anchoring, debt wakes, demand rescues)
go quiet by data flow, not by checks against this config.
"""

from __future__ import annotations

from dataclasses import dataclass

_VALID = ("demands", "assume")


@dataclass(frozen=True)
class AblationConfig:
    """What is switched OFF. Default: nothing (normal operation).

    demands: the proof-demand protocol (submit/resolve tools absent).
    assume:  optimistic callee assumption (no ``assume_callee``, no
             unpublished-callee hypotheses; extern environment axioms
             are exempt — they are environment modeling, not peer
             optimism).
    """

    demands: bool = False
    assume: bool = False

    @classmethod
    def parse(cls, spec: str | None) -> "AblationConfig":
        """Parse a ``--ablate`` value like ``"demands"`` or ``"demands,assume"``."""
        if not spec:
            return cls()
        items = {s.strip() for s in spec.split(",") if s.strip()}
        unknown = items - set(_VALID)
        if unknown:
            raise ValueError(
                f"unknown --ablate value(s): {sorted(unknown)}; "
                f"valid: {list(_VALID)}"
            )
        return cls(demands="demands" in items, assume="assume" in items)

    def active(self) -> list[str]:
        """Names of switched-off mechanisms (empty in normal operation)."""
        return [name for name in _VALID if getattr(self, name)]
