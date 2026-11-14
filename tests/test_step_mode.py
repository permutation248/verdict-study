"""Tests for the interactive step-through orchestrator (wake_selector hook)."""

from __future__ import annotations

import asyncio

import pytest

from verdict.core.types import FunctionInfo, FunctionSpec, WakeReason
from verdict.orchestrator.orchestrator import Orchestrator, WakeSelection


def _two_functions() -> list[FunctionInfo]:
    return [
        FunctionInfo(
            name="callee",
            source_file="test.c",
            source_code="int callee(int x) { return x + 1; }",
            callees=[],
            callers=["caller"],
        ),
        FunctionInfo(
            name="caller",
            source_file="test.c",
            source_code="int caller(int x) { return callee(x); }",
            callees=["callee"],
            callers=[],
        ),
    ]


# ---------------------------------------------------------------------------
# wake_selector=None preserves auto behavior
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_auto_mode_unchanged_with_none_selector():
    """When wake_selector is None, run() behaves like normal solve."""
    orch = Orchestrator(_two_functions(), timeout=10)
    # Without project_context, agents use mock path and finish in 1 cycle.
    results = await orch.run(wake_selector=None)
    assert len(results) == 2


# ---------------------------------------------------------------------------
# wake_selector can selectively wake agents
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_selective_wake():
    """wake_selector picks only one agent per round."""
    orch = Orchestrator(_two_functions(), timeout=10)
    call_count = 0

    async def selector(ready: dict) -> WakeSelection:
        nonlocal call_count
        call_count += 1
        if not ready:
            return None  # quit when nothing pending
        # Pick the first pending agent only
        first_id = next(iter(ready))
        return [first_id], {}

    results = await orch.run(wake_selector=selector)
    assert len(results) == 2


# ---------------------------------------------------------------------------
# wake_selector can inject user messages
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_user_message_injection():
    """User messages are appended to the agent's cycle prompt."""
    orch = Orchestrator(_two_functions(), timeout=10)

    # Seed a fake wake reason so there's something to select
    orch._wakes.pending()["agent_callee"] = [
        WakeReason(reason="test", details="manual test")
    ]

    captured_prompts: list[str] = []
    original_execute = type(orch._agents[0])._execute_cycle

    async def _capture_execute(self, wake_reasons, user_message=None):
        if user_message:
            captured_prompts.append(user_message)
        # Skip actual LLM call — just return a snapshot
        return self._snapshot_result(wake_reasons)

    # Monkey-patch _execute_cycle on all agents
    for agent in orch._agents:
        agent._execute_cycle = _capture_execute.__get__(agent)

    async def selector(ready: dict) -> WakeSelection:
        if not ready:
            return None
        return list(ready.keys()), {"agent_callee": "check the RTE"}

    results = await orch.run(wake_selector=selector)
    assert "check the RTE" in captured_prompts


# ---------------------------------------------------------------------------
# wake_selector returning None quits the loop
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_selector_none_quits():
    """Returning None from wake_selector exits the run loop."""
    orch = Orchestrator(_two_functions(), timeout=10)

    # Seed wakes so the loop has work
    orch._wakes.pending()["agent_callee"] = [
        WakeReason(reason="test", details="manual")
    ]

    async def selector(ready: dict) -> WakeSelection:
        return None  # quit immediately

    results = await orch.run(wake_selector=selector)
    assert len(results) == 2  # finalize still produces results


# ---------------------------------------------------------------------------
# Deferred agents persist across rounds
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_deferred_agents_persist():
    """Agents not selected stay in _ready for the next round."""
    orch = Orchestrator(_two_functions(), timeout=10)

    round_num = 0

    # Seed wakes for both
    orch._wakes.pending()["agent_callee"] = [
        WakeReason(reason="test_callee", details="c")
    ]
    orch._wakes.pending()["agent_caller"] = [
        WakeReason(reason="test_caller", details="r")
    ]

    # Monkey-patch agents to skip LLM
    for agent in orch._agents:
        async def _noop(self, wr, user_message=None):
            return self._snapshot_result(wr)
        agent._execute_cycle = _noop.__get__(agent)

    async def selector(ready: dict) -> WakeSelection:
        nonlocal round_num
        round_num += 1
        if round_num == 1:
            # Only wake callee in round 1
            assert "agent_callee" in ready
            assert "agent_caller" in ready
            return ["agent_callee"], {}
        elif round_num == 2:
            # caller should still be pending from round 1
            assert "agent_caller" in ready
            return ["agent_caller"], {}
        else:
            return None

    results = await orch.run(wake_selector=selector)
    assert round_num >= 2
