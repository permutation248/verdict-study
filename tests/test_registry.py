"""Tests for AgentRegistry."""

import pytest
import asyncio

from verdict.core.types import DemandStatus, FunctionInfo, FunctionSpec, SpecStatus
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.spec_diff import classify_spec_change


def test_classify_spec_change_fresh_publication():
    new = FunctionSpec(
        function_name="f",
        preconditions=["requires x > 0;"],
        postconditions=["ensures \\result >= 0;"],
    )
    c = classify_spec_change(None, new)
    assert c.precondition_changed is True
    assert c.postcondition_additive_only is True
    assert c.postcondition_weakened_or_modified is False


def test_classify_spec_change_postcondition_additive():
    old = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;"],
    )
    new = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;", "ensures \\result < 100;"],
    )
    c = classify_spec_change(old, new)
    assert c.postcondition_additive_only is True
    assert c.postcondition_weakened_or_modified is False


def test_classify_spec_change_postcondition_removed():
    old = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;", "ensures \\result < 100;"],
    )
    new = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;"],
    )
    c = classify_spec_change(old, new)
    assert c.postcondition_weakened_or_modified is True
    assert c.postcondition_additive_only is False


def test_classify_spec_change_postcondition_modified():
    old = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;"],
    )
    new = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result > 0;"],  # modified, not added
    )
    c = classify_spec_change(old, new)
    assert c.postcondition_weakened_or_modified is True


def test_classify_spec_change_precondition_any_change_flagged():
    old = FunctionSpec(
        function_name="f",
        preconditions=["requires x > 0;"],
    )
    new = FunctionSpec(
        function_name="f",
        preconditions=["requires x >= 0;"],
    )
    c = classify_spec_change(old, new)
    assert c.precondition_changed is True


def test_classify_spec_change_assigns_any_change():
    old = FunctionSpec(function_name="f", assigns=["assigns \\nothing;"])
    new = FunctionSpec(function_name="f", assigns=["assigns *p;"])
    c = classify_spec_change(old, new)
    assert c.assigns_changed is True


def test_classify_spec_change_loop_contract_isolated():
    old = FunctionSpec(
        function_name="f",
        loop_contracts={"L1": ["loop invariant i >= 0;"]},
    )
    new = FunctionSpec(
        function_name="f",
        loop_contracts={"L1": ["loop invariant i > 0;"]},
    )
    c = classify_spec_change(old, new)
    assert c.loop_contract_changed is True
    assert c.precondition_changed is False
    assert c.postcondition_additive_only is False


def test_register_and_get(registry, sample_function_info):
    entry = registry.get("agent_add")
    assert entry is not None
    assert entry.function_info.name == "add"


def test_get_by_function(registry):
    entry = registry.get_by_function("add")
    assert entry is not None
    assert entry.agent_id == "agent_add"


def test_get_unknown_returns_none(registry):
    assert registry.get("nonexistent") is None
    assert registry.get_by_function("nonexistent") is None


def test_read_spec(registry):
    assert registry.read_spec("add") is None
    working = registry.read_working_spec("add")
    assert working is not None
    assert working.function_name == "add"


@pytest.mark.asyncio
async def test_write_spec_owner(registry):
    new_spec = FunctionSpec(function_name="add", preconditions=["a > 0"])
    await registry.write_spec("agent_add", new_spec)
    assert registry.read_working_spec("add").preconditions == ["a > 0"]
    assert registry.read_spec("add") is None


@pytest.mark.asyncio
async def test_write_published_spec_owner(registry):
    published = FunctionSpec(
        function_name="add",
        preconditions=["a > 0"],
        status=SpecStatus.PUBLISHED,
    )
    await registry.write_spec("agent_add", published)
    authoritative = registry.read_spec("add")
    assert authoritative is not None
    assert authoritative.preconditions == ["a > 0"]
    assert authoritative.status == SpecStatus.PUBLISHED
    assert authoritative.version == 1
    assert authoritative.based_on_versions == {}


@pytest.mark.asyncio
async def test_equivalent_published_spec_does_not_bump_version(registry):
    first = await registry.write_spec(
        "agent_add",
        FunctionSpec(function_name="add", status=SpecStatus.PUBLISHED),
    )
    first_events = registry.pop_publish_events()
    assert first.version == 1
    assert len(first_events) == 1

    second = await registry.write_spec(
        "agent_add",
        FunctionSpec(function_name="add", status=SpecStatus.PUBLISHED),
    )
    second_events = registry.pop_publish_events()
    assert second.version == 1
    assert second_events == []
    assert len(registry.spec_history("add")) == 1


@pytest.mark.asyncio
async def test_snapshot_only_change_is_noop_no_cascade(sample_function_info_callee):
    """Regression: a caller re-publishing a byte-identical contract after a
    callee version bump must NOT bump its own version or emit a publish event
    (which would wake ITS callers) — only refresh based_on in place. This was
    the root of the get_identifier/parse_id_len version-churn oscillation."""
    from verdict.core.spec_dep import SpecDep

    reg = AgentRegistry()
    caller = FunctionInfo(
        name="add", source_file="m.c", source_code="int add(int a){return helper(a);}",
        callees=["helper"], callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_add", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    body = dict(
        function_name="add", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    await reg.write_spec("agent_add", FunctionSpec(**body))
    reg.pop_publish_events()  # drain caller's initial publish

    # helper bumps to v1 (then again to v2): pure version movement upstream.
    await reg.write_spec("agent_helper", FunctionSpec(
        function_name="helper", status=SpecStatus.PUBLISHED))
    await reg.write_spec("agent_helper", FunctionSpec(
        function_name="helper", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED))
    reg.pop_publish_events()

    # caller re-verifies, producing the IDENTICAL contract body but with a
    # freshly recorded support map and an advanced based_on snapshot.
    spec = FunctionSpec(**body)
    spec.spec_deps = {"helper": SpecDep(depends=False)}
    republished = await reg.write_spec("agent_add", spec)
    events = reg.pop_publish_events()

    # No public change: version frozen, NO event (so callers of `add` stay asleep).
    assert republished.version == 1
    assert events == []
    assert len(reg.spec_history("add")) == 1
    # But bookkeeping advanced: snapshot current, support visible to the judge.
    entry = reg.get("agent_add")
    assert entry.published_spec.based_on_versions == {"helper": 2}
    assert entry.published_spec.spec_deps == {"helper": SpecDep(depends=False)}
    assert reg.dependencies_current("add")


@pytest.mark.asyncio
async def test_dependency_versions_track_direct_callees(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="add",
        source_file="math.c",
        source_code="int add(int a) { return helper(a); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_add", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    caller_spec = await reg.write_spec(
        "agent_add",
        FunctionSpec(function_name="add", status=SpecStatus.PUBLISHED),
    )
    assert caller_spec.based_on_versions == {"helper": 0}
    assert not reg.dependencies_current("add")

    await reg.write_spec(
        "agent_helper",
        FunctionSpec(function_name="helper", status=SpecStatus.PUBLISHED),
    )
    assert reg.current_dependency_versions("add") == {"helper": 1}
    assert not reg.dependencies_current("add")

    refreshed = await reg.write_spec(
        "agent_add",
        FunctionSpec(function_name="add", status=SpecStatus.PUBLISHED),
    )
    assert refreshed.based_on_versions == {"helper": 1}
    assert reg.dependencies_current("add")


@pytest.mark.asyncio
async def test_write_spec_wrong_owner(registry):
    bad_spec = FunctionSpec(function_name="other_func")
    with pytest.raises(PermissionError):
        await registry.write_spec("agent_add", bad_spec)


@pytest.mark.asyncio
async def test_published_based_on_versions_is_runtime_enforced(sample_function_info_callee):
    """Runtime must overwrite agent-provided based_on_versions on publish."""
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    # helper publishes v1 first
    await reg.write_spec(
        "agent_helper",
        FunctionSpec(function_name="helper", status=SpecStatus.PUBLISHED),
    )
    assert reg.current_dependency_versions("main") == {"helper": 1}

    # main tries to publish with stale/forged based_on_versions
    forged = FunctionSpec(
        function_name="main",
        status=SpecStatus.PUBLISHED,
        based_on_versions={"helper": 99},  # ← fake, doesn't match current
    )
    written = await reg.write_spec("agent_main", forged)
    # Runtime must overwrite with authoritative current versions
    assert written.based_on_versions == {"helper": 1}


@pytest.mark.asyncio
async def test_draft_based_on_versions_preserved(sample_function_info_callee):
    """Drafts may keep agent-provided based_on_versions for local exploration."""
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    draft = FunctionSpec(
        function_name="main",
        preconditions=["requires x >= 0;"],
        status=SpecStatus.DRAFT,
        based_on_versions={"helper": 7},  # hypothetical assumption
    )
    written = await reg.write_spec("agent_main", draft)
    assert written.based_on_versions == {"helper": 7}


@pytest.mark.asyncio
async def test_raise_proof_demand_records_runtime_owned_demand(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    await reg.write_spec(
        "agent_main",
        FunctionSpec(
            function_name="main",
            preconditions=["x >= 0"],
            status=SpecStatus.DRAFT,
        ),
    )

    demand = await reg.raise_proof_demand(
        "agent_main",
        "helper",
        "postcondition",
        "ensures \\result >= 0;",
        blocking_vo_ids=["call_pre_1"],
        rationale="needed for caller proof",
    )

    assert demand.target_function == "helper"
    assert demand.blocking_vo_ids == ["call_pre_1"]
    assert demand.requester_dependency_snapshot == {"helper": 0}
    assert demand.demand_id == "demand_1"
    assert demand.status == DemandStatus.OPEN
    assert reg.pop_new_proof_demands() == [demand]
    assert reg.open_proof_demands("helper") == [demand]


@pytest.mark.asyncio
async def test_resolve_proof_demand_satisfied(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    demand = await reg.raise_proof_demand(
        "agent_main", "helper", "postcondition", "ensures \\result >= 0;",
    )
    resolved = await reg.resolve_proof_demand(
        "agent_helper", demand.demand_id, DemandStatus.SATISFIED, "proved it",
    )
    assert resolved.status == DemandStatus.SATISFIED
    assert resolved.resolution_message == "proved it"
    assert reg.open_proof_demands("helper") == []

    events = reg.pop_resolved_demands()
    assert len(events) == 1
    assert events[0].demand.demand_id == demand.demand_id
    assert events[0].resolver_function == "helper"


@pytest.mark.asyncio
async def test_resolve_proof_demand_rejected(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    demand = await reg.raise_proof_demand(
        "agent_main", "helper", "postcondition", "ensures \\result >= 0;",
    )
    resolved = await reg.resolve_proof_demand(
        "agent_helper", demand.demand_id, DemandStatus.REJECTED, "too strong",
    )
    assert resolved.status == DemandStatus.REJECTED
    assert resolved.resolution_message == "too strong"
    assert reg.open_proof_demands("helper") == []


@pytest.mark.asyncio
async def test_resolve_proof_demand_wrong_owner(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    demand = await reg.raise_proof_demand(
        "agent_main", "helper", "postcondition", "ensures \\result >= 0;",
    )
    with pytest.raises(KeyError, match="Unknown demand_id"):
        await reg.resolve_proof_demand(
            "agent_main", demand.demand_id, DemandStatus.SATISFIED,
        )


@pytest.mark.asyncio
async def test_resolve_proof_demand_unknown_id(sample_function_info_callee):
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return helper(1); }",
        callees=["helper"],
        callers=[],
    )
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=sample_function_info_callee))

    with pytest.raises(KeyError, match="Unknown demand_id"):
        await reg.resolve_proof_demand(
            "agent_helper", "demand_999", DemandStatus.SATISFIED,
        )
