"""Reusable stream processor for agent output — shared by chat and solve modes.

The same Claude SDK message loop drives both interactive chat and the
multi-agent solve orchestrator. The loop receives blocks from
`client.receive_response()` and dispatches them to an `AgentOutputSink`
that decides how to render each block.

- chat mode installs a `ChatSink` that prints directly to stdout with no
  per-agent prefix (only one conversation is active at a time).
- solve mode installs a `PrefixedSink` that tags every line with a
  colored `[function_name]` prefix and serializes concurrent agent
  output under an asyncio lock so lines from different agents never
  interleave mid-line.
"""

from __future__ import annotations

import asyncio
import logging
from typing import TYPE_CHECKING, Any, Protocol

if TYPE_CHECKING:
    from claude_agent_sdk import ClaudeSDKClient


class AgentOutputSink(Protocol):
    """Protocol for rendering an agent's message stream."""

    async def on_cycle_start(self, agent_id: str) -> None: ...

    async def on_text(self, agent_id: str, text: str) -> None: ...

    async def on_tool_call(
        self, agent_id: str, tool_use_id: str, name: str, args: dict[str, Any]
    ) -> None: ...

    async def on_tool_result(
        self,
        agent_id: str,
        tool_use_id: str,
        tool_name: str,
        tool_args: dict[str, Any],
        content: Any,
        is_error: bool,
    ) -> None: ...

    async def on_cycle_end(
        self,
        agent_id: str,
        is_error: bool,
        cost: float | None,
        turns: int | None,
    ) -> None: ...


async def process_agent_stream(
    client: "ClaudeSDKClient",
    agent_id: str,
    sink: AgentOutputSink,
) -> str | None:
    """Consume a ClaudeSDKClient response stream and dispatch to a sink.

    Returns the session_id from the final ResultMessage (or None if the
    stream ended without one). The runner stores this so the next cycle's
    client.query() can resume the same session.
    """
    from claude_agent_sdk import (
        AssistantMessage,
        ResultMessage,
        TextBlock,
        ToolResultBlock,
        ToolUseBlock,
    )

    pending: dict[str, tuple[str, dict[str, Any]]] = {}
    first_msg = True
    import logging
    agent_logger = logging.getLogger(f"verdict.agent.{agent_id}")

    async for msg in client.receive_response():
        if first_msg:
            agent_logger.debug("SDK: first message received (%s)", type(msg).__name__)
            first_msg = False
        if isinstance(msg, AssistantMessage):
            # API-level error on the assistant turn (auth, rate_limit,
            # server_error, etc). Surface as WARNING so it shows up in
            # pipeline.log even though it comes through as a regular
            # message rather than an exception.
            api_err = getattr(msg, "error", None)
            if api_err is not None:
                agent_logger.warning("API error: %s", api_err)
            for block in msg.content:
                if isinstance(block, TextBlock):
                    await sink.on_text(agent_id, block.text)
                elif isinstance(block, ToolUseBlock):
                    pending[block.id] = (block.name, block.input)
                    await sink.on_tool_call(
                        agent_id, block.id, block.name, block.input
                    )
                elif isinstance(block, ToolResultBlock):
                    tool_name, tool_args = pending.pop(
                        block.tool_use_id, ("?", {})
                    )
                    await sink.on_tool_result(
                        agent_id,
                        block.tool_use_id,
                        tool_name,
                        tool_args,
                        block.content,
                        bool(block.is_error),
                    )
        elif isinstance(msg, ResultMessage):
            is_err = bool(getattr(msg, "is_error", False))
            if is_err:
                subtype = getattr(msg, "subtype", "?")
                result_text = getattr(msg, "result", "") or ""
                agent_logger.warning(
                    "Cycle ended with error (subtype=%s): %s",
                    subtype, result_text[:200],
                )
            cost = getattr(msg, "total_cost_usd", None)
            turns = getattr(msg, "num_turns", None)
            await sink.on_cycle_end(
                agent_id, is_error=is_err, cost=cost, turns=turns,
            )
            return getattr(msg, "session_id", None), cost, turns
    return None, None, None


# ---------------------------------------------------------------------------
# Chat sink: direct print, no agent prefix (single-conversation mode)
# ---------------------------------------------------------------------------


class ChatSink:
    """Renders agent output for interactive chat mode: no prefix."""

    def __init__(
        self,
        output_buffer: Any | None = None,
        conv_logger: Any | None = None,
    ) -> None:
        from verdict.cli.output import _format_tool_call, _format_tool_name, _format_tool_result

        self._format_call = _format_tool_call
        self._format_name = _format_tool_name
        self._format_result = _format_tool_result
        self._buffer = output_buffer
        self._conv = conv_logger

    async def on_cycle_start(self, agent_id: str) -> None:
        pass

    async def on_text(self, agent_id: str, text: str) -> None:
        from verdict.cli.output import _C

        if self._buffer is not None:
            self._buffer.flush_all()
        print(f"  {_C.AGENT}{agent_id}: {text}{_C.RESET}")
        if self._conv is not None:
            self._conv.log_agent_text(agent_id, text)

    async def on_tool_call(
        self, agent_id: str, tool_use_id: str, name: str, args: dict[str, Any]
    ) -> None:
        print(self._format_call(name, args, "call"))
        display_name, _ = self._format_name(name)
        if self._conv is not None:
            self._conv.log_tool_call(agent_id, display_name, args)

    async def on_tool_result(
        self,
        agent_id: str,
        tool_use_id: str,
        tool_name: str,
        tool_args: dict[str, Any],
        content: Any,
        is_error: bool,
    ) -> None:
        # Reproduce chat mode's [done] header + buffer flush + [result] flow.
        _, is_verdict = self._format_name(tool_name)
        print(self._format_call(tool_name, tool_args, "done"))
        if is_verdict and self._buffer is not None:
            self._buffer.flush_one()
        if self._conv is not None:
            self._conv.log_tool_result(
                agent_id, tool_use_id, content, is_error=is_error
            )
        line = self._format_result(content, is_error=is_error)
        if line:
            print(line)

    async def on_cycle_end(
        self,
        agent_id: str,
        is_error: bool,
        cost: float | None,
        turns: int | None,
    ) -> None:
        from verdict.cli.output import _C

        if self._buffer is not None:
            self._buffer.flush_all()
        if self._conv is not None:
            self._conv.log_result(agent_id, is_error=is_error, cost=cost, turns=turns)
        if is_error:
            print(f"  {_C.ERROR}[error] Agent returned an error.{_C.RESET}")


# ---------------------------------------------------------------------------
# Prefixed sink: per-agent color + asyncio lock for atomic multi-line output
# ---------------------------------------------------------------------------


_AGENT_COLORS = [
    "\033[38;5;75m",   # blue
    "\033[38;5;222m",  # amber
    "\033[38;5;156m",  # light green
    "\033[38;5;213m",  # pink
    "\033[38;5;111m",  # steel blue
    "\033[38;5;178m",  # mustard
    "\033[38;5;147m",  # lavender
    "\033[38;5;108m",  # teal
]


class PrefixedSink:
    """Multi-agent sink that prefixes every line with a colored [name]
    tag and serializes concurrent writes under an asyncio lock.

    The lock is held for the duration of one handler call (one text
    block, one tool call, one tool result), so multi-line output from a
    single agent is flushed as one unit — other agents' prints wait.

    Optionally captures `verdict.*` logger records emitted during a tool
    call and prints them as `▸` lines prefixed with the owning agent's
    tag. Enable via `install_log_capture()` / disable via
    `uninstall_log_capture()`.
    """

    def __init__(self) -> None:
        from verdict.cli.output import _format_tool_call, _format_tool_name, _format_tool_result

        self._format_call = _format_tool_call
        self._format_name = _format_tool_name
        self._format_result = _format_tool_result
        self._lock = asyncio.Lock()
        self._colors: dict[str, str] = {}
        self._log_handler: logging.Handler | None = None
        self._suppressed_console: logging.Handler | None = None
        self._original_console_level: int = logging.NOTSET

    def _color_for(self, agent_id: str) -> str:
        from verdict.cli.output import colors_enabled

        if not colors_enabled():
            return ""
        if agent_id not in self._colors:
            idx = len(self._colors) % len(_AGENT_COLORS)
            self._colors[agent_id] = _AGENT_COLORS[idx]
        return self._colors[agent_id]

    def _prefix(self, agent_id: str) -> str:
        from verdict.cli.output import _C

        name = agent_id.removeprefix("agent_")
        color = self._color_for(agent_id)
        return f"{color}[{name}]{_C.RESET} "

    def _render_prefixed(self, agent_id: str, text: str) -> str:
        """Prefix every non-empty line of `text` with the agent tag."""
        prefix = self._prefix(agent_id)
        lines = text.splitlines() or [""]
        return "\n".join(prefix + line for line in lines)

    async def on_cycle_start(self, agent_id: str) -> None:
        from verdict.cli.output import _C

        async with self._lock:
            name = agent_id.removeprefix("agent_")
            color = self._color_for(agent_id)
            print(f"{color}[{name}] ─── cycle start ───{_C.RESET}", flush=True)

    async def on_text(self, agent_id: str, text: str) -> None:
        if not text.strip():
            return
        async with self._lock:
            print(self._render_prefixed(agent_id, text), flush=True)

    async def on_tool_call(
        self, agent_id: str, tool_use_id: str, name: str, args: dict[str, Any]
    ) -> None:
        line = self._format_call(name, args, "call")
        async with self._lock:
            print(self._render_prefixed(agent_id, line), flush=True)

    async def on_tool_result(
        self,
        agent_id: str,
        tool_use_id: str,
        tool_name: str,
        tool_args: dict[str, Any],
        content: Any,
        is_error: bool,
    ) -> None:
        done_line = self._format_call(tool_name, tool_args, "done")
        result_line = self._format_result(content, is_error=is_error)
        async with self._lock:
            print(self._render_prefixed(agent_id, done_line), flush=True)
            if result_line is not None:
                print(self._render_prefixed(agent_id, result_line), flush=True)

    async def on_cycle_end(
        self,
        agent_id: str,
        is_error: bool,
        cost: float | None,
        turns: int | None,
    ) -> None:
        from verdict.cli.output import _C

        async with self._lock:
            name = agent_id.removeprefix("agent_")
            color = self._color_for(agent_id)
            bits: list[str] = []
            if turns is not None:
                bits.append(f"turns={turns}")
            if cost is not None:
                bits.append(f"cost=${cost:.4f}")
            suffix = " " + " ".join(bits) if bits else ""
            marker = "error" if is_error else "done"
            print(
                f"{color}[{name}] ─── cycle {marker}{suffix} ───{_C.RESET}",
                flush=True,
            )

    # ------------------------------------------------------------------
    # Log capture: stream `verdict.*` log records emitted during tool
    # handlers as prefixed `▸` lines under the owning agent.
    # ------------------------------------------------------------------

    def install_log_capture(self, level: int = logging.INFO) -> None:
        """Attach a logging handler that prints log records from inside
        tool calls with the owning agent's prefix.

        Log records only surface when _agent_id_var is set (i.e. inside
        a tool handler wrapped by factory._adapt_handler). Records
        outside a tool call are ignored so general orchestrator logs
        still go through the normal console handler.

        Level defaults to INFO so noisy DEBUG-level FramaC/CIL plumbing
        stays in pipeline.log. Pass logging.DEBUG for verbose mode.
        """
        if self._log_handler is not None:
            return

        sink = self

        class _PrefixedLogHandler(logging.Handler):
            def emit(self, record: logging.LogRecord) -> None:
                from verdict.cli.output import _C
                from verdict.tools.context import _agent_id_var, _tool_call_seq

                agent_id = _agent_id_var.get("")
                seq = _tool_call_seq.get(-1)
                if not agent_id or seq < 0:
                    # Outside of any tool call — let other handlers format it.
                    return
                try:
                    msg = self.format(record)
                except Exception:
                    return
                prefix = sink._prefix(agent_id)
                # Dim grey + deeper indent (8 spaces past the prefix) so
                # tool-internal lines visually nest under the [call]/[done]
                # headers and fade into the background compared to agent
                # text and tool-call lines.
                indent = "        "
                dim = _C.DIM
                reset = _C.RESET
                for line in (msg or "").splitlines() or [msg or ""]:
                    print(
                        f"{prefix}{dim}{indent}\u25b8 {line}{reset}",
                        flush=True,
                    )

        handler = _PrefixedLogHandler()
        handler.setLevel(level)
        handler.setFormatter(logging.Formatter("%(message)s"))
        root = logging.getLogger("verdict")
        root.addHandler(handler)
        self._log_handler = handler

        # Suppress the default console handler's INFO output during solve
        # so tool-internal INFO/DEBUG lines only show up with our prefix.
        for h in list(root.handlers):
            if (
                isinstance(h, logging.StreamHandler)
                and not isinstance(h, logging.FileHandler)
                and h is not handler
            ):
                self._suppressed_console = h
                self._original_console_level = h.level
                h.setLevel(logging.WARNING)
                break

    def uninstall_log_capture(self) -> None:
        """Detach the log capture handler and restore console settings."""
        if self._log_handler is None:
            return
        root = logging.getLogger("verdict")
        try:
            root.removeHandler(self._log_handler)
        except Exception:
            pass
        self._log_handler = None
        if self._suppressed_console is not None:
            self._suppressed_console.setLevel(self._original_console_level)
            self._suppressed_console = None
            self._original_console_level = logging.NOTSET
