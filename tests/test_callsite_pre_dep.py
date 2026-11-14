"""Tests for the reverse (precondition-direction) call-site dependency.

Covers the two changes that make call-site discharge sound:
1. Call-site VOs are sufficiency-check targets (build_spec_deps adds each
   published callee's preconditions, labeled precondition_test; verify_spec_dep
   adds precondition_test to the WP target set). This captures sibling-frame
   postcondition dependencies in the POST direction.
2. judge_precondition_change discharges a call-site proof when the callee's
   preconditions are unchanged/relaxed (subset of what the caller proved),
   aligned with judge_dependency_change's subset test. callsite_pre_deps is
   recorded at publish time when call-site VOs are VERIFIED.
"""

import logging
from unittest.mock import AsyncMock, patch

import pytest

from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.core.project_context import ProjectContext
from verdict.core.vo_tracker import VOTracker, VOEntry, VOStatus
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.orchestrator.dependency_judge import judge_precondition_change

_LOG = logging.getLogger("test_callsite_pre_dep")


# ── judge_precondition_change: subset-aligned discharge ──


def _callee(pre, version=2):
    return FunctionSpec(
        function_name="callee", preconditions=pre, version=version,
        status=SpecStatus.PUBLISHED,
    )


def _caller_with(recorded):
    s = FunctionSpec(function_name="caller", status=SpecStatus.PUBLISHED)
    s.callsite_pre_deps = {"callee": frozenset(recorded)}
    return s


def test_pre_unchanged_discharged():
    caller = _caller_with(["requires x>0;", "requires \\valid(p);"])
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>0;", "requires \\valid(p);"])
    ) is False


def test_pre_relaxed_discharged():
    """Callee removed a precondition (subset of what caller proved) → discharge,
    mirroring the postcondition judge allowing additive changes."""
    caller = _caller_with(["requires x>0;", "requires \\valid(p);"])
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>0;"])
    ) is False


def test_pre_added_affected():
    """Callee added a precondition the caller never proved → AFFECTED."""
    caller = _caller_with(["requires x>0;"])
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>0;", "requires y>0;"])
    ) is True


def test_pre_modified_affected():
    """Callee changed a precondition's text → AFFECTED (set containment)."""
    caller = _caller_with(["requires x>0;"])
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>1;"])
    ) is True


def test_no_record_with_pre_conservative():
    """No recorded call-site proof + callee has preconditions → AFFECTED."""
    caller = FunctionSpec(function_name="caller", status=SpecStatus.PUBLISHED)
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>0;"])
    ) is True


def test_no_record_no_pre_unaffected():
    """No recorded proof + callee has no preconditions → nothing to satisfy."""
    caller = FunctionSpec(function_name="caller", status=SpecStatus.PUBLISHED)
    callee = FunctionSpec(
        function_name="callee", postconditions=["ensures \\result >= 0;"],
        version=2, status=SpecStatus.PUBLISHED,
    )
    assert judge_precondition_change(caller, "callee", callee) is False


def test_callee_unpublished_conservative():
    caller = _caller_with(["requires x>0;"])
    assert judge_precondition_change(caller, "callee", None) is True
    assert judge_precondition_change(
        caller, "callee", _callee(["requires x>0;"], version=0)
    ) is True


# ── _record_callsite_pre_deps_on_publish: record only when verified ──


def _ctx_caller_callee(callsite_status: VOStatus | None):
    """ToolContext where 'main' calls 'callee'; callee published with a
    precondition. Optionally seed a call-site VO with the given status."""
    info = FunctionInfo(name="main", source_file="t.c",
                        source_code="int main(){return callee();}", callees=["callee"])
    callee_info = FunctionInfo(name="callee", source_file="t.c",
                               source_code="int callee(){return 1;}", callers=["main"])
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_main", function_info=info))
    reg.register(AgentEntry(agent_id="agent_callee", function_info=callee_info))
    reg.get("agent_callee").published_spec = FunctionSpec(
        function_name="callee", preconditions=["requires x>0;"],
        version=1, status=SpecStatus.PUBLISHED,
    )
    tracker = VOTracker()
    if callsite_status is not None:
        tracker.add_call_site_vo(VOEntry(
            vo_id="cs_1", vo_type="call_site", description="cs",
            related_function="callee", status=callsite_status,
        ))
    bus = AsyncQueueBus()
    pc = ProjectContext(source_file="t.c", source_dir="/tmp", call_graph_dict={},
                        function_declarations_map={}, include_paths=[], work_dir="/tmp")
    return ToolContext(
        agent_id="agent_main", function_info=info, registry=reg,
        message_bus=bus,
        logger=_LOG, project_context=pc, vo_tracker=tracker,
    )


def test_record_when_callsite_verified():
    from verdict.tools.runtime.spec_io import _record_callsite_pre_deps_on_publish
    ctx = _ctx_caller_callee(VOStatus.VERIFIED)
    spec = FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED)
    _record_callsite_pre_deps_on_publish(ctx, spec)
    assert spec.callsite_pre_deps.get("callee") == frozenset(["requires x>0;"])


def test_no_record_when_callsite_pending():
    from verdict.tools.runtime.spec_io import _record_callsite_pre_deps_on_publish
    ctx = _ctx_caller_callee(VOStatus.PENDING)
    spec = FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED)
    _record_callsite_pre_deps_on_publish(ctx, spec)
    assert "callee" not in spec.callsite_pre_deps


# ── verify_spec_dep: call-site targets added under include_callsites ──


@pytest.mark.asyncio
async def test_include_callsites_adds_precondition_test_target():
    """include_callsites=True adds precondition_test to the WP target set."""
    import verdict.tools._spec_dep_check as sd
    from verdict.tools._cil_builder import label_preconditions

    annotations = {
        "preconditions": [], "postconditions": ["ensures \\true;"],
        "loop_contracts": {}, "assigns": [],
    }
    resolved_cc = {
        "callee": {
            "postconditions": ["ensures \\true;"], "assigns": [],
            "preconditions": label_preconditions(["requires x>0;"]),
        }
    }

    captured = {}

    async def fake_cil(*a, **k):
        return "/tmp/fake.c"

    async def fake_wp(cil, inc, targets, timeout=10):
        captured["targets"] = list(targets)
        return (True, "[wp] Proved goals: 2 / 2", ["cmd"])

    from unittest.mock import MagicMock
    pc = MagicMock(include_paths=[])

    with patch.object(sd, "build_annotated_cil_file", return_value="/tmp/fake.c"), \
         patch.object(sd, "run_wp_rte_targeted", new=fake_wp):
        await sd.verify_spec_dep(
            pc, "caller", annotations, resolved_cc, {"callee"}, ["rte_0"],
            "agent_x", _LOG, include_callsites=False,
        )
        off = list(captured["targets"])
        await sd.verify_spec_dep(
            pc, "caller", annotations, resolved_cc, {"callee"}, ["rte_0"],
            "agent_x", _LOG, include_callsites=True,
        )
        on = list(captured["targets"])

    assert "precondition_test" not in off
    assert "precondition_test" in on


# ── integration: both directions must discharge to skip the wake ──


@pytest.mark.asyncio
async def test_both_directions_discharge_skips_wake():
    """Guarantee-only change (pre unchanged) the caller does not depend on →
    both judges UNAFFECTED → no wake."""
    from verdict.core.spec_dep import SpecDep
    from verdict.orchestrator.orchestrator import Orchestrator

    callee_info = FunctionInfo(name="callee", source_file="c.c",
                               source_code="int callee(int x){return x;}",
                               callees=[], callers=["caller"])
    caller_info = FunctionInfo(name="caller", source_file="c.c",
                               source_code="int caller(){return callee(1);}",
                               callees=["callee"], callers=[])
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
    caller_spec.spec_deps = {"callee": SpecDep(depends=False)}  # post: independent
    caller_spec.callsite_pre_deps = {"callee": frozenset(["requires x >= 0;"])}  # pre: proved
    await reg.write_spec("agent_caller", caller_spec)
    reg.pop_publish_events()

    # callee adds a postcondition; precondition UNCHANGED.
    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", preconditions=["requires x >= 0;"],
        postconditions=["ensures \\result >= 0;", "ensures \\result <= 100;"],
        status=SpecStatus.PUBLISHED,
    ))
    orch._wakes.pending().clear()
    orch._pending_reasons = {}; orch._reconcile_dependency_staleness(); orch._derive_all_states(); orch._schedule_actionable()
    assert "agent_caller" not in orch._wakes.pending()  # discharged: no wake


@pytest.mark.asyncio
async def test_pre_change_wakes_even_if_post_independent():
    """Precondition change wakes the caller even when the post direction is
    NONE — the pre direction is AFFECTED."""
    from verdict.core.spec_dep import SpecDep
    from verdict.orchestrator.orchestrator import Orchestrator

    callee_info = FunctionInfo(name="callee", source_file="c.c",
                               source_code="int callee(int x){return x;}",
                               callees=[], callers=["caller"])
    caller_info = FunctionInfo(name="caller", source_file="c.c",
                               source_code="int caller(){return callee(1);}",
                               callees=["callee"], callers=[])
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
    caller_spec.callsite_pre_deps = {"callee": frozenset(["requires x >= 0;"])}
    await reg.write_spec("agent_caller", caller_spec)
    reg.pop_publish_events()

    # callee TIGHTENS its precondition (x>=0 -> x>=1): caller never proved x>=1.
    await reg.write_spec("agent_callee", FunctionSpec(
        function_name="callee", preconditions=["requires x >= 1;"],
        postconditions=["ensures \\result >= 0;"], status=SpecStatus.PUBLISHED,
    ))
    orch._wakes.pending().clear()
    orch._pending_reasons = {}; orch._reconcile_dependency_staleness(); orch._derive_all_states(); orch._schedule_actionable()
    assert "agent_caller" in orch._wakes.pending()  # pre direction affected → wake
