"""Extern environment functions (prototype-only callees).

A function that is declared but never defined (hardware port layers,
e.g. archContextSwitch in atomthreads) is environment, not a
verification target: it gets no agent, it is not a dependency (nothing
will ever publish for it), and a callee_contracts entry naming it is a
standing environment axiom — auto-settled at publish and reported with
the result.
"""

from __future__ import annotations

import logging

import pytest

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.cli.display import _build_function_info
from verdict.core.project_context import ProjectContext
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.messaging.bus import AsyncQueueBus
from verdict.orchestrator.orchestrator import Orchestrator
from verdict.tools.context import ToolContext
from verdict.tools.runtime.proof_demand import submit_proof_demand


class _StubNode:
    def __init__(self, name, children=(), parents=(), is_definition=True):
        self.func_name = name
        self.source_code = f"void {name}(void);"
        self.children = list(children)
        self.parents = list(parents)
        self.is_definition = is_definition


def _project_ctx_with_extern() -> ProjectContext:
    arch = _StubNode("arch_init", is_definition=False)
    caller = _StubNode("caller", children=[arch])
    arch.parents = [caller]
    return ProjectContext(
        source_file="m.c",
        source_dir=".",
        call_graph_dict={"caller": caller, "arch_init": arch},
        function_declarations_map={},
        include_paths=[],
        work_dir=".",
        extern_functions={"arch_init"},
    )


def _caller_info() -> FunctionInfo:
    return FunctionInfo(
        name="caller",
        source_file="m.c",
        source_code="void caller(void) { arch_init(); helper(); }",
        callees=["helper", "arch_init"],
        callers=[],
        extern_callees=["arch_init"],
    )


def _helper_info() -> FunctionInfo:
    return FunctionInfo(
        name="helper",
        source_file="m.c",
        source_code="void helper(void) {}",
        callees=[],
        callers=["caller"],
    )


def test_build_function_info_marks_extern_callees():
    ctx = _project_ctx_with_extern()
    info = _build_function_info("caller", ctx)
    assert info.callees == ["arch_init"]
    assert info.extern_callees == ["arch_init"]


def test_functions_from_call_graph_excludes_externs():
    ctx = _project_ctx_with_extern()
    orch = Orchestrator(
        [FunctionInfo(name="caller", source_file="m.c", source_code="")],
        timeout=10,
    )
    infos = orch._functions_from_call_graph(ctx)
    assert [i.name for i in infos] == ["caller"]


@pytest.mark.asyncio
async def test_dependency_versions_skip_extern_callees():
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_caller", function_info=_caller_info()))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=_helper_info()))

    await reg.write_spec(
        "agent_helper",
        FunctionSpec(
            function_name="helper",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    versions = reg.current_dependency_versions("caller")
    assert versions == {"helper": 1}  # arch_init absent, not version 0

    # After the caller publishes on top of helper v1, its dependencies are
    # current — the extern callee does not pin an undischargeable 0.
    await reg.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\true;"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    assert reg.dependencies_current("caller")


@pytest.mark.asyncio
async def test_unmarked_agentless_callee_still_anchors():
    """Scoped-run semantics unchanged: an agent-less callee NOT marked
    extern (e.g. out-of-scope in a functions-scope run) still pins
    version 0 and blocks dependency currency."""
    reg = AgentRegistry()
    info = _caller_info()
    info.extern_callees = []
    reg.register(AgentEntry(agent_id="agent_caller", function_info=info))

    versions = reg.current_dependency_versions("caller")
    assert versions["arch_init"] == 0
    assert versions["helper"] == 0
    assert not reg.dependencies_current("caller")


@pytest.mark.asyncio
async def test_publish_born_settles_extern_assumes():
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_caller", function_info=_caller_info()))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=_helper_info()))

    spec = FunctionSpec(
        function_name="caller",
        postconditions=["ensures \\true;"],
        callee_contracts={
            "arch_init": {"postconditions": [], "assigns": ["assigns \\nothing;"]},
        },
        assumed_callees=["arch_init"],
        status=SpecStatus.PUBLISHED,
    )
    written = await reg.write_spec("agent_caller", spec)

    assert "arch_init" not in written.based_on_versions
    assert reg.settled_assumes("agent_caller") == {"arch_init"}


@pytest.mark.asyncio
async def test_publish_mixed_assumes_anchor_agents_settle_externs():
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_caller", function_info=_caller_info()))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=_helper_info()))

    await reg.write_spec(
        "agent_helper",
        FunctionSpec(
            function_name="helper",
            postconditions=["ensures \\result >= 0;"],
            status=SpecStatus.PUBLISHED,
        ),
    )

    spec = FunctionSpec(
        function_name="caller",
        postconditions=["ensures \\true;"],
        callee_contracts={
            "helper": {"postconditions": ["ensures \\result > 0;"], "assigns": []},
            "arch_init": {"postconditions": [], "assigns": ["assigns \\nothing;"]},
        },
        assumed_callees=["helper", "arch_init"],
        status=SpecStatus.PUBLISHED,
    )
    written = await reg.write_spec("agent_caller", spec)

    # Agent callee under assume: anchored at 0 (blocks GLOBAL_OK).
    assert written.based_on_versions["helper"] == 0
    # Extern callee: no anchor, born-settled instead.
    assert "arch_init" not in written.based_on_versions
    assert reg.settled_assumes("agent_caller") == {"arch_init"}


@pytest.mark.asyncio
async def test_submit_proof_demand_rejects_extern_target():
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_caller", function_info=_caller_info()))

    ctx = ToolContext(
        agent_id="agent_caller",
        function_info=_caller_info(),
        registry=reg,
        message_bus=AsyncQueueBus(),
        logger=logging.getLogger("test"),
        project_context=_project_ctx_with_extern(),
    )
    with pytest.raises(ValueError, match="extern environment function"):
        await submit_proof_demand(
            ctx,
            target_function="arch_init",
            property_kind="postcondition",
            property_text="ensures \\true;",
        )


@pytest.mark.asyncio
async def test_result_splits_extern_axioms_from_settled_assumes():
    """Caught live in the first preguss-atomthreads run: the registry's
    settled set mixes budget-settled agent debts and born-settled extern
    axioms — the split must happen at AgentResult construction so EVERY
    reporter (orchestrator logger AND cli solve summary) shows externs as
    axioms, not as 'assume budget spent'."""
    orch = Orchestrator([_caller_info(), _helper_info()], timeout=10)
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\true;"],
            callee_contracts={
                "arch_init": {
                    "postconditions": [],
                    "assigns": ["assigns \\nothing;"],
                },
            },
            assumed_callees=["arch_init"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    results = orch._finalize_results()
    by_name = {r.function_name: r for r in results}
    caller = by_name["caller"]
    assert caller.extern_axioms == ["arch_init"]
    assert caller.settled_assumes == []


def test_cli_summary_prints_axioms_line(capsys):
    from verdict.cli.solve_mode import _print_solve_summary
    from verdict.core.types import AgentResult, ProofState

    r = AgentResult(
        agent_id="agent_caller",
        function_name="caller",
        proof_state=ProofState.GLOBAL_OK,
        extern_axioms=["arch_init"],
    )
    _print_solve_summary([r], scope="global")
    out = capsys.readouterr().out
    assert "Environment axioms (extern callees, contracts assumed): " in out
    assert "caller (axioms for arch_init)" in out
    assert "Conditional OK" not in out
    assert "[env axioms: arch_init]" in out


@pytest.mark.asyncio
async def test_part2_ratchet_keeps_extern_marks():
    """The discharged-debt ratchet must not strip born-settled extern
    marks: externs never appear in the debt scan, so their absence
    means nothing."""
    orch = Orchestrator(
        [_caller_info(), _helper_info()],
        timeout=10,
    )
    await orch.registry.write_spec(
        "agent_caller",
        FunctionSpec(
            function_name="caller",
            postconditions=["ensures \\true;"],
            callee_contracts={
                "arch_init": {
                    "postconditions": [],
                    "assigns": ["assigns \\nothing;"],
                },
            },
            assumed_callees=["arch_init"],
            status=SpecStatus.PUBLISHED,
        ),
    )
    assert orch.registry.settled_assumes("agent_caller") == {"arch_init"}

    orch._reconcile_dependency_staleness()

    assert orch.registry.settled_assumes("agent_caller") == {"arch_init"}
