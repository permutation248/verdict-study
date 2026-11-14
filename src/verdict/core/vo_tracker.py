"""VOTracker: per-agent verification obligation state tracking."""

from __future__ import annotations

import enum
from dataclasses import dataclass, field
from typing import Callable


class VOStatus(enum.Enum):
    """Status of a single verification obligation."""

    PENDING = "pending"
    VERIFIED = "verified"
    FAILED = "failed"
    BUG = "bug"  # agent-declared: cannot be proven, accepted as known issue


@dataclass
class VOEntry:
    """A single verification obligation tracked by an agent."""

    vo_id: str
    vo_type: str  # "rte" | "call_site"
    description: str
    status: VOStatus = VOStatus.PENDING
    assertion_content: str = ""
    related_function: str = ""  # RTE: self; call-site: callee name
    bug_reason: str | None = None  # populated when status == BUG
    # Parent-child fields for call-site VOs
    parent_vo_id: str | None = None  # non-None for child VOs
    child_vo_ids: list[str] = field(default_factory=list)  # non-empty for parent VOs with children
    call_site_line: int | None = None  # source line of the physical call site
    precondition_index: int | None = None  # child only: index in callee's preconditions
    precondition_text: str | None = None  # child only: the ACSL requires clause


class VOTracker:
    """Per-agent VO state tracker.

    Three VO categories:

    - **RTE**: one per RTE assertion, assigned at init.
    - **Call-site**: two-level (parent per physical call site, children
      per callee precondition), rebuilt when callee publishes.
    - **Spec**: a single entry representing the function's own contract
      (postconditions, assigns, loop contracts). Marked PENDING when a
      callee's guarantees change; marked VERIFIED when
      ``verify_function_contract`` succeeds against published callee
      contracts.

    Parent status for call-site VOs is derived from children via AND:
      - all children VERIFIED → parent VERIFIED
      - any child FAILED → parent FAILED
      - any child BUG (none FAILED) → parent BUG
      - otherwise → PENDING
    """

    def __init__(self) -> None:
        self.rte_vos: dict[str, VOEntry] = {}
        self.call_site_vos: dict[str, VOEntry] = {}
        self.spec_vo: VOEntry | None = None
        self._on_change: Callable[[], None] | None = None

    # -- add --

    def add_rte_vo(self, vo: VOEntry) -> None:
        self.rte_vos[vo.vo_id] = vo

    def add_call_site_vo(self, vo: VOEntry) -> None:
        self.call_site_vos[vo.vo_id] = vo

    # -- spec VO --

    def invalidate_spec(self, reason: str) -> None:
        """Mark the function's own contract as stale (needs re-verification).

        Called by the orchestrator when a callee publishes changed
        guarantees. *reason* is surfaced in the wake prompt so the agent
        knows WHY its spec is stale.
        """
        self.spec_vo = VOEntry(
            vo_id="spec",
            vo_type="spec",
            description=reason,
            status=VOStatus.PENDING,
        )
        if self._on_change:
            self._on_change()

    def mark_spec_verified(self) -> None:
        """Mark the spec VO as verified (verify_function_contract succeeded)."""
        if self.spec_vo is None:
            self.spec_vo = VOEntry(
                vo_id="spec",
                vo_type="spec",
                description="function contract verified",
                status=VOStatus.VERIFIED,
            )
        else:
            self.spec_vo.status = VOStatus.VERIFIED
        if self._on_change:
            self._on_change()

    # -- mark (with auto parent recompute) --

    def mark_verified(self, vo_id: str) -> None:
        entry = self._find(vo_id)
        if entry:
            entry.status = VOStatus.VERIFIED
            if entry.parent_vo_id:
                self._recompute_parent_status(entry.parent_vo_id)
            if self._on_change:
                self._on_change()

    def mark_failed(self, vo_id: str) -> None:
        entry = self._find(vo_id)
        if entry:
            entry.status = VOStatus.FAILED
            if entry.parent_vo_id:
                self._recompute_parent_status(entry.parent_vo_id)
            if self._on_change:
                self._on_change()

    def mark_pending(self, vo_id: str) -> None:
        entry = self._find(vo_id)
        if entry:
            entry.status = VOStatus.PENDING
            entry.bug_reason = None
            if entry.parent_vo_id:
                self._recompute_parent_status(entry.parent_vo_id)
            if self._on_change:
                self._on_change()

    def mark_bug(self, vo_id: str, reason: str) -> None:
        entry = self._find(vo_id)
        if entry:
            entry.status = VOStatus.BUG
            entry.bug_reason = reason
            if entry.parent_vo_id:
                self._recompute_parent_status(entry.parent_vo_id)
            if self._on_change:
                self._on_change()

    # -- child VO lifecycle --

    def rebuild_callsite_children(
        self,
        parent_vo_id: str,
        preconditions: list[str],
    ) -> list[str]:
        """Remove old children and create new ones for *parent_vo_id*.

        Called by the orchestrator whenever the callee publishes or
        updates its preconditions. Returns newly created child VO IDs.
        """
        parent = self.call_site_vos.get(parent_vo_id)
        if parent is None:
            return []

        # Tear down old children
        for old_id in list(parent.child_vo_ids):
            self.call_site_vos.pop(old_id, None)
        parent.child_vo_ids.clear()

        # Create new children
        new_ids: list[str] = []
        for i, pre_text in enumerate(preconditions):
            child_id = f"{parent_vo_id}_{i}"
            child = VOEntry(
                vo_id=child_id,
                vo_type="call_site",
                description=f"Precondition {i}: {pre_text[:60]}",
                related_function=parent.related_function,
                parent_vo_id=parent_vo_id,
                call_site_line=parent.call_site_line,
                precondition_index=i,
                precondition_text=pre_text,
            )
            self.call_site_vos[child_id] = child
            parent.child_vo_ids.append(child_id)
            new_ids.append(child_id)

        # Reset parent to PENDING (children are all PENDING)
        parent.status = VOStatus.PENDING
        parent.bug_reason = None

        if self._on_change:
            self._on_change()
        return new_ids

    def sync_callsite_children(
        self,
        parent_vo_id: str,
        preconditions: list[str],
    ) -> None:
        """Align a parent's children with *preconditions*, preserving status.

        Twin of ``rebuild_callsite_children`` for the advance path (the
        call-site proof is still valid, but the callee's precondition set may
        have changed). Children whose precondition text is unchanged keep their
        status (VERIFIED stays VERIFIED, bug_reason preserved); children for
        removed/changed preconditions are dropped or reset to PENDING. Parent
        status is then re-derived from the surviving children.
        """
        parent = self.call_site_vos.get(parent_vo_id)
        if parent is None:
            return

        old_children = {
            cid: self.call_site_vos[cid]
            for cid in parent.child_vo_ids
            if cid in self.call_site_vos
        }

        # Tear down old children
        for old_id in list(parent.child_vo_ids):
            self.call_site_vos.pop(old_id, None)
        parent.child_vo_ids.clear()

        # Recreate, inheriting status from the matching (same-text) old child
        for i, pre_text in enumerate(preconditions):
            child_id = f"{parent_vo_id}_{i}"
            old_child = old_children.get(child_id)
            unchanged = (
                old_child is not None
                and old_child.precondition_text == pre_text
            )
            child = VOEntry(
                vo_id=child_id,
                vo_type="call_site",
                description=f"Precondition {i}: {pre_text[:60]}",
                related_function=parent.related_function,
                parent_vo_id=parent_vo_id,
                call_site_line=parent.call_site_line,
                precondition_index=i,
                precondition_text=pre_text,
            )
            child.status = old_child.status if unchanged else VOStatus.PENDING
            if unchanged:
                child.bug_reason = old_child.bug_reason
            self.call_site_vos[child_id] = child
            parent.child_vo_ids.append(child_id)

        self._recompute_parent_status(parent_vo_id)
        if self._on_change:
            self._on_change()

    def remove_children(self, parent_vo_id: str) -> None:
        """Remove all children of a parent VO (callee dropped all preconditions)."""
        parent = self.call_site_vos.get(parent_vo_id)
        if parent is None:
            return
        for old_id in list(parent.child_vo_ids):
            self.call_site_vos.pop(old_id, None)
        parent.child_vo_ids.clear()
        if self._on_change:
            self._on_change()

    def _recompute_parent_status(self, parent_vo_id: str) -> None:
        """Derive parent status from children (AND semantics)."""
        parent = self.call_site_vos.get(parent_vo_id)
        if parent is None or not parent.child_vo_ids:
            return

        children = [
            self.call_site_vos[cid]
            for cid in parent.child_vo_ids
            if cid in self.call_site_vos
        ]
        if not children:
            return

        statuses = [c.status for c in children]
        if all(s == VOStatus.VERIFIED for s in statuses):
            parent.status = VOStatus.VERIFIED
        elif any(s == VOStatus.FAILED for s in statuses):
            parent.status = VOStatus.FAILED
        elif any(s == VOStatus.BUG for s in statuses):
            parent.status = VOStatus.BUG
        else:
            parent.status = VOStatus.PENDING

    # -- leaf VO queries --
    # "Leaf" = children + childless parents. Parents with children are
    # containers whose status is derived; they are excluded from counts.

    def leaf_call_site_vos(self) -> dict[str, VOEntry]:
        """Return only the call-site VOs that carry independent status."""
        return {
            vo_id: vo
            for vo_id, vo in self.call_site_vos.items()
            if vo.parent_vo_id is not None  # child
            or not vo.child_vo_ids  # childless parent
        }

    def _all_leaf_vos(self) -> list[VOEntry]:
        vos = list(self.rte_vos.values()) + list(self.leaf_call_site_vos().values())
        if self.spec_vo is not None:
            vos.append(self.spec_vo)
        return vos

    def all_verified(self) -> bool:
        vos = self._all_leaf_vos()
        return len(vos) > 0 and all(v.status == VOStatus.VERIFIED for v in vos)

    def is_vo_complete(self) -> bool:
        """True iff no leaf VO is PENDING or FAILED. Empty tracker -> True.

        STRICT completeness: every leaf VO (incl. call-site VOs whose callee
        has not published yet) must be VERIFIED/BUG. Used for GLOBAL_OK, where
        ``deps_current`` already guarantees all callees have published, so no
        call-site VO can be legitimately deferred.
        """
        return all(
            v.status in (VOStatus.VERIFIED, VOStatus.BUG)
            for v in self._all_leaf_vos()
        )

    def is_locally_complete(self, is_published) -> bool:
        """LOCAL completeness — matches the publish gate exactly.

        Like ``is_vo_complete`` but a call-site VO that is PENDING *because
        its callee has not published yet* is treated as deferred (allowed),
        not incomplete. This is the LOCAL_OK / publishability predicate:
        the agent has discharged every obligation it currently can.

        ``is_published(func_name) -> bool`` reports whether a callee has a
        published spec (injected so the tracker stays free of registry deps).
        """
        if self.spec_vo is not None and self.spec_vo.status == VOStatus.PENDING:
            return False
        for vo in self.rte_vos.values():
            if vo.status in (VOStatus.PENDING, VOStatus.FAILED):
                return False
        for vo in self.leaf_call_site_vos().values():
            if vo.status == VOStatus.FAILED:
                return False
            if vo.status == VOStatus.PENDING and is_published(vo.related_function):
                return False
        return True

    def pending_vos(self) -> list[VOEntry]:
        return [v for v in self._all_leaf_vos() if v.status == VOStatus.PENDING]

    def failed_vos(self) -> list[VOEntry]:
        return [v for v in self._all_leaf_vos() if v.status == VOStatus.FAILED]

    def bug_vos(self) -> list[VOEntry]:
        return [v for v in self._all_leaf_vos() if v.status == VOStatus.BUG]

    # -- parent VO queries --

    def parent_call_site_vos(self) -> dict[str, VOEntry]:
        """Return only parent call-site VOs (no parent_vo_id)."""
        return {
            vo_id: vo
            for vo_id, vo in self.call_site_vos.items()
            if vo.parent_vo_id is None
        }

    # -- lookup --

    def _find(self, vo_id: str) -> VOEntry | None:
        if vo_id == "spec" and self.spec_vo is not None:
            return self.spec_vo
        return self.rte_vos.get(vo_id) or self.call_site_vos.get(vo_id)
