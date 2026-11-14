"""ToolContext: shared context passed to all MCP tool implementations."""

from __future__ import annotations

import contextvars
import logging
from dataclasses import dataclass, field
from typing import TYPE_CHECKING, Callable

# Per-tool-call sequence number, set in factory._adapt_handler before each
# handler invocation.  Used by cli._ToolOutputBuffer to group log records
# by originating tool call so concurrent calls don't interleave output.
_tool_call_seq: contextvars.ContextVar[int] = contextvars.ContextVar(
    "_tool_call_seq", default=-1
)

# Completed seq numbers, appended by factory._adapt_handler when a handler
# finishes.  cli._ToolOutputBuffer pops from this to flush per-tool output
# as soon as each tool completes (rather than waiting for all tools).
_completed_seqs: list[int] = []

# Seq → (tool_name, args_summary) populated by factory._adapt_handler so
# that cli._ToolOutputBuffer.flush_all() can print [done] headers.
_seq_tool_info: dict[int, tuple[str, str]] = {}

# Callback invoked by factory._adapt_handler when a tool call finishes.
# Set by cli._ToolOutputBuffer to enable immediate per-tool output.
_on_tool_complete: Callable[[int], None] | None = None

# Agent id for the currently-executing tool call. Set by
# factory._adapt_handler so logging handlers can attribute log records
# to the owning agent when multiple agents run concurrently.
_agent_id_var: contextvars.ContextVar[str] = contextvars.ContextVar(
    "_agent_id_var", default=""
)

from verdict.core.ablation import AblationConfig
from verdict.core.types import FunctionInfo, ToolProfile
from verdict.agent.registry import AgentRegistry
from verdict.messaging.bus import AsyncQueueBus

if TYPE_CHECKING:
    from verdict.core.project_context import ProjectContext
    from verdict.core.spec_tracker import SpecTracker
    from verdict.core.vo_tracker import VOTracker
    from verdict.logging.logger import ToolDebugger


@dataclass
class ToolContext:
    """Injected into every MCP tool call so tools can access framework state."""

    agent_id: str
    function_info: FunctionInfo
    registry: AgentRegistry
    message_bus: AsyncQueueBus
    logger: logging.Logger
    project_context: ProjectContext | None = None
    vo_tracker: VOTracker | None = None
    debugger: ToolDebugger | None = None
    spec_tracker: SpecTracker | None = None
    tool_profile: ToolProfile = "solve"
    wp_timeout: int = 10
    # RQ3 coordination ablation; the default instance means "everything
    # enabled" so existing constructions are unaffected.
    ablation: AblationConfig = field(default_factory=AblationConfig)
