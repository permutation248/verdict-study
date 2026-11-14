"""Integration tests for FunctionAgent + Claude SDK (mocked transport)."""

import asyncio
from unittest.mock import AsyncMock, MagicMock, patch

import pytest

from claude_agent_sdk import (
    AssistantMessage,
    ResultMessage,
    TextBlock,
    ToolUseBlock,
)

from verdict.core.types import AgentResult, FunctionInfo, FunctionSpec, ProofState, SpecStatus
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.agent.runner import FunctionAgent
from verdict.messaging.bus import AsyncQueueBus


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

MODEL = "claude-sonnet-4-20250514"


def _assistant_msg(*blocks):
    """Create an AssistantMessage with given content blocks."""
    return AssistantMessage(content=list(blocks), model=MODEL)


def _result_msg():
    """Create a successful ResultMessage."""
    return ResultMessage(
        subtype="success",
        total_cost_usd=0.01,
        duration_ms=1000,
        duration_api_ms=800,
        num_turns=3,
        is_error=False,
        session_id="test-session",
    )


def _mock_client(messages):
    """Patch ClaudeSDKClient to yield the given messages."""

    async def mock_receive_response():
        for msg in messages:
            yield msg

    patcher = patch("verdict.agent.runner.ClaudeSDKClient")
    MockClient = patcher.start()
    instance = MockClient.return_value
    instance.connect = AsyncMock()
    instance.query = AsyncMock()
    instance.receive_response = mock_receive_response
    instance.disconnect = AsyncMock()
    return patcher, instance


# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------


@pytest.fixture
def registry():
    reg = AgentRegistry()
    info = FunctionInfo(
        name="inc",
        source_file="test.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    reg.register(AgentEntry(agent_id="agent_inc", function_info=info))
    return reg


@pytest.fixture
def bus():
    return AsyncQueueBus()


@pytest.fixture
def func_info():
    return FunctionInfo(
        name="inc",
        source_file="test.c",
        source_code="int inc(int x) { return x + 1; }",
    )


# ---------------------------------------------------------------------------
# FunctionAgent construction tests
# ---------------------------------------------------------------------------


class TestFunctionAgentInit:
    def test_default_model_and_turns(self, func_info, registry, bus):
        agent = FunctionAgent("agent_inc", func_info, registry, bus)
        assert agent._model is None
        assert agent._max_turns == 50

    def test_custom_model_and_turns(self, func_info, registry, bus):
        agent = FunctionAgent(
            "agent_inc", func_info, registry, bus,
            model="claude-sonnet-4-20250514",
            max_turns=10,
        )
        assert agent._model == "claude-sonnet-4-20250514"
        assert agent._max_turns == 10

    def test_mcp_server_created(self, func_info, registry, bus):
        agent = FunctionAgent("agent_inc", func_info, registry, bus)
        assert isinstance(agent._mcp_server, dict)
        assert agent._mcp_server["type"] == "sdk"


# ---------------------------------------------------------------------------
# FunctionAgent.run() with mocked SDK client
# ---------------------------------------------------------------------------


class TestFunctionAgentRun:
    @pytest.mark.asyncio
    async def test_clean_cycle_reports_spec_without_promoting(self, func_info, registry, bus):
        """A clean cycle reports the working spec and raises no error, but does
        NOT promote to LOCAL_OK: promotion is derived solely by the orchestrator
        (_recompute_proof_state), so the runner leaves a published spec at its
        spec-content bootstrap (DRAFT). LOCAL_OK promotion is covered at the
        orchestrator level (test_e2e / test_vo_invalidation)."""
        agent = FunctionAgent("agent_inc", func_info, registry, bus)

        spec = FunctionSpec(
            function_name="inc",
            preconditions=["x >= 0"],
            postconditions=["\\result == x + 1"],
            status=SpecStatus.PUBLISHED,
        )
        await registry.write_spec("agent_inc", spec)

        messages = [
            _assistant_msg(ToolUseBlock(id="tu_1", name="get_function_context", input={})),
            _assistant_msg(ToolUseBlock(id="tu_2", name="verify_function_contract", input={})),
            _assistant_msg(TextBlock(text="Verification complete.")),
            _result_msg(),
        ]

        patcher, instance = _mock_client(messages)
        try:
            result = await agent.run()
        finally:
            patcher.stop()

        assert result.proof_state == ProofState.DRAFT
        assert result.spec is not None
        assert result.error is None
        instance.disconnect.assert_awaited_once()

    @pytest.mark.asyncio
    async def test_run_failed_no_spec(self, func_info, registry, bus):
        """Agent run where no verified spec is produced."""
        agent = FunctionAgent("agent_inc", func_info, registry, bus)

        messages = [
            _assistant_msg(TextBlock(text="I could not verify this function.")),
            _result_msg(),
        ]

        patcher, instance = _mock_client(messages)
        try:
            result = await agent.run()
        finally:
            patcher.stop()

        assert result.proof_state == ProofState.EMPTY
        assert result.error is not None

    @pytest.mark.asyncio
    async def test_run_sdk_exception(self, func_info, registry, bus):
        """Agent run where SDK throws an exception."""
        agent = FunctionAgent("agent_inc", func_info, registry, bus)

        with patch("verdict.agent.runner.ClaudeSDKClient") as MockClient:
            instance = MockClient.return_value
            instance.connect = AsyncMock(side_effect=RuntimeError("SDK connection failed"))
            instance.disconnect = AsyncMock()

            result = await agent.run()

        assert result.proof_state == ProofState.FAILED
        assert "SDK connection failed" in result.error

    @pytest.mark.asyncio
    async def test_clean_cycle_clears_prior_failed(self, func_info, registry, bus):
        """The runner owns the FAILED execution flag: a clean cycle clears a
        prior FAILED back to the spec-content bootstrap so the orchestrator can
        re-derive promotion. (The orchestrator never clears FAILED itself.)"""
        agent = FunctionAgent("agent_inc", func_info, registry, bus)

        spec = FunctionSpec(
            function_name="inc",
            preconditions=["x >= 0"],
            postconditions=["\\result == x + 1"],
            status=SpecStatus.PUBLISHED,
        )
        await registry.write_spec("agent_inc", spec)
        registry.set_proof_state("agent_inc", ProofState.FAILED)  # prior cycle threw

        messages = [
            _assistant_msg(ToolUseBlock(id="tu_1", name="get_function_context", input={})),
            _assistant_msg(ToolUseBlock(id="tu_2", name="check_acsl_syntax", input={})),
            _assistant_msg(ToolUseBlock(id="tu_3", name="verify_function_contract", input={})),
            _result_msg(),
        ]

        patcher, _ = _mock_client(messages)
        try:
            result = await agent.run()
        finally:
            patcher.stop()

        # FAILED cleared → spec-content bootstrap (DRAFT); not stuck FAILED,
        # and not self-promoted (that is the orchestrator's job).
        assert result.proof_state == ProofState.DRAFT

    @pytest.mark.asyncio
    async def test_published_unverified_spec_is_not_hard_failed(self, func_info, registry, bus):
        agent = FunctionAgent("agent_inc", func_info, registry, bus)

        await registry.write_spec(
            "agent_inc",
            FunctionSpec(
                function_name="inc",
                preconditions=["x >= 0"],
                status=SpecStatus.PUBLISHED,
            ),
        )

        patcher, _ = _mock_client([_result_msg()])
        try:
            result = await agent.run()
        finally:
            patcher.stop()

        assert result.proof_state != ProofState.FAILED
        assert result.error is None
