"""Debug-only MCP communication tools: direct inter-agent messaging."""

from __future__ import annotations

from typing import Any, Optional

from verdict.core.types import (
    AgentMessage,
    SpecType,
    SpecificationRequest,
    SpecificationResponse,
    SpecificationUpdate,
)
from verdict.tools.context import ToolContext


async def check_inbox(ctx: ToolContext) -> list[AgentMessage]:
    """Return and clear all pending debug messages for this agent (drains the
    per-agent bus queue — the bus is the inbox)."""
    messages: list[AgentMessage] = []
    while ctx.message_bus.has_messages(ctx.agent_id):
        messages.append(await ctx.message_bus.receive(ctx.agent_id))
    ctx.logger.debug("check_inbox: %d messages", len(messages))
    return messages


async def send_spec_request(
    ctx: ToolContext,
    target_function: str,
    request_type: str,
    required_property: str,
    context: dict[str, Any] | None = None,
) -> dict:
    """Send a debug-only SpecificationRequest to the agent owning *target_function*."""
    entry = ctx.registry.get_by_function(target_function)
    if entry is None:
        raise ValueError(f"No agent registered for function: {target_function}")
    req = SpecificationRequest(
        requester_id=ctx.agent_id,
        target_function=target_function,
        request_type=SpecType(request_type),
        required_property=required_property,
        context=context or {},
    )
    await ctx.message_bus.send(entry.agent_id, req)
    ctx.logger.debug("sent debug spec_request -> %s", entry.agent_id)
    return {
        "success": True,
        "delivered_to": entry.agent_id,
        "target_function": target_function,
        "request_type": request_type,
    }


async def send_spec_response(
    ctx: ToolContext,
    requester_id: str,
    target_function: str,
    request_type: str,
    status: str,
    specification: Optional[str] = None,
    error_message: Optional[str] = None,
) -> dict:
    """Reply to a debug SpecificationRequest."""
    resp = SpecificationResponse(
        target_function=target_function,
        responder_id=ctx.agent_id,
        request_type=SpecType(request_type),
        status=status,
        specification=specification,
        error_message=error_message,
    )
    await ctx.message_bus.send(requester_id, resp)
    ctx.logger.debug("sent debug spec_response -> %s status=%s", requester_id, status)
    return {
        "success": True,
        "delivered_to": requester_id,
        "status": status,
    }


async def notify_spec_update(
    ctx: ToolContext,
    spec_type: str,
    old_value: Optional[str] = None,
    new_value: Optional[str] = None,
) -> dict:
    """Broadcast a debug-only targeted SpecificationUpdate to caller agents."""
    current = ctx.registry.read_spec(ctx.function_info.name)
    update = SpecificationUpdate(
        source_agent_id=ctx.agent_id,
        function_name=ctx.function_info.name,
        spec_type=SpecType(spec_type),
        version=current.version if current else None,
        changed_kinds=[spec_type],
        old_value=old_value,
        new_value=new_value,
    )
    if spec_type in {"precondition", "postcondition", "assigns"}:
        affected = ctx.function_info.callers
    else:
        affected = []
    delivered: list[str] = []
    for func_name in set(affected):
        entry = ctx.registry.get_by_function(func_name)
        if entry:
            await ctx.message_bus.send(entry.agent_id, update)
            delivered.append(entry.agent_id)
    ctx.logger.debug("notified %d debug agents of spec update", len(affected))
    return {
        "success": True,
        "spec_type": spec_type,
        "delivered_to": delivered,
        "delivered_count": len(delivered),
    }
