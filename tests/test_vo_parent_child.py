"""Tests for VOTracker parent-child call-site VO lifecycle.

Covers:
- rebuild_callsite_children creates children + resets parent
- Parent status auto-derived from children (AND semantics)
- Rebuild with different precondition count replaces old children
- Rebuild with empty preconditions removes children + resets parent
- leaf_call_site_vos returns correct set
- is_vo_complete / all_verified respect leaf-only counting
- parent_call_site_vos excludes children
"""

from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker


def _make_parent(vo_id: str, callee: str = "helper", line: int = 14) -> VOEntry:
    return VOEntry(
        vo_id=vo_id,
        vo_type="call_site",
        description=f"CS {vo_id}",
        related_function=callee,
        call_site_line=line,
    )


# ---------------------------------------------------------------------------
# Basic rebuild
# ---------------------------------------------------------------------------


def test_rebuild_creates_children_and_resets_parent():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1", line=14))
    t.mark_verified("cs_1")

    new_ids = t.rebuild_callsite_children("cs_1", ["requires x > 0;", "requires y >= 0;"])
    assert new_ids == ["cs_1_0", "cs_1_1"]
    assert len(t.call_site_vos["cs_1"].child_vo_ids) == 2

    # Parent reset to PENDING
    assert t.call_site_vos["cs_1"].status == VOStatus.PENDING

    # Children are PENDING with correct metadata
    c0 = t.call_site_vos["cs_1_0"]
    assert c0.status == VOStatus.PENDING
    assert c0.parent_vo_id == "cs_1"
    assert c0.call_site_line == 14
    assert c0.precondition_index == 0
    assert c0.precondition_text == "requires x > 0;"
    assert c0.related_function == "helper"

    c1 = t.call_site_vos["cs_1_1"]
    assert c1.precondition_index == 1
    assert c1.precondition_text == "requires y >= 0;"


def test_rebuild_with_empty_list_removes_children_resets_parent():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["requires x > 0;"])
    t.mark_verified("cs_1_0")
    assert t.call_site_vos["cs_1"].status == VOStatus.VERIFIED

    # Rebuild with empty → removes child, resets parent
    t.rebuild_callsite_children("cs_1", [])
    assert "cs_1_0" not in t.call_site_vos
    assert t.call_site_vos["cs_1"].child_vo_ids == []
    assert t.call_site_vos["cs_1"].status == VOStatus.PENDING


def test_rebuild_replaces_old_children():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["requires a;", "requires b;"])
    t.mark_verified("cs_1_0")
    t.mark_verified("cs_1_1")

    # New preconditions: 3 items
    t.rebuild_callsite_children("cs_1", ["requires x;", "requires y;", "requires z;"])
    assert "cs_1_0" in t.call_site_vos  # new child 0
    assert "cs_1_1" in t.call_site_vos  # new child 1
    assert "cs_1_2" in t.call_site_vos  # new child 2
    assert t.call_site_vos["cs_1"].child_vo_ids == ["cs_1_0", "cs_1_1", "cs_1_2"]
    # All new children are PENDING
    assert all(
        t.call_site_vos[cid].status == VOStatus.PENDING
        for cid in ["cs_1_0", "cs_1_1", "cs_1_2"]
    )
    assert t.call_site_vos["cs_1_0"].precondition_text == "requires x;"


# ---------------------------------------------------------------------------
# Parent status derived from children
# ---------------------------------------------------------------------------


def test_parent_verified_when_all_children_verified():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["requires a;", "requires b;"])

    t.mark_verified("cs_1_0")
    assert t.call_site_vos["cs_1"].status == VOStatus.PENDING  # only 1/2

    t.mark_verified("cs_1_1")
    assert t.call_site_vos["cs_1"].status == VOStatus.VERIFIED  # all done


def test_parent_failed_when_any_child_failed():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["requires a;", "requires b;"])

    t.mark_verified("cs_1_0")
    t.mark_failed("cs_1_1")
    assert t.call_site_vos["cs_1"].status == VOStatus.FAILED


def test_parent_bug_when_child_bug_none_failed():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["requires a;", "requires b;"])

    t.mark_verified("cs_1_0")
    t.mark_bug("cs_1_1", "known limitation")
    assert t.call_site_vos["cs_1"].status == VOStatus.BUG


def test_parent_failed_takes_priority_over_bug():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b", "c"])

    t.mark_verified("cs_1_0")
    t.mark_bug("cs_1_1", "known")
    t.mark_failed("cs_1_2")
    assert t.call_site_vos["cs_1"].status == VOStatus.FAILED


# ---------------------------------------------------------------------------
# Leaf VO queries
# ---------------------------------------------------------------------------


def test_leaf_call_site_vos_returns_children_and_childless_parents():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))  # will get children
    t.add_call_site_vo(_make_parent("cs_2", callee="other"))  # childless
    t.rebuild_callsite_children("cs_1", ["requires a;"])

    leaves = t.leaf_call_site_vos()
    # cs_1 is a parent with children → excluded
    assert "cs_1" not in leaves
    # cs_1_0 is a child → included
    assert "cs_1_0" in leaves
    # cs_2 is childless parent → included
    assert "cs_2" in leaves


def test_parent_call_site_vos_excludes_children():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b"])

    parents = t.parent_call_site_vos()
    assert "cs_1" in parents
    assert "cs_1_0" not in parents
    assert "cs_1_1" not in parents


# ---------------------------------------------------------------------------
# Aggregate queries use leaf VOs
# ---------------------------------------------------------------------------


def test_all_verified_uses_leaves():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b"])

    t.mark_verified("cs_1_0")
    t.mark_verified("cs_1_1")
    assert t.all_verified()  # all leaves verified

    # Add a childless parent → breaks all_verified
    t.add_call_site_vo(_make_parent("cs_2", callee="other"))
    assert not t.all_verified()
    t.mark_verified("cs_2")
    assert t.all_verified()


def test_is_vo_complete_uses_leaves():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b"])

    t.mark_verified("cs_1_0")
    t.mark_bug("cs_1_1", "ok")
    assert t.is_vo_complete()  # verified + bug → complete


def test_pending_vos_uses_leaves():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b"])

    pending = t.pending_vos()
    # Two children are PENDING, parent is excluded
    assert len(pending) == 2
    assert all(v.parent_vo_id == "cs_1" for v in pending)


def test_failed_vos_uses_leaves():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a"])
    t.mark_failed("cs_1_0")

    failed = t.failed_vos()
    assert len(failed) == 1
    assert failed[0].vo_id == "cs_1_0"


# ---------------------------------------------------------------------------
# Multiple parents, multiple callees
# ---------------------------------------------------------------------------


def test_multiple_parents_different_callees():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1", callee="helper", line=10))
    t.add_call_site_vo(_make_parent("cs_2", callee="helper", line=20))
    t.add_call_site_vo(_make_parent("cs_3", callee="utils", line=30))

    # Rebuild for helper callee
    t.rebuild_callsite_children("cs_1", ["requires x > 0;"])
    t.rebuild_callsite_children("cs_2", ["requires x > 0;"])

    assert "cs_1_0" in t.call_site_vos
    assert "cs_2_0" in t.call_site_vos
    assert t.call_site_vos["cs_1_0"].call_site_line == 10
    assert t.call_site_vos["cs_2_0"].call_site_line == 20

    # utils callee has no children yet
    assert t.call_site_vos["cs_3"].child_vo_ids == []

    # Verify all helper children
    t.mark_verified("cs_1_0")
    t.mark_verified("cs_2_0")
    assert t.call_site_vos["cs_1"].status == VOStatus.VERIFIED
    assert t.call_site_vos["cs_2"].status == VOStatus.VERIFIED
    assert t.call_site_vos["cs_3"].status == VOStatus.PENDING

    # Not all_verified because cs_3 is still pending
    assert not t.all_verified()
    t.mark_verified("cs_3")
    assert t.all_verified()


# ---------------------------------------------------------------------------
# remove_children
# ---------------------------------------------------------------------------


def test_remove_children():
    t = VOTracker()
    t.add_call_site_vo(_make_parent("cs_1"))
    t.rebuild_callsite_children("cs_1", ["a", "b"])
    assert len(t.call_site_vos) == 3  # parent + 2 children

    t.remove_children("cs_1")
    assert len(t.call_site_vos) == 1  # parent only
    assert t.call_site_vos["cs_1"].child_vo_ids == []


# ---------------------------------------------------------------------------
# rebuild on nonexistent parent is no-op
# ---------------------------------------------------------------------------


def test_rebuild_nonexistent_parent():
    t = VOTracker()
    result = t.rebuild_callsite_children("nonexistent", ["a"])
    assert result == []
