"""Tests for the dependency-discharge decision (Phase D).

Covers the pure judge truth table and the orchestrator's discharge filter:
a guarantee-only callee change that the caller provably does not depend on is
discharged (snapshot advanced, no wake); everything else wakes the caller. The
critical invariant: with no recorded spec_deps (the default), the judge
always returns AFFECTED, so wake behaviour is identical to the version-only
backbone.
"""

import pytest

from verdict.core.spec_dep import SpecDep
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.orchestrator.dependency_judge import judge_dependency_change


def _callee_pub(version=2, posts=None, assigns=None):
    return FunctionSpec(
        function_name="callee",
        postconditions=posts or ["ensures \\result >= 0;"],
        assigns=assigns or ["assigns \\nothing;"],
        status=SpecStatus.PUBLISHED,
        version=version,
    )


def _caller(support=None):
    spec = FunctionSpec(function_name="caller", status=SpecStatus.PUBLISHED)
    if support is not None:
        spec.spec_deps = support
    return spec


# ── judge truth table ──


def test_judge_none_callee_pub_is_affected():
    assert judge_dependency_change(_caller(), "callee", None) is True


def test_judge_callee_version_zero_is_affected():
    assert judge_dependency_change(_caller(), "callee", _callee_pub(version=0)) is True


def test_judge_no_caller_spec_is_affected():
    assert judge_dependency_change(None, "callee", _callee_pub()) is True


def test_judge_no_support_entry_is_affected():
    # caller published but never measured support for this callee
    assert judge_dependency_change(_caller({}), "callee", _callee_pub()) is True


def test_judge_independent_is_unaffected():
    support = {"callee": SpecDep(depends=False)}
    assert judge_dependency_change(_caller(support), "callee", _callee_pub()) is False


def test_judge_coarse_dependent_is_affected():
    support = {"callee": SpecDep(depends=True, clauses=None)}
    assert judge_dependency_change(_caller(support), "callee", _callee_pub()) is True


def test_judge_clauses_still_present_is_unaffected():
    support = {
        "callee": SpecDep(
            depends=True, clauses=frozenset({"ensures \\result >= 0;"})
        )
    }
    pub = _callee_pub(posts=["ensures \\result >= 0;", "ensures \\result < 100;"])
    assert judge_dependency_change(_caller(support), "callee", pub) is False


def test_judge_clause_dropped_is_affected():
    support = {
        "callee": SpecDep(
            depends=True, clauses=frozenset({"ensures \\result >= 0;"})
        )
    }
    pub = _callee_pub(posts=["ensures \\result < 100;"])  # the depended clause is gone
    assert judge_dependency_change(_caller(support), "callee", pub) is True


def test_judge_clause_in_assigns_dimension():
    support = {
        "callee": SpecDep(depends=True, clauses=frozenset({"assigns *p;"}))
    }
    pub = _callee_pub(assigns=["assigns *p;"])
    assert judge_dependency_change(_caller(support), "callee", pub) is False


# ── orchestrator discharge filter ──


def _registry_with_caller_callee(caller_support=None):
    """caller -> callee; callee published v1; caller published v1 based_on callee v1."""
    reg = AgentRegistry()
    callee_info = FunctionInfo(
        name="callee", source_file="c.c", source_code="int callee(){return 0;}",
        callees=[], callers=["caller"],
    )
    caller_info = FunctionInfo(
        name="caller", source_file="c.c", source_code="int caller(){return callee();}",
        callees=["callee"], callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_callee", function_info=callee_info))
    reg.register(AgentEntry(agent_id="agent_caller", function_info=caller_info))
    return reg


@pytest.mark.asyncio
async def test_discharge_advances_snapshot_no_wake():
    """Caller with depends=False is discharged: snapshot advances, not enqueued."""
    from verdict.orchestrator.orchestrator import Orchestrator

    callee_info = FunctionInfo(
        name="callee", source_file="c.c", source_code="int callee(){return 0;}",
        callees=[], callers=["caller"],
    )
    caller_info = FunctionInfo(
        name="caller", source_file="c.c", source_code="int caller(){return callee();}",
        callees=["callee"], callers=[],
    )
    orch = Orchestrator([callee_info, caller_info], timeout=10)
    reg = orch.registry

    # callee publishes v1, then v2 (guarantee change only).
    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))
    # caller publishes, recording depends=False on callee, based_on callee v1.
    caller_spec = FunctionSpec(
        function_name="caller", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    caller_spec.spec_deps = {"callee": SpecDep(depends=False)}
    await reg.write_spec("agent_caller", caller_spec)

    reg.pop_publish_events()  # drain the publishes so far

    # callee re-publishes v2 with an added postcondition (guarantee change).
    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee",
        postconditions=["ensures \\result >= 0;", "ensures \\result < 100;"],
        status=SpecStatus.PUBLISHED,
    ))

    orch._wakes.pending().clear()
    orch._pending_reasons = {}; orch._reconcile_dependency_staleness(); orch._derive_all_states(); orch._schedule_actionable()

    # Caller was discharged: not enqueued, snapshot advanced to callee v2.
    assert "agent_caller" not in orch._wakes.pending()
    caller_entry = reg.get("agent_caller")
    callee_v = reg.get("agent_callee").published_spec.version
    assert caller_entry.published_spec.based_on_versions.get("callee") == callee_v


@pytest.mark.asyncio
async def test_no_support_wakes_caller():
    """Regression: with no recorded support, a guarantee change wakes the caller
    (identical to the version-only backbone)."""
    from verdict.orchestrator.orchestrator import Orchestrator

    callee_info = FunctionInfo(
        name="callee", source_file="c.c", source_code="int callee(){return 0;}",
        callees=[], callers=["caller"],
    )
    caller_info = FunctionInfo(
        name="caller", source_file="c.c", source_code="int caller(){return callee();}",
        callees=["callee"], callers=[],
    )
    orch = Orchestrator([callee_info, caller_info], timeout=10)
    reg = orch.registry

    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))
    # caller publishes WITHOUT support recorded.
    await reg.write_spec("agent_caller", FunctionSpec(
        function_name="caller", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))
    reg.pop_publish_events()

    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee",
        postconditions=["ensures \\result >= 0;", "ensures \\result < 100;"],
        status=SpecStatus.PUBLISHED,
    ))

    orch._wakes.pending().clear()
    orch._pending_reasons = {}; orch._reconcile_dependency_staleness(); orch._derive_all_states(); orch._schedule_actionable()

    assert "agent_caller" in orch._wakes.pending()  # conservative wake


@pytest.mark.asyncio
async def test_precondition_change_never_discharged():
    """A precondition change must wake the caller even with depends=False
    (the judge covers guarantees, not call-site obligations)."""
    from verdict.orchestrator.orchestrator import Orchestrator

    callee_info = FunctionInfo(
        name="callee", source_file="c.c", source_code="int callee(int x){return x;}",
        callees=[], callers=["caller"],
    )
    caller_info = FunctionInfo(
        name="caller", source_file="c.c", source_code="int caller(){return callee(1);}",
        callees=["callee"], callers=[],
    )
    orch = Orchestrator([callee_info, caller_info], timeout=10)
    reg = orch.registry

    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", preconditions=["requires x >= 0;"],
        postconditions=["ensures \\result >= 0;"], status=SpecStatus.PUBLISHED,
    ))
    caller_spec = FunctionSpec(
        function_name="caller", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    caller_spec.spec_deps = {"callee": SpecDep(depends=False)}
    await reg.write_spec("agent_caller", caller_spec)
    reg.pop_publish_events()

    # callee changes its PRECONDITION (and a postcondition) → must wake.
    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", preconditions=["requires x >= 1;"],
        postconditions=["ensures \\result >= 0;"], status=SpecStatus.PUBLISHED,
    ))

    orch._wakes.pending().clear()
    orch._pending_reasons = {}; orch._reconcile_dependency_staleness(); orch._derive_all_states(); orch._schedule_actionable()

    assert "agent_caller" in orch._wakes.pending()  # precondition change forces wake
