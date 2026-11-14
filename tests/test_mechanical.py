"""Unit tests for the internal mechanical layer."""

import pytest
from unittest.mock import AsyncMock, MagicMock, patch
from verdict.tools._mechanical import extract_context, compute_weakening_effects
from verdict.tools._wp_runner import parse_wp_result
from verdict.tools._worklist import _label_contract, _generate_contract_ids
from verdict.tools._vo_reverifier import VOReVerificationResult, reverify_internal_vos


def _make_node(name: str, source: str = "", children=None, parents=None):
    """Build a minimal mock ExtendedCallGraphNode."""
    node = MagicMock()
    node.func_name = name
    node.source_code = source
    node.loops = []
    node.pre_conditions = []
    node.post_conditions = []
    node.loop_contracts = {}
    node.assigns = []
    node.children = children or []
    node.parents = parents or []
    node.global_vars_used = []
    return node


def test_extract_context_missing_function():
    result = extract_context({}, "nonexistent")
    assert "error" in result


def test_extract_context_basic():
    child = _make_node("helper", "int helper(int x){return x;}")
    root = _make_node("foo", "int foo(){return helper(1);}", children=[child])
    graph = {"foo": root, "helper": child}

    result = extract_context(graph, "foo", depth=1)
    assert result["function_name"] == "foo"
    assert result["source_code"] == root.source_code
    assert isinstance(result["callee_layers"], list)


def test_compute_weakening_effects_missing():
    result = compute_weakening_effects({}, "nope")
    assert "error" in result


def test_compute_weakening_effects_basic():
    caller = _make_node("main")
    callee = _make_node("helper")
    node = _make_node("foo", children=[callee], parents=[caller])
    graph = {"foo": node}

    result = compute_weakening_effects(graph, "foo")
    assert "main" in result["affected_callers"]
    assert "helper" in result["affected_callees"]


def test_parse_wp_result_empty():
    result = parse_wp_result("")
    assert not result["success"]
    assert result["total_goals"] == 0


# ---------------------------------------------------------------------------
# Contract labeling tests
# ---------------------------------------------------------------------------


def test_label_contract_loop_invariant():
    result = _label_contract("loop invariant 0 <= i <= n;", "loop_0_0")
    assert result == "loop invariant loop_0_0: 0 <= i <= n;"


def test_label_contract_loop_assigns():
    result = _label_contract("loop assigns i, ret;", "loop_0_1")
    assert result == "loop assigns loop_0_1: i, ret;"


def test_label_contract_loop_variant():
    result = _label_contract("loop variant n - i;", "loop_0_2")
    assert result == "loop variant loop_0_2: n - i;"


def test_label_contract_ensures():
    result = _label_contract("ensures \\result == 0 || \\result == 1;", "postcondition_0")
    assert result == "ensures postcondition_0: \\result == 0 || \\result == 1;"


def test_label_contract_assigns():
    result = _label_contract("assigns x, y;", "assign_0")
    assert result == "assigns assign_0: x, y;"


def test_label_contract_assigns_nothing_unchanged():
    result = _label_contract("assigns \\nothing;", "assign_0")
    assert result == "assigns \\nothing;"


def test_label_contract_unknown_keyword():
    result = _label_contract("foobar;", "test_0")
    assert result is None


def test_generate_contract_ids():
    annotations = {
        "loop_contracts": {
            "loop_0": ["loop invariant i >= 0;", "loop assigns i;"],
            "loop_1": ["loop invariant j >= 0;"],
        },
        "postconditions": ["ensures \\result >= 0;"],
        "assigns": ["assigns \\nothing;"],
    }
    c2id, id2c = _generate_contract_ids(annotations)

    # Check loop contracts
    assert ("loop_contracts", "loop_0", "loop invariant i >= 0;") in c2id
    assert ("loop_contracts", "loop_0", "loop assigns i;") in c2id
    assert ("loop_contracts", "loop_1", "loop invariant j >= 0;") in c2id

    # Check postconditions
    assert ("postconditions", "", "ensures \\result >= 0;") in c2id

    # Check assigns
    assert ("assigns", "", "assigns \\nothing;") in c2id

    # Check bidirectional mapping
    for key, cid in c2id.items():
        assert id2c[cid] == key

    # Check total count
    assert len(c2id) == 5


# ---------------------------------------------------------------------------
# reverify_internal_vos tests
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_reverify_internal_vos_rte_transitions():
    """Test that reverify_internal_vos correctly transitions RTE VOs."""
    from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker

    vo_tracker = VOTracker()
    vo_tracker.add_rte_vo(VOEntry(
        vo_id="rte_1", vo_type="rte",
        description="RTE 1", related_function="foo",
        status=VOStatus.VERIFIED,
    ))
    vo_tracker.add_rte_vo(VOEntry(
        vo_id="rte_2", vo_type="rte",
        description="RTE 2", related_function="foo",
        status=VOStatus.VERIFIED,
    ))
    vo_tracker.add_rte_vo(VOEntry(
        vo_id="rte_3", vo_type="rte",
        description="RTE 3", related_function="foo",
        status=VOStatus.PENDING,  # Not VERIFIED — should be skipped
    ))

    registry = MagicMock()
    project_ctx = MagicMock()
    project_ctx.include_paths = []
    project_ctx.work_dir = "/tmp"

    surviving = {
        "preconditions": [],
        "postconditions": [],
        "loop_contracts": {},
        "assigns": [],
    }

    # Mock: rte_1 stays verified, rte_2 fails
    with patch("verdict.tools._vo_reverifier.build_annotated_cil_file", return_value="/tmp/test.c"), \
         patch("verdict.tools._vo_reverifier.run_wp_rte_targeted", new_callable=AsyncMock) as mock_wp, \
         patch("verdict.tools._vo_reverifier.parse_wp_result") as mock_parse:

        mock_wp.return_value = (True, "wp output", ["-wp"])
        mock_parse.return_value = {
            "annotation_status": {"rte_1": True, "rte_2": False},
        }

        result = await reverify_internal_vos(
            project_ctx, "foo", surviving, {}, "agent_foo",
            vo_tracker, registry,
        )

    assert result.total_reverified == 2
    assert result.rte_newly_failed == 1
    assert "rte_1" in result.rte_transitions
    assert result.rte_transitions["rte_1"]["status_after"] == "verified"
    assert "rte_2" in result.rte_transitions
    assert result.rte_transitions["rte_2"]["status_after"] == "failed"
    # VOTracker should be updated
    assert vo_tracker.rte_vos["rte_1"].status == VOStatus.VERIFIED
    assert vo_tracker.rte_vos["rte_2"].status == VOStatus.FAILED
    # rte_3 (PENDING) should not be touched
    assert vo_tracker.rte_vos["rte_3"].status == VOStatus.PENDING
    assert "rte_3" not in result.rte_transitions
    assert result.log != ""


@pytest.mark.asyncio
async def test_reverify_internal_vos_callsite_transitions():
    """Test that reverify_internal_vos correctly handles call-site VOs."""
    from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker

    vo_tracker = VOTracker()
    vo_tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_1", vo_type="call_site",
        description="CS 1", related_function="helper",
        status=VOStatus.VERIFIED,
    ))

    # Mock registry: callee has preconditions
    registry = MagicMock()
    callee_spec = MagicMock()
    callee_spec.preconditions = ["requires x > 0;"]
    callee_spec.postconditions = []
    callee_spec.loop_contracts = {}
    callee_spec.assigns = []
    registry.read_spec.return_value = callee_spec

    project_ctx = MagicMock()
    project_ctx.include_paths = []
    project_ctx.work_dir = "/tmp"

    surviving = {
        "preconditions": [],
        "postconditions": [],
        "loop_contracts": {},
        "assigns": [],
    }

    with patch("verdict.tools._vo_reverifier.build_annotated_cil_file_with_labeled_callee", return_value="/tmp/test.c"), \
         patch("verdict.tools._vo_reverifier.run_wp_callsite_targeted", new_callable=AsyncMock) as mock_wp, \
         patch("verdict.tools._vo_reverifier.parse_wp_callsite_result") as mock_parse:

        mock_wp.return_value = (True, "wp output", ["-wp"])
        # All children pass
        mock_parse.return_value = [{"precondition": "requires x > 0;", "verified": True}]

        result = await reverify_internal_vos(
            project_ctx, "foo", surviving, {}, "agent_foo",
            vo_tracker, registry,
        )

    assert result.total_reverified == 1
    assert result.callsite_newly_failed == 0
    assert "call_site_1" in result.callsite_transitions
    assert result.callsite_transitions["call_site_1"]["status_after"] == "verified"
    assert vo_tracker.call_site_vos["call_site_1"].status == VOStatus.VERIFIED


@pytest.mark.asyncio
async def test_reverify_internal_vos_callsite_child_fails():
    """Test parent call-site VO fails when any child precondition fails."""
    from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker

    vo_tracker = VOTracker()
    vo_tracker.add_call_site_vo(VOEntry(
        vo_id="call_site_2", vo_type="call_site",
        description="CS 2", related_function="helper",
        status=VOStatus.VERIFIED,
    ))

    registry = MagicMock()
    callee_spec = MagicMock()
    callee_spec.preconditions = ["requires x > 0;", "requires y >= 0;"]
    callee_spec.postconditions = []
    callee_spec.loop_contracts = {}
    callee_spec.assigns = []
    registry.read_spec.return_value = callee_spec

    project_ctx = MagicMock()
    project_ctx.include_paths = []
    project_ctx.work_dir = "/tmp"

    surviving = {
        "preconditions": [],
        "postconditions": [],
        "loop_contracts": {},
        "assigns": [],
    }

    with patch("verdict.tools._vo_reverifier.build_annotated_cil_file_with_labeled_callee", return_value="/tmp/test.c"), \
         patch("verdict.tools._vo_reverifier.run_wp_callsite_targeted", new_callable=AsyncMock) as mock_wp, \
         patch("verdict.tools._vo_reverifier.parse_wp_callsite_result") as mock_parse:

        mock_wp.return_value = (True, "wp output", ["-wp"])
        # Second child fails
        mock_parse.return_value = [
            {"precondition": "requires x > 0;", "verified": True},
            {"precondition": "requires y >= 0;", "verified": False},
        ]

        result = await reverify_internal_vos(
            project_ctx, "foo", surviving, {}, "agent_foo",
            vo_tracker, registry,
        )

    assert result.total_reverified == 1
    assert result.callsite_newly_failed == 1
    assert result.callsite_transitions["call_site_2"]["status_after"] == "failed"
    assert vo_tracker.call_site_vos["call_site_2"].status == VOStatus.FAILED


@pytest.mark.asyncio
async def test_reverify_internal_vos_empty():
    """Test reverify_internal_vos when no VOs are VERIFIED."""
    from verdict.core.vo_tracker import VOEntry, VOStatus, VOTracker

    vo_tracker = VOTracker()
    vo_tracker.add_rte_vo(VOEntry(
        vo_id="rte_1", vo_type="rte",
        description="RTE 1", related_function="foo",
        status=VOStatus.PENDING,
    ))

    registry = MagicMock()
    project_ctx = MagicMock()
    project_ctx.include_paths = []

    surviving = {
        "preconditions": [],
        "postconditions": [],
        "loop_contracts": {},
        "assigns": [],
    }

    result = await reverify_internal_vos(
        project_ctx, "foo", surviving, {}, "agent_foo",
        vo_tracker, registry,
    )

    assert result.total_reverified == 0
    assert result.rte_newly_failed == 0
    assert result.callsite_newly_failed == 0
    assert result.log != ""
