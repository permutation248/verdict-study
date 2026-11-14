"""Tests for cli.agent_stream sinks and stream processor."""

from __future__ import annotations

import asyncio
from io import StringIO
from unittest.mock import patch

import pytest

from verdict.cli.agent_stream import ChatSink, PrefixedSink


# ---------------------------------------------------------------------------
# PrefixedSink: prefix formatting + per-agent colors + atomicity under lock
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_prefixed_sink_adds_name_prefix_to_every_line():
    sink = PrefixedSink()
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_text("agent_main", "hello\nworld")
    out = buf.getvalue()
    # Both lines must carry the [main] prefix
    lines = [line for line in out.strip().splitlines() if line.strip()]
    assert len(lines) == 2
    assert "[main]" in lines[0]
    assert "[main]" in lines[1]
    assert "hello" in lines[0]
    assert "world" in lines[1]


@pytest.mark.asyncio
async def test_prefixed_sink_strips_agent_prefix():
    sink = PrefixedSink()
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_text("agent_compute_average", "working")
    out = buf.getvalue()
    # The 'agent_' prefix is stripped for display
    assert "[compute_average]" in out
    assert "[agent_compute_average]" not in out


@pytest.mark.asyncio
async def test_prefixed_sink_assigns_distinct_colors():
    sink = PrefixedSink()
    # First three agents get three distinct color codes
    c1 = sink._color_for("agent_a")
    c2 = sink._color_for("agent_b")
    c3 = sink._color_for("agent_c")
    assert c1 != c2
    assert c2 != c3
    # Same agent always gets the same color
    assert sink._color_for("agent_a") == c1


@pytest.mark.asyncio
async def test_prefixed_sink_tool_call_and_result():
    sink = PrefixedSink()
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_tool_call("agent_main", "tu_1", "verify_function_contract", {"annotations": {}})
        await sink.on_tool_result(
            "agent_main",
            "tu_1",
            "verify_function_contract",
            {"annotations": {}},
            content="proved 3 goals",
            is_error=False,
        )
    out = buf.getvalue()
    assert "[main]" in out
    assert "verify_function_contract" in out
    assert "[call]" in out
    assert "[done]" in out
    assert "proved 3 goals" in out


@pytest.mark.asyncio
async def test_prefixed_sink_cycle_markers():
    sink = PrefixedSink()
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_cycle_start("agent_inc")
        await sink.on_cycle_end(
            "agent_inc", is_error=False, cost=0.001234, turns=5
        )
    out = buf.getvalue()
    assert "[inc]" in out
    assert "cycle start" in out
    assert "cycle done" in out
    assert "turns=5" in out
    assert "cost=$0.0012" in out


@pytest.mark.asyncio
async def test_prefixed_sink_lock_serializes_concurrent_writes():
    """Two agents printing multi-line text concurrently must produce
    an interleaving at the block level, not mid-line. Each agent's
    lines must all appear in order, and every line must carry the
    correct prefix."""
    sink = PrefixedSink()
    buf = StringIO()

    async def _emit(agent: str, payload: str):
        await sink.on_text(agent, payload)

    with patch("sys.stdout", buf):
        await asyncio.gather(
            _emit("agent_alpha", "line-a1\nline-a2\nline-a3"),
            _emit("agent_beta", "line-b1\nline-b2\nline-b3"),
            _emit("agent_alpha", "line-a4\nline-a5"),
        )

    lines = [line for line in buf.getvalue().splitlines() if line.strip()]
    # Every line should carry exactly one tag
    for line in lines:
        assert ("[alpha]" in line) ^ ("[beta]" in line), f"bad line: {line!r}"
    # Alpha's lines appear in submitted order (relative within same agent)
    alpha_texts = [
        line for line in lines if "[alpha]" in line
    ]
    # a1..a5 must appear in order
    indices = [alpha_texts.index(l) for l in alpha_texts if "line-a" in l]
    assert indices == sorted(indices)
    # Same for beta
    beta_texts = [line for line in lines if "[beta]" in line]
    beta_indices = [beta_texts.index(l) for l in beta_texts if "line-b" in l]
    assert beta_indices == sorted(beta_indices)


# ---------------------------------------------------------------------------
# ChatSink: smoke tests for the refactored chat-mode path
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_chat_sink_prints_text_without_prefix():
    sink = ChatSink(output_buffer=None, conv_logger=None)
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_text("main", "hello world")
    out = buf.getvalue()
    assert "main: hello world" in out
    # Chat sink does not use bracketed agent tag
    assert "[main]" not in out


@pytest.mark.asyncio
async def test_chat_sink_tool_call_and_done_header():
    sink = ChatSink(output_buffer=None, conv_logger=None)
    buf = StringIO()
    with patch("sys.stdout", buf):
        await sink.on_tool_call(
            "main", "tu_1", "mcp__verdict__verify_function_contract", {"x": 1}
        )
        await sink.on_tool_result(
            "main",
            "tu_1",
            "mcp__verdict__verify_function_contract",
            {"x": 1},
            content="ok",
            is_error=False,
        )
    out = buf.getvalue()
    assert "[call]" in out
    assert "[done]" in out
    assert "verify_function_contract" in out
    # MCP prefix is stripped in display
    assert "mcp__verdict__" not in out


# ---------------------------------------------------------------------------
# Log capture: `▸` lines prefixed by the owning agent
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_log_capture_tags_records_with_owning_agent():
    """Log records emitted while _agent_id_var + _tool_call_seq are
    set should surface as prefixed `▸` lines under the owning agent."""
    import logging

    from verdict.tools.context import _agent_id_var, _tool_call_seq

    root = logging.getLogger("verdict")
    original_level = root.level
    root.setLevel(logging.DEBUG)

    sink = PrefixedSink()
    sink.install_log_capture()
    buf = StringIO()
    try:
        logger = logging.getLogger("verdict.tools.test_stream")
        agent_token = _agent_id_var.set("agent_main")
        seq_token = _tool_call_seq.set(42)
        try:
            with patch("sys.stdout", buf):
                logger.info("worklist phase 1: 3 proved, 0 failed")
                logger.info("multi\nline\nmessage")
        finally:
            _tool_call_seq.reset(seq_token)
            _agent_id_var.reset(agent_token)
    finally:
        sink.uninstall_log_capture()
        root.setLevel(original_level)

    out = buf.getvalue()
    assert "[main]" in out
    assert "\u25b8" in out
    assert "worklist phase 1: 3 proved, 0 failed" in out
    # Each line of the multi-line message gets its own prefixed ▸ line
    assert "\u25b8 multi" in out
    assert "\u25b8 line" in out
    assert "\u25b8 message" in out


@pytest.mark.asyncio
async def test_log_capture_ignores_records_outside_tool_calls():
    """Log records with no _agent_id_var set go through the normal
    console handler, not the prefixed sink. This keeps orchestrator-level
    logs (spec_published dispatch, cycle counters, etc.) untouched."""
    import logging

    root = logging.getLogger("verdict")
    original_level = root.level
    root.setLevel(logging.DEBUG)

    sink = PrefixedSink()
    sink.install_log_capture()
    buf = StringIO()
    try:
        logger = logging.getLogger("verdict.orchestrator.test_bg")
        with patch("sys.stdout", buf):
            logger.info("background orchestrator event")
    finally:
        sink.uninstall_log_capture()
        root.setLevel(original_level)

    out = buf.getvalue()
    # The prefixed sink's handler emitted nothing — our stdout capture
    # shows no `▸` line. (Other handlers on the verdict logger may have
    # written to their own streams; we're only checking ours is silent.)
    assert "\u25b8" not in out
    assert "[main]" not in out


@pytest.mark.asyncio
async def test_log_capture_defaults_to_info_filters_debug():
    """INFO is the default capture level; DEBUG lines from framac/cil
    don't reach the terminal unless --verbose bumps the level."""
    import logging

    from verdict.tools.context import _agent_id_var, _tool_call_seq

    root = logging.getLogger("verdict")
    original_level = root.level
    root.setLevel(logging.DEBUG)

    sink = PrefixedSink()
    sink.install_log_capture()  # default level=INFO
    buf = StringIO()
    try:
        logger = logging.getLogger("verdict.framac.test")
        agent_token = _agent_id_var.set("agent_main")
        seq_token = _tool_call_seq.set(7)
        try:
            with patch("sys.stdout", buf):
                logger.debug("Running command: frama-c -wp-prop postcondition_0 ...")
                logger.info("Worklist completed: 3/3 verified")
        finally:
            _tool_call_seq.reset(seq_token)
            _agent_id_var.reset(agent_token)
    finally:
        sink.uninstall_log_capture()
        root.setLevel(original_level)

    out = buf.getvalue()
    # DEBUG noise dropped
    assert "Running command" not in out
    # INFO kept
    assert "Worklist completed: 3/3 verified" in out
    # Rendered with dim + deep indent
    assert "\u25b8 Worklist completed" in out
    assert "[main]" in out


@pytest.mark.asyncio
async def test_log_capture_verbose_includes_debug():
    import logging

    from verdict.tools.context import _agent_id_var, _tool_call_seq

    root = logging.getLogger("verdict")
    original_level = root.level
    root.setLevel(logging.DEBUG)

    sink = PrefixedSink()
    sink.install_log_capture(level=logging.DEBUG)
    buf = StringIO()
    try:
        logger = logging.getLogger("verdict.framac.test")
        agent_token = _agent_id_var.set("agent_main")
        seq_token = _tool_call_seq.set(7)
        try:
            with patch("sys.stdout", buf):
                logger.debug("verbose detail")
        finally:
            _tool_call_seq.reset(seq_token)
            _agent_id_var.reset(agent_token)
    finally:
        sink.uninstall_log_capture()
        root.setLevel(original_level)

    out = buf.getvalue()
    assert "verbose detail" in out


@pytest.mark.asyncio
async def test_log_capture_uninstall_restores_console_level():
    import logging

    root = logging.getLogger("verdict")
    # Find and remember the first stream handler level
    for h in root.handlers:
        if (
            isinstance(h, logging.StreamHandler)
            and not isinstance(h, logging.FileHandler)
        ):
            original = h.level
            break
    else:
        pytest.skip("no console handler to suppress")

    sink = PrefixedSink()
    sink.install_log_capture()
    # Capture should have bumped the console to WARNING
    for h in root.handlers:
        if (
            isinstance(h, logging.StreamHandler)
            and not isinstance(h, logging.FileHandler)
            and h is not sink._log_handler
        ):
            assert h.level == logging.WARNING
            break
    sink.uninstall_log_capture()
    # Restored
    for h in root.handlers:
        if (
            isinstance(h, logging.StreamHandler)
            and not isinstance(h, logging.FileHandler)
        ):
            assert h.level == original
            break
