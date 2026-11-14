"""End-to-end test: orchestrator runs multiple agents (mocked SDK)."""

from unittest.mock import AsyncMock, patch

import pytest

from claude_agent_sdk import AssistantMessage, ResultMessage, TextBlock, ToolUseBlock

from verdict.core.types import (
    DemandStatus,
    FunctionInfo,
    FunctionSpec,
    ProofState,
    SpecStatus,
)
from verdict.orchestrator.orchestrator import Orchestrator


MODEL = "claude-sonnet-4-20250514"


def _make_mock_client_that_verifies(registry):
    """Create a mock ClaudeSDKClient class that simulates verified result.

    The mock writes a verified spec via update_my_spec tool call simulation,
    then returns a ResultMessage.
    """
    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            # Extract function name from prompt
            # Prompt format: "Begin verification of function `name`."
            import re
            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"

            # Write a verified spec for this function
            for agent_id, entry in registry._entries.items():
                if entry.function_info.name == func_name:
                    spec = FunctionSpec(function_name=func_name, status=SpecStatus.PUBLISHED)
                    await registry.write_spec(agent_id, spec)
                    break

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="Verification complete.")],
                model=MODEL,
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.01,
                duration_ms=100,
                duration_api_ms=80,
                num_turns=1,
                is_error=False,
                session_id="test-session",
            )

        async def disconnect(self):
            pass

    return MockClientFactory


def _make_mock_client_with_auto_wake(registry):
    """Create a mock client that requires caller reactivation after callee publish."""

    query_log: list[tuple[str, str, str]] = []

    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            import re

            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"
            query_log.append((func_name, prompt, session_id))

            if func_name == "callee":
                await registry.write_spec(
                    "agent_callee",
                    FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
                )
                return

            if func_name == "main" and "Reactivated" in prompt:
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
                )
                return

            await registry.write_spec(
                "agent_main",
                FunctionSpec(
                    function_name="main",
                    preconditions=["x >= 0"],
                    status=SpecStatus.DRAFT,
                ),
            )

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="Verification cycle complete.")],
                model=MODEL,
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.01,
                duration_ms=100,
                duration_api_ms=80,
                num_turns=1,
                is_error=False,
                session_id="test-session",
            )

        async def disconnect(self):
            pass

    return MockClientFactory, query_log


def _make_mock_client_with_stale_caller(registry):
    """Caller publishes first, then the callee publishes a contract that
    invalidates the caller's VOs (assigns change). The reactivated caller
    does NOT re-verify/republish, so its spec_vo stays PENDING — it must
    not finalize to GLOBAL_OK."""

    query_log: list[tuple[str, str, str]] = []

    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            import re

            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"
            query_log.append((func_name, prompt, session_id))

            if func_name == "main" and "Reactivated" not in prompt:
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(
                        function_name="main",
                        postconditions=["ensures \\true;"],
                        status=SpecStatus.PUBLISHED,
                    ),
                )
                return

            if func_name == "callee":
                # Publishing an assigns clause invalidates main's VOs
                # (assigns_changed → spec_vo PENDING on the caller).
                await registry.write_spec(
                    "agent_callee",
                    FunctionSpec(
                        function_name="callee",
                        postconditions=["ensures \\true;"],
                        assigns=["assigns \\nothing;"],
                        status=SpecStatus.PUBLISHED,
                    ),
                )
                return

            # Reactivated main intentionally does not refresh its published spec.

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="Verification cycle complete.")],
                model=MODEL,
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.01,
                duration_ms=100,
                duration_api_ms=80,
                num_turns=1,
                is_error=False,
                session_id="test-session",
            )

        async def disconnect(self):
            pass

    return MockClientFactory, query_log


def _make_mock_client_with_proof_demand(registry):
    """Create a mock client where a caller raises a proof demand for its callee."""

    query_log: list[tuple[str, str, str]] = []

    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            import re

            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"
            query_log.append((func_name, prompt, session_id))

            if func_name == "main" and "Reactivated" not in prompt:
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(
                        function_name="main",
                        preconditions=["x >= 0"],
                        status=SpecStatus.DRAFT,
                    ),
                )
                await registry.raise_proof_demand(
                    "agent_main",
                    "callee",
                    "postcondition",
                    "ensures \\result >= 0;",
                    blocking_vo_ids=["call_pre_1"],
                    rationale="needed by caller",
                )
                return

            if func_name == "main" and "Reactivated" in prompt:
                # Demand satisfied → main can now publish.
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
                )
                return

            if func_name == "callee" and "proof_demand" in prompt:
                demands = registry.open_proof_demands("callee")
                if demands:
                    await registry.resolve_proof_demand(
                        "agent_callee",
                        demands[0].demand_id,
                        DemandStatus.SATISFIED,
                        "postcondition proved",
                    )
                await registry.write_spec(
                    "agent_callee",
                    FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
                )

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="Verification cycle complete.")],
                model=MODEL,
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.01,
                duration_ms=100,
                duration_api_ms=80,
                num_turns=1,
                is_error=False,
                session_id="test-session",
            )

        async def disconnect(self):
            pass

    return MockClientFactory, query_log


def _make_mock_client_with_demand_resolution(registry):
    """Mock client where callee resolves the demand, requester gets woken."""

    query_log: list[tuple[str, str, str]] = []

    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            import re

            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"
            query_log.append((func_name, prompt, session_id))

            if func_name == "main" and "Reactivated" not in prompt:
                # Initial solve: submit a proof demand
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(
                        function_name="main",
                        preconditions=["x >= 0"],
                        status=SpecStatus.DRAFT,
                    ),
                )
                await registry.raise_proof_demand(
                    "agent_main",
                    "callee",
                    "postcondition",
                    "ensures \\result >= 0;",
                    blocking_vo_ids=["call_pre_1"],
                    rationale="needed by caller",
                )
                return

            if func_name == "callee" and "proof_demand" in prompt:
                # Callee wakes for demand, resolves it satisfied
                demands = registry.open_proof_demands("callee")
                if demands:
                    await registry.resolve_proof_demand(
                        "agent_callee",
                        demands[0].demand_id,
                        DemandStatus.SATISFIED,
                        "postcondition proved",
                    )
                await registry.write_spec(
                    "agent_callee",
                    FunctionSpec(function_name="callee", status=SpecStatus.PUBLISHED),
                )
                return

            if func_name == "main" and "demand_satisfied" in prompt:
                # Requester wakes for resolution
                await registry.write_spec(
                    "agent_main",
                    FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
                )
                return

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="Verification cycle complete.")],
                model=MODEL,
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.01,
                duration_ms=100,
                duration_api_ms=80,
                num_turns=1,
                is_error=False,
                session_id="test-session",
            )

        async def disconnect(self):
            pass

    return MockClientFactory, query_log


@pytest.mark.asyncio
async def test_single_agent_lifecycle():
    func = FunctionInfo(
        name="inc",
        source_file="test.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10)

    MockFactory = _make_mock_client_that_verifies(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    assert len(results) == 1
    assert results[0].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}
    assert results[0].function_name == "inc"


@pytest.mark.asyncio
async def test_multi_agent_parallel():
    funcs = [
        FunctionInfo(
            name="add",
            source_file="math.c",
            source_code="int add(int a, int b) { return a + b; }",
            callees=[],
            callers=["main"],
        ),
        FunctionInfo(
            name="mul",
            source_file="math.c",
            source_code="int mul(int a, int b) { return a * b; }",
            callees=[],
            callers=["main"],
        ),
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return add(1,2) + mul(3,4); }",
            callees=["add", "mul"],
            callers=[],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10)

    MockFactory = _make_mock_client_that_verifies(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    assert len(results) == 3
    for r in results:
        assert r.proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}


@pytest.mark.asyncio
async def test_auto_wake_rechecks_dirty_caller():
    funcs = [
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10)

    MockFactory, query_log = _make_mock_client_with_auto_wake(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {result.function_name: result for result in results}
    assert result_by_name["callee"].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}
    assert result_by_name["main"].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}

    main_queries = [entry for entry in query_log if entry[0] == "main"]
    assert len(main_queries) == 2
    assert "Reactivated" in main_queries[1][1]


@pytest.mark.asyncio
async def test_stale_dependency_does_not_finalize_global_ok():
    funcs = [
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, max_parallel=1)

    MockFactory, query_log = _make_mock_client_with_stale_caller(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {result.function_name: result for result in results}
    assert result_by_name["callee"].proof_state == ProofState.GLOBAL_OK
    assert result_by_name["main"].proof_state != ProofState.GLOBAL_OK

    main_queries = [entry for entry in query_log if entry[0] == "main"]
    assert len(main_queries) >= 2


@pytest.mark.asyncio
async def test_proof_demand_wakes_target_owner():
    funcs = [
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, max_parallel=1)

    MockFactory, query_log = _make_mock_client_with_proof_demand(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {result.function_name: result for result in results}
    assert result_by_name["callee"].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}

    # The target owner saw the demand and resolved it (durable-demand wake).
    callee_queries = [entry for entry in query_log if entry[0] == "callee"]
    assert any("proof_demand" in q[1] for q in callee_queries)
    assert not orch.registry.open_proof_demands("callee")


@pytest.mark.asyncio
async def test_demand_resolution_wakes_requester():
    funcs = [
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, max_parallel=1)

    MockFactory, query_log = _make_mock_client_with_demand_resolution(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {result.function_name: result for result in results}
    assert result_by_name["main"].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}

    main_queries = [entry for entry in query_log if entry[0] == "main"]
    # main: initial solve, then reactivated for demand_satisfied
    assert len(main_queries) >= 2
    satisfied_query = [q for q in main_queries if "demand_satisfied" in q[1]]
    assert len(satisfied_query) >= 1


@pytest.mark.asyncio
async def test_dirty_callee_blocks_caller_global_ok():
    """A → B → C. C changes, B stays dirty. A must not be GLOBAL_OK."""

    query_log: list[tuple[str, str, str]] = []

    def _make_mock(registry):
        class MockClientFactory:
            def __init__(self, options=None, transport=None):
                self._options = options

            async def connect(self):
                pass

            async def query(self, prompt, session_id="default"):
                import re

                m = re.search(r"`(\w+)`", prompt)
                func_name = m.group(1) if m else "unknown"
                query_log.append((func_name, prompt, session_id))

                if func_name == "A" and "Reactivated" not in prompt:
                    await registry.write_spec(
                        "agent_A",
                        FunctionSpec(function_name="A", status=SpecStatus.PUBLISHED),
                    )
                    return

                if func_name == "B" and "Reactivated" not in prompt:
                    await registry.write_spec(
                        "agent_B",
                        FunctionSpec(function_name="B", status=SpecStatus.PUBLISHED),
                    )
                    return

                if func_name == "C":
                    await registry.write_spec(
                        "agent_C",
                        FunctionSpec(function_name="C", status=SpecStatus.PUBLISHED),
                    )
                    return

                # Reactivated B: intentionally does NOT republish (stays dirty)

            async def receive_response(self):
                yield AssistantMessage(
                    content=[TextBlock(text="done.")],
                    model=MODEL,
                )
                yield ResultMessage(
                    subtype="success",
                    total_cost_usd=0.01,
                    duration_ms=100,
                    duration_api_ms=80,
                    num_turns=1,
                    is_error=False,
                    session_id="test-session",
                )

            async def disconnect(self):
                pass

        return MockClientFactory

    funcs = [
        FunctionInfo(
            name="A",
            source_file="a.c",
            source_code="int A() { return B(); }",
            callees=["B"],
            callers=[],
        ),
        FunctionInfo(
            name="B",
            source_file="b.c",
            source_code="int B() { return C(); }",
            callees=["C"],
            callers=["A"],
        ),
        FunctionInfo(
            name="C",
            source_file="c.c",
            source_code="int C() { return 1; }",
            callees=[],
            callers=["B"],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, max_parallel=1)

    MockFactory = _make_mock(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {r.function_name: r for r in results}
    assert result_by_name["C"].proof_state == ProofState.GLOBAL_OK
    # B was marked dirty by C's publication and did not republish
    assert result_by_name["B"].proof_state == ProofState.DIRTY
    # A must NOT be GLOBAL_OK because its callee B is dirty
    assert result_by_name["A"].proof_state == ProofState.DIRTY


@pytest.mark.asyncio
async def test_cycle_limit_prevents_infinite_oscillation():
    """Mutual recursion where both sides keep republishing must stop at cycle limit."""

    cycle_log: dict[str, int] = {"A": 0, "B": 0}

    def _make_mock(registry):
        class MockClientFactory:
            def __init__(self, options=None, transport=None):
                self._options = options

            async def connect(self):
                pass

            async def query(self, prompt, session_id="default"):
                import re

                m = re.search(r"`(\w+)`", prompt)
                func_name = m.group(1) if m else "unknown"
                cycle_log[func_name] = cycle_log.get(func_name, 0) + 1
                n = cycle_log[func_name]
                # Each cycle publishes a DIFFERENT postcondition → version always bumps
                await registry.write_spec(
                    f"agent_{func_name}",
                    FunctionSpec(
                        function_name=func_name,
                        postconditions=[f"ensures \\result >= {n};"],
                        status=SpecStatus.PUBLISHED,
                    ),
                )

            async def receive_response(self):
                yield AssistantMessage(
                    content=[TextBlock(text="done.")],
                    model=MODEL,
                )
                yield ResultMessage(
                    subtype="success",
                    total_cost_usd=0.01,
                    duration_ms=100,
                    duration_api_ms=80,
                    num_turns=1,
                    is_error=False,
                    session_id="test-session",
                )

            async def disconnect(self):
                pass

        return MockClientFactory

    funcs = [
        FunctionInfo(
            name="A",
            source_file="a.c",
            source_code="int A() { return B(); }",
            callees=["B"],
            callers=["B"],
        ),
        FunctionInfo(
            name="B",
            source_file="b.c",
            source_code="int B() { return A(); }",
            callees=["A"],
            callers=["A"],
        ),
    ]
    orch = Orchestrator(
        funcs,
        timeout=10,
        max_parallel=1,
        max_cycles_per_agent=3,
    )

    MockFactory = _make_mock(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {r.function_name: r for r in results}
    # Neither agent should exceed the cycle limit
    assert result_by_name["A"].cycle_count <= 3
    assert result_by_name["B"].cycle_count <= 3
    # At least one should have hit the limit (the oscillation was detected)
    assert (
        result_by_name["A"].cycle_limit_reached
        or result_by_name["B"].cycle_limit_reached
    )
    # The loop must have terminated without timeout
    assert cycle_log["A"] <= 3
    assert cycle_log["B"] <= 3


@pytest.mark.asyncio
async def test_cycle_count_reflects_initial_and_reactivations():
    """Initial solve + each reactivation contribute to cycle_count."""
    funcs = [
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10)

    MockFactory, _query_log = _make_mock_client_with_auto_wake(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    result_by_name = {r.function_name: r for r in results}
    # main: initial solve + at least one reactivation
    assert result_by_name["main"].cycle_count >= 2
    # callee: initial solve only (no caller wakes it)
    assert result_by_name["callee"].cycle_count >= 1
    # Neither should hit the default limit
    assert not result_by_name["main"].cycle_limit_reached
    assert not result_by_name["callee"].cycle_limit_reached


@pytest.mark.asyncio
async def test_vo_failed_wakes_agent_to_retry():
    """Agent is LOCAL_OK but has a FAILED VO → orchestrator wakes for retry.

    The first cycle installs a FAILED RTE VO alongside a verified contract.
    The vo_failed channel must reactivate the agent with the failed VO ids
    in the wake reason, and the agent can discharge it (e.g. mark as BUG)
    so the run reaches GLOBAL_OK.
    """
    from verdict.core.vo_tracker import VOEntry

    reactivation_prompts: list[str] = []

    def _make_mock(registry, agents_by_id):
        class MockClientFactory:
            def __init__(self, options=None, transport=None):
                self._options = options

            async def connect(self):
                pass

            async def query(self, prompt, session_id="default"):
                import re

                m = re.search(r"`(\w+)`", prompt)
                func_name = m.group(1) if m else "unknown"
                agent_id = f"agent_{func_name}"
                tracker = agents_by_id[agent_id].vo_tracker

                if "Reactivated" not in prompt:
                    # Initial: install a FAILED RTE VO, publish verified spec.
                    tracker.add_rte_vo(
                        VOEntry(
                            vo_id="rte_1",
                            vo_type="rte",
                            description="test rte",
                        )
                    )
                    tracker.mark_failed("rte_1")
                    await registry.write_spec(
                        agent_id,
                        FunctionSpec(
                            function_name=func_name,
                            postconditions=["ensures \\result >= 0;"],
                            status=SpecStatus.PUBLISHED,
                        ),
                    )
                    return

                reactivation_prompts.append(prompt)
                # Agent decides the failed VO is a known limitation.
                tracker.mark_bug("rte_1", "accepted known limitation")

            async def receive_response(self):
                yield AssistantMessage(
                    content=[TextBlock(text="done.")],
                    model=MODEL,
                )
                yield ResultMessage(
                    subtype="success",
                    total_cost_usd=0.01,
                    duration_ms=100,
                    duration_api_ms=80,
                    num_turns=1,
                    is_error=False,
                    session_id="test-session",
                )

            async def disconnect(self):
                pass

        return MockClientFactory

    func = FunctionInfo(
        name="inc",
        source_file="inc.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10)

    MockFactory = _make_mock(orch.registry, orch._agents_by_id)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    # The agent was reactivated (state-driven: DIRTY with FAILED VO)
    assert reactivation_prompts, "agent should have been reactivated"

    # After marking bug, the tracker considers itself complete.
    tracker = orch._agents_by_id["agent_inc"].vo_tracker
    assert tracker.is_vo_complete()

    result = results[0]
    summary = result.vo_summary
    assert summary is not None
    assert summary.rte_bug == 1
    assert summary.rte_failed == 0
    assert "rte_1" in summary.bugs


@pytest.mark.asyncio
async def test_on_batch_complete_callback_invoked_with_snapshots():
    """Orchestrator must invoke on_batch_complete for initial + reactivation."""
    events: list[tuple[str, list[str]]] = []

    def _capture(phase, snapshots):
        events.append(
            (phase, [s.function_name for s in snapshots])
        )
        # Verify snapshot has cycle_count and vo_summary populated
        for s in snapshots:
            assert s.cycle_count >= 1
            assert s.vo_summary is not None

    funcs = [
        FunctionInfo(
            name="callee",
            source_file="math.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
        FunctionInfo(
            name="main",
            source_file="main.c",
            source_code="int main() { return callee(1); }",
            callees=["callee"],
            callers=[],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, on_batch_complete=_capture)

    MockFactory = _make_mock_client_that_verifies(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        await orch.run()

    # Initial batch should have fired at least once
    initial_events = [e for e in events if e[0] == "initial"]
    assert len(initial_events) >= 1


@pytest.mark.asyncio
async def test_function_scope_does_not_promote_global_ok():
    func = FunctionInfo(
        name="inc",
        source_file="test.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10, scope_kind="functions")

    MockFactory = _make_mock_client_that_verifies(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    assert len(results) == 1
    assert results[0].proof_state in {ProofState.LOCAL_OK, ProofState.GLOBAL_OK}
    assert results[0].proof_state == ProofState.LOCAL_OK
