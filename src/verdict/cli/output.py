"""Tool output buffering and formatting for the CLI."""

from __future__ import annotations

import json
import logging
import sys
from typing import Any

import verdict.tools.context as _ctx
from verdict.tools.context import _tool_call_seq, _completed_seqs, _seq_tool_info

# MCP server name used in ClaudeAgentOptions.mcp_servers
_MCP_PREFIX = "mcp__verdict__"


class _ToolOutputBuffer:
    """Capture verdict.* log records grouped by tool call, flush per-tool.

    Log records are tagged via ``_tool_call_seq`` contextvar.  When a
    ToolResultBlock arrives, ``flush_one()`` pops one completed seq and
    prints its grouped ``▸`` lines.  The chat loop prints ``[call]`` on
    start and ``[done]`` + logs + ``[result]`` on completion.
    """

    class _Handler(logging.Handler):
        def __init__(self, buf: "_ToolOutputBuffer") -> None:
            super().__init__()
            self._buf = buf

        def emit(self, record: logging.LogRecord) -> None:
            msg = self.format(record)
            seq = _tool_call_seq.get(-1)
            if seq < 0:
                self._buf._ungrouped.append(msg)
            else:
                self._buf._seq_logs.setdefault(seq, []).append(msg)

    def __init__(self, verbose: bool = False) -> None:
        self._verbose = verbose
        self._handler = self._Handler(self)
        self._handler.setLevel(logging.DEBUG if verbose else logging.INFO)
        self._handler.setFormatter(logging.Formatter("%(message)s"))
        self._console_handler: logging.Handler | None = None
        self._original_level: int = logging.NOTSET
        self._seq_logs: dict[int, list[str]] = {}
        self._ungrouped: list[str] = []

    def install(self) -> None:
        """Attach to the verdict root logger; suppress its console handler."""
        root = logging.getLogger("verdict")
        for h in root.handlers:
            if isinstance(h, logging.StreamHandler) and not isinstance(h, logging.FileHandler):
                self._console_handler = h
                self._original_level = h.level
                h.setLevel(logging.WARNING)
                break
        root.addHandler(self._handler)
        # Register callback so handler completion triggers immediate output
        _ctx._on_tool_complete = self._on_complete

    def uninstall(self) -> None:
        """Restore original console handler level and remove our handler."""
        _ctx._on_tool_complete = None
        root = logging.getLogger("verdict")
        root.removeHandler(self._handler)
        if self._console_handler is not None:
            self._console_handler.setLevel(self._original_level)

    def _on_complete(self, seq: int) -> None:
        """Called by factory when a tool handler finishes -- print immediately."""
        # Remove from _completed_seqs so flush_all doesn't re-process
        try:
            _completed_seqs.remove(seq)
        except ValueError:
            pass
        self._print_seq_group(seq)

    def _print_seq_group(self, seq: int) -> None:
        """Print [done] header + ▸ logs for one completed seq."""
        logs = self._seq_logs.pop(seq, [])
        tool_name, args_str = _seq_tool_info.pop(seq, ("", ""))
        if not logs:
            return
        if tool_name:
            color = _C.VERDICT_TOOL
            print(f"  {color}[done]{_C.RESET} {color}{_C.BOLD}{tool_name}{_C.RESET}{color}({args_str}){_C.RESET}")
        for msg in logs:
            print(f"    {_C.DIM}\u25b8 {msg}{_C.RESET}")
        sys.stdout.flush()

    def flush_one(self) -> None:
        """Pop one completed seq and print its [done] + ▸ log lines."""
        if _completed_seqs:
            self._print_seq_group(_completed_seqs.pop(0))

    def flush_all(self) -> None:
        """Print any remaining buffered records (fallback before TextBlock)."""
        while _completed_seqs:
            self._print_seq_group(_completed_seqs.pop(0))
        for seq, logs in sorted(self._seq_logs.items()):
            for msg in logs:
                print(f"    {_C.DIM}\u25b8 {msg}{_C.RESET}")
            _seq_tool_info.pop(seq, None)
        for msg in self._ungrouped:
            print(f"    {_C.DIM}\u25b8 {msg}{_C.RESET}")
        self._seq_logs.clear()
        self._ungrouped.clear()


# ---------------------------------------------------------------------------
# ANSI colors
# ---------------------------------------------------------------------------

class _C:
    """ANSI color codes (disabled when not a TTY)."""
    RESET = "\033[0m"
    BOLD = "\033[1m"
    DIM = "\033[2m"

    # Tool colors
    VERDICT_TOOL = "\033[38;5;43m"   # teal -- our custom MCP tools
    BUILTIN_TOOL = "\033[38;5;247m"  # grey -- built-in tools (Read, Bash, etc.)
    TOOL_RESULT = "\033[38;5;244m"   # dark grey -- tool result summary

    # Chat colors
    USER = "\033[38;5;75m"           # blue
    AGENT = "\033[38;5;222m"         # amber
    ERROR = "\033[38;5;203m"         # red
    SYSTEM = "\033[38;5;248m"        # light grey


_COLORS_ON = True


def _init_colors() -> None:
    """Disable colors if stdout is not a TTY."""
    global _COLORS_ON
    if not sys.stdout.isatty():
        _COLORS_ON = False
        for attr in dir(_C):
            if not attr.startswith("_"):
                setattr(_C, attr, "")


def colors_enabled() -> bool:
    """Single source of truth for ANSI color emission.

    Palettes defined outside _C (e.g. the per-agent prefix colors in
    agent_stream) must consult this instead of printing raw codes, or
    escape sequences leak into piped logs.
    """
    return _COLORS_ON


# ---------------------------------------------------------------------------
# Tool name formatting
# ---------------------------------------------------------------------------


def _format_tool_name(raw_name: str) -> tuple[str, bool]:
    """Strip mcp__verdict__ prefix if present.

    Returns (display_name, is_verdict_tool).
    """
    if raw_name.startswith(_MCP_PREFIX):
        return raw_name[len(_MCP_PREFIX):], True
    return raw_name, False


def _format_tool_call(tool_name: str, args: dict, label: str = "call") -> str:
    """Format tool call with color-coded name. Returns the formatted string."""
    display_name, is_verdict = _format_tool_name(tool_name)
    args_str = json.dumps(args, indent=None, ensure_ascii=False) if args else ""
    if len(args_str) > 120:
        args_str = args_str[:117] + "..."

    color = _C.VERDICT_TOOL if is_verdict else _C.BUILTIN_TOOL
    return f"  {color}[{label}]{_C.RESET} {color}{_C.BOLD}{display_name}{_C.RESET}{color}({args_str}){_C.RESET}"


def _format_tool_result(content: Any, is_error: bool = False) -> str | None:
    """Format abbreviated tool result. Returns None if empty."""
    if isinstance(content, str):
        summary = content
    elif isinstance(content, list):
        parts = []
        for block in content:
            if isinstance(block, dict) and block.get("type") == "text":
                parts.append(block.get("text", ""))
            else:
                parts.append(str(block))
        summary = " ".join(parts)
    else:
        summary = str(content) if content else ""

    if not summary:
        return None

    # Truncate for console display
    if len(summary) > 200:
        summary = summary[:197] + "..."

    prefix = "error" if is_error else "result"
    return f"  {_C.TOOL_RESULT}[{prefix}] {summary}{_C.RESET}"
