"""Integration tests: agent tools on real C files with real Frama-C.

Uses the full pipeline:
  raw .c → Frama-C preprocessing → ECG parse → RTE renumber → re-parse → tools

No mocking — real ExtendedCallGraph, real Frama-C WP verification.
"""

from __future__ import annotations

import json
import logging
import os

import shutil

import pytest

logger = logging.getLogger(__name__)

# Inline skip check — conftest markers can't be imported as a module
_BINDINGS_DIR = os.environ.get("BINDINGS_DIR", "")
_LIB_EXT = "dylib" if os.uname().sysname == "Darwin" else "so"
_FULL_PIPELINE = (
    shutil.which("frama-c") is not None
    and os.path.isfile(os.path.join(_BINDINGS_DIR, "build", f"libclang_parser.{_LIB_EXT}"))
    and os.path.isfile(os.path.join(os.path.dirname(__file__), "..", "benchmarks", "x509", "submodule-2", "x509-submodule-2_cleaned_26.c"))
)

pytestmark = [
    pytest.mark.skipif(not _FULL_PIPELINE, reason="Full pipeline requires frama-c + bindings + benchmark"),
    pytest.mark.asyncio,
]

# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------

_BENCH_X509 = os.path.join(os.path.dirname(__file__), "..", "benchmarks", "x509", "submodule-2")
SOURCE_FILE = os.path.join(_BENCH_X509, "x509-submodule-2_cleaned_26.c")
HEADER_FILE = os.path.join(_BENCH_X509, "x509-parser.h")


@pytest.fixture(scope="module")
def project_context():
    """Build real ProjectContext once for all tests in this module.

    Runs the full pipeline: Frama-C preprocessing → ECG → RTE renumber → re-parse.
    """
    from tests.helpers.real_context_builder import build_real_project_context

    logger.info("=== Building ProjectContext from %s ===", SOURCE_FILE)
    ctx, ecg = build_real_project_context(SOURCE_FILE, header_files=[HEADER_FILE])

    logger.info("Functions: %s", list(ctx.call_graph_dict.keys()))
    logger.info("RTE VOs by function: %s", {
        k: len(v) for k, v in ctx.rte_vos_by_function.items()
    })
    return ctx


# ---------------------------------------------------------------------------
# Test 1: get_function_context — simple function (bufs_differ)
# ---------------------------------------------------------------------------


async def test_get_function_context_real(project_context):
    """get_function_context on bufs_differ: loops, no callees."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.inspection import get_function_context

    ctx = build_tool_context("bufs_differ", project_context)
    logger.info("INPUT: func_name=%s, depth=1", ctx.function_info.name)

    result = await get_function_context(ctx, depth=1)

    logger.info("OUTPUT: function_name=%s", result.function_name)
    logger.info("OUTPUT: source_code length=%d", len(result.source_code))
    logger.info("OUTPUT: loops=%d", len(result.loops))
    logger.info("OUTPUT: callee_layers=%d", len(result.callee_layers))

    assert result.function_name == "bufs_differ"
    assert "bufs_differ" in result.source_code
    assert len(result.loops) >= 1, "bufs_differ should have at least 1 loop"
    assert len(result.callee_layers) == 0, "bufs_differ has no callees"


# ---------------------------------------------------------------------------
# Test 2: check_acsl_syntax — valid annotations (bufs_differ)
# ---------------------------------------------------------------------------


async def test_check_acsl_syntax_valid_real(project_context):
    """check_acsl_syntax with valid preconditions for bufs_differ."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.inspection import check_acsl_syntax

    ctx = build_tool_context("bufs_differ", project_context)
    annotations = {
        "preconditions": [
            "requires \\valid_read(b1 + (0 .. n-1));",
            "requires \\valid_read(b2 + (0 .. n-1));",
        ],
    }
    logger.info("INPUT: annotations=%s", json.dumps(annotations, indent=2))

    result = await check_acsl_syntax(ctx, annotations)

    logger.info("OUTPUT: valid=%s, errors=%s", result.valid, result.errors)

    assert result.valid is True, f"Expected valid syntax, got errors: {result.errors}"


# ---------------------------------------------------------------------------
# Test 3: check_acsl_syntax — invalid annotations (bufs_differ)
# ---------------------------------------------------------------------------


async def test_check_acsl_syntax_invalid_real(project_context):
    """check_acsl_syntax with malformed ACSL for bufs_differ."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.inspection import check_acsl_syntax

    ctx = build_tool_context("bufs_differ", project_context)
    annotations = {
        "preconditions": [
            "requires \\not_a_real_predicate(b1);",
        ],
    }
    logger.info("INPUT: annotations=%s", json.dumps(annotations, indent=2))

    result = await check_acsl_syntax(ctx, annotations)

    logger.info("OUTPUT: valid=%s, errors=%s", result.valid, result.errors)

    assert result.valid is False, "Malformed ACSL should be rejected"
    assert len(result.errors) > 0


# ---------------------------------------------------------------------------
# Test 4: verify_function_contract — bufs_differ with real WP
# ---------------------------------------------------------------------------


async def test_verify_function_contract_real(project_context):
    """verify_function_contract on bufs_differ with preconditions + assigns."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.contract import verify_function_contract

    ctx = build_tool_context("bufs_differ", project_context)
    annotations = {
        "preconditions": [
            "requires n >= 0;",
            "requires \\valid_read(b1 + (0 .. n-1));",
            "requires \\valid_read(b2 + (0 .. n-1));",
        ],
        "postconditions": [],
        "assigns": ["assigns \\nothing;"],
        "loop_contracts": {},
    }
    logger.info("INPUT: annotations=%s", json.dumps(annotations, indent=2))

    result = await verify_function_contract(ctx, annotations)

    logger.info("OUTPUT: success=%s", result.success)
    logger.info("OUTPUT: feedback=%s", result.feedback)
    if result.wp_warnings:
        logger.info("OUTPUT: wp_warnings=%s", result.wp_warnings)

    # Worklist path: check per-spec structured fields
    has_goals = (
        bool(result.verified_loop_contracts) or bool(result.failed_loop_contracts)
        or bool(result.verified_postconditions) or bool(result.failed_postconditions)
        or bool(result.verified_assigns) or bool(result.failed_assigns)
    )
    assert has_goals, "WP should produce verification goals"


# ---------------------------------------------------------------------------
# Test 5: verify_rte_assertions — bufs_differ RTEs
# ---------------------------------------------------------------------------


async def test_verify_rte_assertions_real(project_context):
    """verify_rte_assertions on bufs_differ's RTE VOs with VOTracker."""
    from tests.helpers.real_context_builder import (
        build_tool_context,
        build_vo_tracker_for_function,
    )
    from verdict.tools.verify.rte import verify_rte_assertions

    vo_tracker = build_vo_tracker_for_function("bufs_differ", project_context)
    ctx = build_tool_context("bufs_differ", project_context, vo_tracker=vo_tracker)

    rte_vos = project_context.rte_vos_by_function.get("bufs_differ", [])
    assert len(rte_vos) > 0, "bufs_differ should have RTE assertions"

    logger.info("INPUT: vo_ids=%s", rte_vos)

    result = await verify_rte_assertions(ctx, rte_vos)

    logger.info("OUTPUT: total_count=%d", result.total_count)
    logger.info("OUTPUT: verified_count=%d", result.verified_count)
    logger.info("OUTPUT: all_verified=%s", result.all_verified)
    logger.info("OUTPUT: results=%s", {
        k: {"verified": v.verified, "obligation": v.obligation[:80]}
        for k, v in result.results.items()
    })
    logger.info("OUTPUT: summary=%s", result.summary)

    assert result.total_count == len(rte_vos)
    assert len(result.results) == len(rte_vos)
    # VOTracker should be updated
    for vo_id in rte_vos:
        entry = vo_tracker._find(vo_id)
        assert entry is not None, f"VOTracker should have entry for {vo_id}"
        assert entry.status.value in ("verified", "failed"), (
            f"VOTracker entry {vo_id} should be verified or failed, got {entry.status}"
        )


# ---------------------------------------------------------------------------
# Test 6: get_function_context — function with callees (parse_integer)
# ---------------------------------------------------------------------------


async def test_get_function_context_with_callees_real(project_context):
    """get_function_context on parse_integer: should have callee parse_id_len."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.inspection import get_function_context

    ctx = build_tool_context("parse_integer", project_context)
    logger.info("INPUT: func_name=%s, depth=1", ctx.function_info.name)

    result = await get_function_context(ctx, depth=1)

    logger.info("OUTPUT: function_name=%s", result.function_name)
    logger.info("OUTPUT: callee_layers=%d", len(result.callee_layers))
    for layer in result.callee_layers:
        logger.info("  layer depth=%d: %s", layer["depth"], [f["name"] for f in layer["functions"]])

    assert result.function_name == "parse_integer"
    assert len(result.callee_layers) > 0, "parse_integer should have callees"
    callee_names = [
        f["name"]
        for layer in result.callee_layers
        for f in layer["functions"]
    ]
    assert "parse_id_len" in callee_names, (
        f"parse_integer should call parse_id_len, got: {callee_names}"
    )


# ---------------------------------------------------------------------------
# Test 7: verify_callsite_preconditions — parse_integer → parse_id_len
# ---------------------------------------------------------------------------


async def test_verify_callsite_preconditions_real(project_context):
    """verify_callsite_preconditions for parse_integer calling parse_id_len.

    The tool refuses to verify against an unpublished callee, so we
    pre-publish parse_id_len's spec directly on the registry entry.
    """
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.agent.registry import AgentEntry
    from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
    from verdict.tools.verify.callsite import verify_callsite_preconditions

    ctx = build_tool_context("parse_integer", project_context)

    # Register parse_id_len with a published spec so the call-site tool
    # will actually run (instead of returning the 'not published' no-op).
    callee_info = FunctionInfo(
        name="parse_id_len",
        source_file=project_context.source_file,
        source_code="",
    )
    callee_entry = AgentEntry(
        agent_id="agent_parse_id_len",
        function_info=callee_info,
    )
    callee_entry.published_spec = FunctionSpec(
        function_name="parse_id_len",
        preconditions=[
            "requires \\valid_read(buf + (0 .. len - 1));",
            "requires len > 0;",
        ],
        version=1,
        status=SpecStatus.PUBLISHED,
    )
    ctx.registry.register(callee_entry)

    logger.info(
        "INPUT: caller=%s, callee=parse_id_len", ctx.function_info.name,
    )

    result = await verify_callsite_preconditions(ctx, "parse_id_len")

    logger.info("OUTPUT: total_count=%d", result.total_count)
    logger.info("OUTPUT: verified_count=%d", result.verified_count)
    logger.info("OUTPUT: all_verified=%s", result.all_verified)
    for pr in result.results:
        logger.info("  precondition=%s, verified=%s", pr.precondition[:60], pr.verified)
    logger.info("OUTPUT: summary=%s", result.summary)

    # Pipeline ran — we expect results
    assert result.total_count > 0, "Should have precondition results"


# ---------------------------------------------------------------------------
# Test 8: apply_precondition_weakening — parse_id_len cascade
# ---------------------------------------------------------------------------


async def test_weakening_cascade_real(project_context):
    """apply_precondition_weakening on parse_id_len: affected_callers should include parse_integer."""
    from tests.helpers.real_context_builder import build_tool_context
    from verdict.tools.verify.weakening import apply_precondition_weakening

    ctx = build_tool_context("parse_id_len", project_context)
    logger.info("INPUT: func_name=%s", ctx.function_info.name)

    result = await apply_precondition_weakening(
        ctx,
        original_precondition="requires buf_len > 0;",
        weakened_precondition="requires buf_len >= 0;",
    )

    logger.info("OUTPUT: success=%s", result.success)
    logger.info("OUTPUT: affected_callers=%s", result.affected_callers)
    logger.info("OUTPUT: affected_callees=%s", result.affected_callees)

    assert result.success is True
    assert "parse_integer" in result.affected_callers, (
        f"parse_id_len parents should include parse_integer, got: {result.affected_callers}"
    )

    # VO re-verification fields should be populated
    logger.info("OUTPUT: affected_vo_nodes=%s", result.affected_vo_nodes)
    logger.info("OUTPUT: affected_vo_details=%s", result.affected_vo_details)
    logger.info("OUTPUT: vo_reverification_summary=%s", result.vo_reverification_summary)
    assert isinstance(result.affected_vo_nodes, list)
    assert isinstance(result.affected_vo_details, dict)
    assert isinstance(result.vo_reverification_summary, str)
