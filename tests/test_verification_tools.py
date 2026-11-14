"""Unit tests for agent-facing verification tools (mock fallback path)."""

import pytest
from verdict.core.types import FunctionInfo
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext
from verdict.core.vo_tracker import VOTracker
from verdict.tools.verify.contract import verify_function_contract
from verdict.tools.verify.inspection import check_acsl_syntax, get_function_context
from verdict.tools.verify.weakening import apply_precondition_weakening
from verdict.tools.verify.rte import verify_rte_assertions
from verdict.tools.verify.callsite import verify_callsite_preconditions
from verdict.tools.result_types import CallsiteVerificationResult
import logging


@pytest.fixture
def tool_ctx():
    """ToolContext with no project_context (mock fallback).

    Registers `helper` with a pre-built published_spec so that
    verify_callsite_preconditions can run against it (the tool refuses
    to touch unpublished callees). The soft-unpublished path has its
    own dedicated test.
    """
    from verdict.core.types import FunctionSpec, SpecStatus

    info = FunctionInfo(name="foo", source_file="test.c", source_code="int foo(){return 1;}")
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_foo", function_info=info))

    helper_info = FunctionInfo(
        name="helper",
        source_file="test.c",
        source_code="int helper(int x, int y) { return x + y; }",
    )
    helper_entry = AgentEntry(agent_id="agent_helper", function_info=helper_info)
    helper_entry.published_spec = FunctionSpec(
        function_name="helper",
        preconditions=["requires x > 0;", "requires y >= 0;"],
        postconditions=["ensures \\result >= 0;"],
        version=1,
        status=SpecStatus.PUBLISHED,
    )
    reg.register(helper_entry)

    bus = AsyncQueueBus()
    return ToolContext(
        agent_id="agent_foo",
        function_info=info,
        registry=reg,
        message_bus=bus,
        logger=logging.getLogger("test"),
    )


@pytest.mark.asyncio
async def test_verify_function_contract_mock(tool_ctx):
    result = await verify_function_contract(tool_ctx, {"preconditions": ["x>0"]})
    assert result.success is True
    assert result.feedback == "Mock: all goals proven."


@pytest.mark.asyncio
async def test_check_acsl_syntax_mock(tool_ctx):
    result = await check_acsl_syntax(tool_ctx, {"preconditions": ["x>0"]})
    assert result.valid is True


@pytest.mark.asyncio
async def test_malformed_annotation_shapes_rejected(tool_ctx):
    """Wrong JSON shapes (e.g. loop_contracts as a list) must produce a
    clear error instead of an AttributeError deep in the CIL builder."""
    res = await check_acsl_syntax(
        tool_ctx, {"loop_contracts": ["loop invariant i >= 0;"]}
    )
    assert res.valid is False
    assert "loop_contracts" in res.errors[0].message

    r2 = await verify_function_contract(
        tool_ctx, {"preconditions": "requires x > 0;"}
    )
    assert r2.success is False
    assert "Malformed annotations" in r2.feedback


@pytest.mark.asyncio
async def test_get_function_context_mock(tool_ctx):
    result = await get_function_context(tool_ctx)
    assert result.function_name == "foo"
    assert result.source_code == "int foo(){return 1;}"


@pytest.mark.asyncio
async def test_apply_precondition_weakening_mock(tool_ctx):
    result = await apply_precondition_weakening(tool_ctx, "x>0", "x>=0")
    assert result.success is True
    assert result.affected_vo_nodes == []
    assert result.affected_vo_details == {}
    assert result.vo_reverification_summary == ""


@pytest.mark.asyncio
async def test_verify_rte_assertions_mock(tool_ctx):
    result = await verify_rte_assertions(tool_ctx, ["rte_1", "rte_3"])
    assert result.all_verified is True
    assert result.verified_count == 2
    assert result.total_count == 2
    assert "rte_1" in result.results
    assert "rte_3" in result.results
    assert result.results["rte_1"].verified is True
    assert result.results["rte_3"].verified is True
    assert "Mock" in result.summary


@pytest.mark.asyncio
async def test_verify_rte_assertions_updates_vo_tracker(tool_ctx):
    tracker = VOTracker()
    from verdict.core.vo_tracker import VOEntry
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="test"))
    tracker.add_rte_vo(VOEntry(vo_id="rte_3", vo_type="rte", description="test"))
    tool_ctx.vo_tracker = tracker

    result = await verify_rte_assertions(tool_ctx, ["rte_1", "rte_3"])
    assert result.all_verified is True
    from verdict.core.vo_tracker import VOStatus
    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED
    assert tracker.rte_vos["rte_3"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_verify_callsite_preconditions_mock(tool_ctx):
    result = await verify_callsite_preconditions(tool_ctx, "helper")
    assert result.all_verified is True
    assert result.verified_count == 2
    assert result.total_count == 2
    assert len(result.results) == 2
    assert result.results[0].precondition == "requires x > 0;"
    assert result.results[0].verified is True
    assert "Mock" in result.summary


@pytest.mark.asyncio
async def test_verify_callsite_preconditions_updates_vo_tracker(tool_ctx):
    """verify_callsite_preconditions should update VOTracker for matching call-site VOs."""
    tracker = VOTracker()
    from verdict.core.vo_tracker import VOEntry, VOStatus
    tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_1",
        vo_type="call_site",
        description="Call-site check: foo → helper",
        related_function="helper",
    ))
    tool_ctx.vo_tracker = tracker

    result = await verify_callsite_preconditions(tool_ctx, "helper")
    assert result.all_verified is True
    assert tracker.call_site_vos["call_site_1"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_verify_callsite_preconditions_uses_registry_contracts(tool_ctx):
    """The target's preconditions always come from the callee's published
    spec in the registry — there is no agent-supplied override."""
    result = await verify_callsite_preconditions(tool_ctx, "helper")
    assert result.all_verified is True
    assert result.total_count == 2  # from the fixture's published helper


@pytest.mark.asyncio
async def test_verify_callsite_updates_child_vos(tool_ctx):
    """When child VOs exist, verification updates them individually."""
    tracker = VOTracker()
    from verdict.core.vo_tracker import VOEntry, VOStatus

    # Parent call-site VO at line 14
    tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_1",
        vo_type="call_site",
        description="Call-site check: foo -> helper",
        related_function="helper",
        call_site_line=14,
    ))
    # Build child VOs for 2 preconditions
    tracker.rebuild_callsite_children(
        "call_site_1",
        ["requires x > 0;", "requires y >= 0;"],
    )
    tool_ctx.vo_tracker = tracker

    result = await verify_callsite_preconditions(tool_ctx, "helper")
    assert result.all_verified is True
    # Children individually verified
    assert tracker.call_site_vos["call_site_1_0"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["call_site_1_1"].status == VOStatus.VERIFIED
    # Parent status auto-derived
    assert tracker.call_site_vos["call_site_1"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_verify_callsite_prunes_stale_child_on_precondition_removal(tool_ctx):
    """Regression (x509 parse_integer): a callee that REMOVED a precondition
    must not leave a stale child VO that strands the parent.

    The caller built children against the callee's OLD precondition set; the
    callee then republished with fewer preconditions. verify_callsite_-
    preconditions must align the child set to the callee's CURRENT published
    spec (pruning the orphan) instead of leaving it PENDING — which previously
    forced the agent to mark_vo_as_bug on the orphan.
    """
    from verdict.core.vo_tracker import VOEntry, VOStatus

    tracker = VOTracker()
    tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_1",
        vo_type="call_site",
        description="Call-site check: foo -> helper",
        related_function="helper",
        call_site_line=14,
    ))
    # Caller built children against the callee's OLD spec (3 preconditions);
    # index 2 is the one the callee will drop. The fixture's published helper
    # carries only 2 preconditions (the third already removed).
    tracker.rebuild_callsite_children(
        "call_site_1",
        ["requires x > 0;", "requires y >= 0;", "requires x + y < 100;"],
    )
    assert "call_site_1_2" in tracker.call_site_vos  # stale child present
    tool_ctx.vo_tracker = tracker

    result = await verify_callsite_preconditions(tool_ctx, "helper")

    assert result.all_verified is True
    # The orphan child for the removed precondition is pruned...
    assert "call_site_1_2" not in tracker.call_site_vos
    assert tracker.call_site_vos["call_site_1"].child_vo_ids == [
        "call_site_1_0", "call_site_1_1",
    ]
    # ...and the survivors + parent settle as verified (no undischargeable orphan).
    assert tracker.call_site_vos["call_site_1_0"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["call_site_1_1"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["call_site_1"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_verify_callsite_prunes_all_children_when_callee_drops_all_preconditions(tool_ctx):
    """A callee that dropped ALL preconditions: existing children are removed
    and the call site settles as verified, not stranded by orphans."""
    from verdict.core.types import FunctionSpec, SpecStatus
    from verdict.core.vo_tracker import VOEntry, VOStatus

    helper_entry = tool_ctx.registry.get_by_function("helper")
    helper_entry.published_spec = FunctionSpec(
        function_name="helper",
        preconditions=[],
        postconditions=["ensures \\result >= 0;"],
        version=2,
        status=SpecStatus.PUBLISHED,
    )

    tracker = VOTracker()
    tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_1",
        vo_type="call_site",
        description="Call-site check: foo -> helper",
        related_function="helper",
        call_site_line=14,
    ))
    tracker.rebuild_callsite_children(
        "call_site_1", ["requires x > 0;", "requires y >= 0;"],
    )
    assert tracker.call_site_vos["call_site_1"].child_vo_ids  # children present
    tool_ctx.vo_tracker = tracker

    result = await verify_callsite_preconditions(tool_ctx, "helper")

    assert result.all_verified is True
    assert tracker.call_site_vos["call_site_1"].child_vo_ids == []
    assert "call_site_1_0" not in tracker.call_site_vos
    assert tracker.call_site_vos["call_site_1"].status == VOStatus.VERIFIED


# ── Batch mode tests ──


@pytest.fixture
def batch_tool_ctx():
    """ToolContext with two published callees for batch-mode testing."""
    from verdict.core.types import FunctionSpec, SpecStatus

    info = FunctionInfo(
        name="caller",
        source_file="test.c",
        source_code="int caller(){return helper(1,2) + adder(3);}",
        callees=["helper", "adder"],
    )
    reg = AgentRegistry()
    reg.register(AgentEntry(agent_id="agent_caller", function_info=info))

    helper_info = FunctionInfo(
        name="helper", source_file="test.c",
        source_code="int helper(int x, int y) { return x + y; }",
    )
    helper_entry = AgentEntry(agent_id="agent_helper", function_info=helper_info)
    helper_entry.published_spec = FunctionSpec(
        function_name="helper",
        preconditions=["requires x > 0;", "requires y >= 0;"],
        postconditions=["ensures \\result >= 0;"],
        version=1,
        status=SpecStatus.PUBLISHED,
    )
    reg.register(helper_entry)

    adder_info = FunctionInfo(
        name="adder", source_file="test.c",
        source_code="int adder(int a) { return a + 1; }",
    )
    adder_entry = AgentEntry(agent_id="agent_adder", function_info=adder_info)
    adder_entry.published_spec = FunctionSpec(
        function_name="adder",
        preconditions=["requires a >= 0;"],
        postconditions=["ensures \\result > 0;"],
        version=1,
        status=SpecStatus.PUBLISHED,
    )
    reg.register(adder_entry)

    bus = AsyncQueueBus()
    return ToolContext(
        agent_id="agent_caller",
        function_info=info,
        registry=reg,
        message_bus=bus,
        logger=logging.getLogger("test_batch"),
    )


@pytest.mark.asyncio
async def test_batch_single_string_backward_compat(batch_tool_ctx):
    """Single string callee_function still returns CallsiteVerificationResult."""
    result = await verify_callsite_preconditions(batch_tool_ctx, "helper")
    # Must be a CallsiteVerificationResult dataclass, not a dict
    assert isinstance(result, CallsiteVerificationResult)
    assert result.all_verified is True
    assert result.callee_function == "helper"
    assert result.total_count == 2


@pytest.mark.asyncio
async def test_batch_list_returns_dict(batch_tool_ctx):
    """List of callees returns a dict with per-callee results."""
    result = await verify_callsite_preconditions(
        batch_tool_ctx, ["helper", "adder"],
    )
    assert isinstance(result, dict)
    assert "helper" in result
    assert "adder" in result
    # Each value is a dict (serialized CallsiteVerificationResult)
    assert isinstance(result["helper"], dict)
    assert isinstance(result["adder"], dict)


@pytest.mark.asyncio
async def test_batch_per_callee_results_correct(batch_tool_ctx):
    """Each callee in batch results contains correct PreconditionResult entries."""
    result = await verify_callsite_preconditions(
        batch_tool_ctx, ["helper", "adder"],
    )
    # helper has 2 preconditions
    helper_r = result["helper"]
    assert helper_r["all_verified"] is True
    assert helper_r["total_count"] == 2
    assert helper_r["callee_function"] == "helper"
    assert len(helper_r["results"]) == 2
    assert helper_r["results"][0]["precondition"] == "requires x > 0;"
    assert helper_r["results"][1]["precondition"] == "requires y >= 0;"

    # adder has 1 precondition
    adder_r = result["adder"]
    assert adder_r["all_verified"] is True
    assert adder_r["total_count"] == 1
    assert adder_r["callee_function"] == "adder"
    assert len(adder_r["results"]) == 1
    assert adder_r["results"][0]["precondition"] == "requires a >= 0;"


@pytest.mark.asyncio
async def test_batch_single_element_list(batch_tool_ctx):
    """A list with one element still returns a dict (batch format)."""
    result = await verify_callsite_preconditions(batch_tool_ctx, ["helper"])
    assert isinstance(result, dict)
    assert "helper" in result
    assert result["helper"]["all_verified"] is True


@pytest.mark.asyncio
async def test_batch_with_unpublished_callee(batch_tool_ctx):
    """Batch gracefully handles a callee that has no published spec."""
    result = await verify_callsite_preconditions(
        batch_tool_ctx, ["helper", "nonexistent"],
    )
    assert isinstance(result, dict)
    # helper verified normally
    assert result["helper"]["all_verified"] is True
    # nonexistent gets the soft-skip
    assert "no published spec" in result["nonexistent"]["summary"]
    assert result["nonexistent"]["all_verified"] is False


# ---------------------------------------------------------------------------
# Draft proof context: single assume source + VO reset on context change
# ---------------------------------------------------------------------------


def _spec(name="foo", pre=None, post=None, loops=None, cc=None, assumed=None):
    from verdict.core.types import FunctionSpec
    return FunctionSpec(
        function_name=name,
        preconditions=pre or [],
        postconditions=post or [],
        loop_contracts=loops or {},
        assigns=[],
        callee_contracts=cc or {},
        assumed_callees=assumed or [],
    )


def _tracker_with_verified_vos():
    from verdict.core.vo_tracker import VOEntry
    tracker = VOTracker()
    tracker.add_rte_vo(VOEntry(vo_id="rte_1", vo_type="rte", description="r"))
    tracker.add_call_site_vo(VOEntry(
        vo_id="cs_1", vo_type="call_site", description="c",
        related_function="helper",
    ))
    tracker.mark_verified("rte_1")
    tracker.mark_verified("cs_1")
    return tracker


def test_draft_assumes_reads_draft_and_filters():
    """The assume set comes from the draft; names without a hypothesis
    entry degrade to the published-spec path instead of erroring."""
    from verdict.tools.verify._shared import _draft_assumes

    assert _draft_assumes(None) == []
    spec = _spec(
        cc={"helper": {"postconditions": ["ensures \\result >= 0;"]}},
        assumed=["helper", "ghost"],
    )
    assert _draft_assumes(spec) == ["helper"]


def test_reset_vos_on_precondition_change():
    from verdict.core.vo_tracker import VOStatus
    from verdict.tools.verify._shared import _reset_vos_on_context_change

    tracker = _tracker_with_verified_vos()
    prev = _spec(pre=["requires x > 0;"])
    new = _spec(pre=["requires x >= 0;"])
    assert _reset_vos_on_context_change(tracker, prev, new) == 2
    assert tracker.rte_vos["rte_1"].status == VOStatus.PENDING
    assert tracker.call_site_vos["cs_1"].status == VOStatus.PENDING


def test_no_reset_on_postcondition_only_change():
    """Own postconditions are proof goals, not hypotheses — strengthening
    them must not invalidate RTE/call-site proofs."""
    from verdict.core.vo_tracker import VOStatus
    from verdict.tools.verify._shared import _reset_vos_on_context_change

    tracker = _tracker_with_verified_vos()
    prev = _spec(pre=["requires x > 0;"], post=["ensures \\result == 0;"])
    new = _spec(pre=["requires x > 0;"], post=["ensures \\result >= 0;"])
    assert _reset_vos_on_context_change(tracker, prev, new) == 0
    assert tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED
    assert tracker.call_site_vos["cs_1"].status == VOStatus.VERIFIED


def test_reset_on_assume_set_change():
    """Discharging (or adding) an assumption changes the proof world —
    every RTE/call-site proof must be re-established in the new one."""
    from verdict.core.vo_tracker import VOStatus
    from verdict.tools.verify._shared import _reset_vos_on_context_change

    cc = {"helper": {"postconditions": ["ensures \\result >= 0;"]}}
    tracker = _tracker_with_verified_vos()
    prev = _spec(pre=["requires x > 0;"], cc=cc, assumed=["helper"])
    new = _spec(pre=["requires x > 0;"], cc=cc, assumed=[])
    assert _reset_vos_on_context_change(tracker, prev, new) == 2
    assert tracker.rte_vos["rte_1"].status == VOStatus.PENDING


@pytest.mark.asyncio
async def test_update_my_spec_draft_preserves_proof_context(tool_ctx):
    """A manual draft write cannot launder the assume record or hypothesis
    away — update_my_spec carries them over from the existing draft and
    resets VOs when the annotations changed the proof context."""
    from verdict.core.types import SpecStatus
    from verdict.core.vo_tracker import VOStatus
    from verdict.tools.runtime.spec_io import update_my_spec

    cc = {"helper": {"postconditions": ["ensures \\result >= 0;"]}}
    # Simulate verify_function_contract's draft save.
    await tool_ctx.registry.write_spec(
        "agent_foo",
        _spec(pre=["requires x > 0;"], cc=cc, assumed=["helper"]),
    )
    tool_ctx.vo_tracker = _tracker_with_verified_vos()

    # Agent writes a draft directly with different preconditions and no
    # bookkeeping fields.
    manual = _spec(pre=["requires x >= 0;"])
    manual.status = SpecStatus.DRAFT
    result = await update_my_spec(tool_ctx, manual)
    assert result["success"] is True

    working = tool_ctx.registry.read_working_spec("foo")
    assert working.callee_contracts == cc
    assert working.assumed_callees == ["helper"]
    # Precondition changed → previously verified VOs reset.
    assert tool_ctx.vo_tracker.rte_vos["rte_1"].status == VOStatus.PENDING
    assert tool_ctx.vo_tracker.call_site_vos["cs_1"].status == VOStatus.PENDING


def test_resolver_records_unpublished_hypotheses(tool_ctx):
    """A hypothesis used for an UNPUBLISHED callee is an assumption: the
    resolver reports it so contract.py records it in assumed_callees."""
    from verdict.tools.verify._shared import _resolve_callee_contracts_for_wp

    cc = {"ghost": {"postconditions": ["ensures \\result <= 16;"]}}
    resolved, overridden, hypothesized, dropped = _resolve_callee_contracts_for_wp(
        agent_cc=cc,
        callee_names=["ghost", "helper"],
        registry=tool_ctx.registry,
        assume_callee=None,
    )
    assert hypothesized == ["ghost"]
    assert dropped == []
    assert resolved["ghost"]["postconditions"] == ["ensures \\result <= 16;"]
    # helper is published → resolved from the registry, not hypothesized.
    assert "helper" in resolved and "helper" not in hypothesized


@pytest.mark.asyncio
async def test_update_my_spec_rejects_malformed_draft_shapes(tool_ctx):
    """The registry write path must reject malformed annotation shapes —
    they would otherwise poison later CIL builds from the draft."""
    from verdict.core.types import FunctionSpec, SpecStatus
    from verdict.tools.runtime.spec_io import update_my_spec

    bad = FunctionSpec(
        function_name="foo",
        loop_contracts={"foo_loop_0": "loop invariant i >= 0;"},  # str, not list
        status=SpecStatus.DRAFT,
    )
    result = await update_my_spec(tool_ctx, bad)
    assert result["success"] is False
    assert "loop_contracts" in result["message"]


def test_factory_oversize_fallback_is_valid_json():
    import json as _json
    from verdict.tools.factory import _MAX_RESULT_CHARS, _trim_long_strings

    # Simulate the fallback branch: many long fields survive trimming.
    payload = {f"k{i}": "x" * 60_000 for i in range(9)}
    trimmed = _trim_long_strings(payload, 60_000)
    text = _json.dumps(trimmed)
    assert len(text) > _MAX_RESULT_CHARS  # would trigger the fallback
    fallback = _json.dumps({
        "truncated": True,
        "original_chars": len(text),
        "message": "m",
        "head": text[:4000],
    })
    parsed = _json.loads(fallback)  # must be valid JSON
    assert parsed["truncated"] is True
