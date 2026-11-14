"""Tests for Phase B: SpecDep data model + advance_dependency_snapshot."""

import dataclasses

import pytest

from verdict.core.spec_dep import SpecDep
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.spec_diff import _published_equivalent


# ── SpecDep data model ──


def test_spec_deps_constructs_three_states():
    none = SpecDep(depends=False)
    assert none.depends is False
    assert none.clauses is None

    coarse = SpecDep(depends=True)
    assert coarse.depends is True
    assert coarse.clauses is None

    fine = SpecDep(depends=True, clauses=frozenset({"ensures \\result >= 0;"}))
    assert fine.depends is True
    assert fine.clauses == frozenset({"ensures \\result >= 0;"})


def test_spec_deps_is_frozen():
    cs = SpecDep(depends=False)
    with pytest.raises(dataclasses.FrozenInstanceError):
        cs.depends = True  # type: ignore[misc]


def test_function_spec_default_spec_deps_is_empty():
    spec = FunctionSpec(function_name="f")
    assert spec.spec_deps == {}


# ── _published_equivalent: contract-body only ──


def test_published_equivalent_ignores_support_difference():
    """spec_deps is private bookkeeping, NOT the public contract: a
    support-only difference must be treated as EQUIVALENT, so a no-op re-verify
    is recognised as a fixpoint and does not bump the version / cascade. (The
    advanced support is persisted separately in write_spec's dedup branch.)"""
    base_kwargs = dict(
        function_name="f",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    left = FunctionSpec(
        **base_kwargs,
        spec_deps={"helper": SpecDep(depends=False)},
    )
    right = FunctionSpec(
        **base_kwargs,
        spec_deps={"helper": SpecDep(depends=True)},
    )
    assert _published_equivalent(left, right) is True


def test_published_equivalent_ignores_based_on_difference():
    """based_on_versions is also bookkeeping — a snapshot-only difference is
    equivalent (this was the version-churn root cause)."""
    base_kwargs = dict(
        function_name="f",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    left = FunctionSpec(**base_kwargs, based_on_versions={"helper": 1})
    right = FunctionSpec(**base_kwargs, based_on_versions={"helper": 2})
    assert _published_equivalent(left, right) is True


def test_published_equivalent_false_when_contract_body_differs():
    """A genuine contract change (postcondition added) is NOT equivalent."""
    left = FunctionSpec(
        function_name="f", postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    )
    right = FunctionSpec(
        function_name="f",
        postconditions=["ensures \\result >= 0;", "ensures \\result < 100;"],
        status=SpecStatus.PUBLISHED,
    )
    assert _published_equivalent(left, right) is False


# ── advance_dependency_snapshot ──


class _SpyRunStore:
    """Minimal run-store stand-in capturing append_event calls."""

    def __init__(self) -> None:
        self.events: list[tuple[str, dict, str | None]] = []

    def append_event(self, type, data=None, agent_id=None):  # noqa: A002
        self.events.append((type, data or {}, agent_id))


def _two_callee_registry(run_store=None) -> AgentRegistry:
    reg = AgentRegistry(run_store=run_store)
    caller = FunctionInfo(
        name="main",
        source_file="main.c",
        source_code="int main(void) { return a() + b(); }",
        callees=["a", "b"],
        callers=[],
    )
    callee_a = FunctionInfo(name="a", source_file="m.c", source_code="int a(void){return 1;}", callees=[], callers=["main"])
    callee_b = FunctionInfo(name="b", source_file="m.c", source_code="int b(void){return 2;}", callees=[], callers=["main"])
    reg.register(AgentEntry(agent_id="agent_main", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_a", function_info=callee_a))
    reg.register(AgentEntry(agent_id="agent_b", function_info=callee_b))
    return reg


@pytest.mark.asyncio
async def test_advance_sets_only_named_callee_and_emits_event():
    spy = _SpyRunStore()
    reg = _two_callee_registry(run_store=spy)

    # Publish both callees so main can record a real based_on snapshot.
    await reg.write_spec("agent_a", FunctionSpec(function_name="a", status=SpecStatus.PUBLISHED))
    await reg.write_spec("agent_b", FunctionSpec(function_name="b", status=SpecStatus.PUBLISHED))
    main_spec = await reg.write_spec(
        "agent_main",
        FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
    )
    assert main_spec.based_on_versions == {"a": 1, "b": 1}

    entry = reg.get("agent_main")
    pre_version = entry.published_spec.version
    pre_state = entry.proof_state
    spy.events.clear()

    # 'a' advances to v2; surgically advance only main's snapshot for 'a'.
    reg.advance_dependency_snapshot("main", "a", 2)

    # Only the named callee's entry moved; 'b' untouched.
    assert entry.published_spec.based_on_versions == {"a": 2, "b": 1}
    assert entry.spec.based_on_versions == {"a": 2, "b": 1}
    # No version bump, no proof_state change.
    assert entry.published_spec.version == pre_version
    assert entry.proof_state == pre_state
    # Lightweight event emitted.
    assert ("dependency_advanced", {"function_name": "main", "callee": "a", "new_version": 2}, "agent_main") in spy.events
    # No spec_published emitted by the advance.
    assert not any(t == "spec_published" for (t, _d, _a) in spy.events)


def test_advance_rejects_version_zero():
    reg = _two_callee_registry()
    with pytest.raises(ValueError):
        reg.advance_dependency_snapshot("main", "a", 0)


def test_advance_unknown_function_raises():
    reg = _two_callee_registry()
    with pytest.raises(KeyError):
        reg.advance_dependency_snapshot("nonexistent", "a", 1)
