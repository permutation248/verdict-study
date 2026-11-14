"""Unit tests for VOTracker."""

from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker


def _make_rte(vo_id: str) -> VOEntry:
    return VOEntry(vo_id=vo_id, vo_type="rte", description=f"RTE {vo_id}")


def _make_cs(vo_id: str, callee: str = "helper") -> VOEntry:
    return VOEntry(
        vo_id=vo_id, vo_type="call_site",
        description=f"CS {vo_id}", related_function=callee,
    )


def test_empty_tracker_not_verified():
    t = VOTracker()
    assert not t.all_verified()
    assert t.pending_vos() == []


def test_add_and_mark_rte():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.add_rte_vo(_make_rte("rte_2"))
    assert len(t.pending_vos()) == 2
    assert not t.all_verified()

    t.mark_verified("rte_1")
    assert len(t.pending_vos()) == 1
    assert not t.all_verified()

    t.mark_verified("rte_2")
    assert t.all_verified()
    assert t.pending_vos() == []


def test_add_call_site_vo():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.mark_verified("rte_1")
    assert t.all_verified()

    # Adding a call-site VO breaks all_verified
    t.add_call_site_vo(_make_cs("cs_1"))
    assert not t.all_verified()

    t.mark_verified("cs_1")
    assert t.all_verified()


def test_mark_failed_and_pending():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.mark_verified("rte_1")
    assert t.all_verified()

    t.mark_failed("rte_1")
    assert not t.all_verified()
    assert len(t.failed_vos()) == 1

    t.mark_pending("rte_1")
    assert len(t.pending_vos()) == 1
    assert len(t.failed_vos()) == 0


def test_mark_nonexistent_is_noop():
    t = VOTracker()
    t.mark_verified("nonexistent")  # should not raise
    t.mark_failed("nonexistent")
    t.mark_pending("nonexistent")


def test_mark_bug_requires_reason_and_persists():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.mark_bug("rte_1", "division is intentional UB sink for test harness")

    entry = t.rte_vos["rte_1"]
    assert entry.status == VOStatus.BUG
    assert "intentional" in entry.bug_reason
    assert len(t.bug_vos()) == 1


def test_is_vo_complete_with_mixed_states():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.add_rte_vo(_make_rte("rte_2"))
    t.add_call_site_vo(_make_cs("cs_1"))

    # All PENDING → not complete
    assert not t.is_vo_complete()

    t.mark_verified("rte_1")
    t.mark_verified("cs_1")
    assert not t.is_vo_complete()  # rte_2 still PENDING

    t.mark_bug("rte_2", "accepted as known limitation")
    assert t.is_vo_complete()  # verified + verified + bug → complete


def test_is_vo_complete_rejects_failed():
    t = VOTracker()
    t.add_rte_vo(_make_rte("rte_1"))
    t.mark_failed("rte_1")
    assert not t.is_vo_complete()  # FAILED blocks completeness


def test_empty_tracker_is_trivially_complete():
    t = VOTracker()
    assert t.is_vo_complete()  # no VOs → nothing to prove
