"""Unit tests for factory.py: _adapt_handler, TOOL_SCHEMAS, create_agent_mcp_server."""

import asyncio
import json
from dataclasses import dataclass

import pytest

from verdict.core.types import FunctionInfo, FunctionSpec
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.tools.factory import (
    _adapt_handler,
    _deserialize_function_spec,
    TOOL_SCHEMAS,
    TOOL_DESCRIPTIONS,
    create_agent_mcp_server,
)

import logging


# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------


@pytest.fixture
def tool_ctx():
    """ToolContext with no project_context (mock fallback)."""
    info = FunctionInfo(name="foo", source_file="test.c", source_code="int foo(){return 1;}")
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_foo", function_info=info))
    bus = AsyncQueueBus()
    return ToolContext(
        agent_id="agent_foo",
        function_info=info,
        registry=reg,
        message_bus=bus,
        logger=logging.getLogger("test"),
    )


# ---------------------------------------------------------------------------
# _adapt_handler tests
# ---------------------------------------------------------------------------


class TestAdaptHandler:
    """Tests for the _adapt_handler bridge."""

    @pytest.mark.asyncio
    async def test_sync_handler(self):
        """Sync handler returning a dict is serialized to JSON."""
        def handler(x: int, y: int) -> dict:
            return {"sum": x + y}

        adapted = _adapt_handler(handler)
        result = await adapted({"x": 1, "y": 2})
        assert result == {"content": [{"type": "text", "text": '{"sum": 3}'}]}

    @pytest.mark.asyncio
    async def test_async_handler(self):
        """Async handler is properly awaited."""
        async def handler(name: str) -> str:
            return f"hello {name}"

        adapted = _adapt_handler(handler)
        result = await adapted({"name": "world"})
        parsed = json.loads(result["content"][0]["text"])
        assert parsed == "hello world"

    @pytest.mark.asyncio
    async def test_dataclass_return(self):
        """Dataclass return values are serialized via asdict."""
        @dataclass
        class Point:
            x: int
            y: int

        def handler() -> Point:
            return Point(x=1, y=2)

        adapted = _adapt_handler(handler)
        result = await adapted({})
        parsed = json.loads(result["content"][0]["text"])
        assert parsed == {"x": 1, "y": 2}

    @pytest.mark.asyncio
    async def test_list_of_dataclasses(self):
        """List of dataclasses are serialized element-wise."""
        @dataclass
        class Item:
            name: str

        def handler() -> list:
            return [Item(name="a"), Item(name="b")]

        adapted = _adapt_handler(handler)
        result = await adapted({})
        parsed = json.loads(result["content"][0]["text"])
        assert parsed == [{"name": "a"}, {"name": "b"}]

    @pytest.mark.asyncio
    async def test_none_return(self):
        """None return is serialized as 'null'."""
        def handler():
            return None

        adapted = _adapt_handler(handler)
        result = await adapted({})
        assert result["content"][0]["text"] == "null"

    @pytest.mark.asyncio
    async def test_deserializer(self):
        """Per-param deserializer is applied before calling handler."""
        received = {}

        def handler(spec: FunctionSpec):
            received["spec"] = spec
            return "ok"

        adapted = _adapt_handler(handler, {"spec": _deserialize_function_spec})
        await adapted({"spec": {"function_name": "bar"}})

        assert isinstance(received["spec"], FunctionSpec)
        assert received["spec"].function_name == "bar"


# ---------------------------------------------------------------------------
# TOOL_SCHEMAS / TOOL_DESCRIPTIONS tests
# ---------------------------------------------------------------------------


class TestToolSchemas:
    """Tests for TOOL_SCHEMAS and TOOL_DESCRIPTIONS completeness."""

    EXPECTED_TOOLS = [
        "verify_function_contract",
        "check_acsl_syntax",
        "get_function_context",
        "apply_precondition_weakening",
        "verify_rte_assertions",
        "verify_callsite_preconditions",
        "submit_proof_demand",
        "resolve_proof_demand",
        "mark_vo_as_bug",
        "check_inbox",
        "send_spec_request",
        "send_spec_response",
        "notify_spec_update",
        "read_function_spec",
        "read_my_spec",
        "update_my_spec",
    ]

    def test_all_tools_have_schemas(self):
        for name in self.EXPECTED_TOOLS:
            assert name in TOOL_SCHEMAS, f"Missing schema for {name}"

    def test_all_tools_have_descriptions(self):
        for name in self.EXPECTED_TOOLS:
            assert name in TOOL_DESCRIPTIONS, f"Missing description for {name}"

    def test_schema_count(self):
        assert len(TOOL_SCHEMAS) == 17

    def test_schemas_are_valid_json_schema_objects(self):
        for name, schema in TOOL_SCHEMAS.items():
            assert schema["type"] == "object", f"{name} schema type should be 'object'"
            assert "properties" in schema, f"{name} schema missing 'properties'"
            assert "required" in schema, f"{name} schema missing 'required'"

    def test_verify_function_contract_schema(self):
        schema = TOOL_SCHEMAS["verify_function_contract"]
        assert "annotations" in schema["properties"]
        assert "annotations" in schema["required"]

    def test_check_inbox_has_no_required(self):
        schema = TOOL_SCHEMAS["check_inbox"]
        assert schema["required"] == []

    def test_update_my_spec_has_spec_required(self):
        schema = TOOL_SCHEMAS["update_my_spec"]
        assert "spec" in schema["required"]

    def test_submit_proof_demand_required_fields(self):
        schema = TOOL_SCHEMAS["submit_proof_demand"]
        assert schema["required"] == [
            "target_function",
            "property_kind",
            "property_text",
        ]

    def test_resolve_proof_demand_required_fields(self):
        schema = TOOL_SCHEMAS["resolve_proof_demand"]
        assert schema["required"] == ["demand_id", "resolution"]
        assert "message" in schema["properties"]

    def test_mark_vo_as_bug_requires_reason(self):
        schema = TOOL_SCHEMAS["mark_vo_as_bug"]
        assert schema["required"] == ["vo_id", "reason"]


# ---------------------------------------------------------------------------
# _deserialize_function_spec tests
# ---------------------------------------------------------------------------


class TestDeserializeFunctionSpec:
    def test_minimal(self):
        spec = _deserialize_function_spec({"function_name": "bar"})
        assert spec.function_name == "bar"
        assert spec.preconditions == []

    def test_full(self):
        spec = _deserialize_function_spec({
            "function_name": "baz",
            "preconditions": ["x > 0"],
            "postconditions": ["\\result > 0"],
            "loop_contracts": {"L1": ["i >= 0"]},
            "assigns": ["\\nothing"],
            "verified": True,
        })
        assert spec.function_name == "baz"
        assert spec.preconditions == ["x > 0"]
        assert spec.loop_contracts == {"L1": ["i >= 0"]}


# ---------------------------------------------------------------------------
# create_agent_mcp_server tests
# ---------------------------------------------------------------------------


class TestCreateAgentMcpServer:
    def test_returns_sdk_server_config(self, tool_ctx):
        server = create_agent_mcp_server(tool_ctx)
        assert isinstance(server, dict)
        assert server["type"] == "sdk"
        assert "verdict-agent-agent_foo" in server["name"]

    def test_server_has_instance(self, tool_ctx):
        server = create_agent_mcp_server(tool_ctx)
        assert "instance" in server

    def test_debug_profile_has_distinct_server_name(self, tool_ctx):
        server = create_agent_mcp_server(tool_ctx, tool_profile="debug")
        assert server["name"].endswith("-debug")
