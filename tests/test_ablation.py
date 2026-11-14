"""RQ3 coordination ablation (--ablate demands,assume).

Pins the contract from scripts/experiments/rq3/README.md: each ablation
is the absence of a capability, the default config is a no-op, extern
environment axioms are exempt from the assume ablation, and the normal
flow is byte-identical when nothing is ablated.
"""

from __future__ import annotations

import logging

import pytest

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.core.ablation import AblationConfig
from verdict.core.types import FunctionInfo, FunctionSpec, SpecStatus
from verdict.messaging.bus import AsyncQueueBus
from verdict.prompts.function_agent import build_function_agent_prompt
from verdict.tools.context import ToolContext
from verdict.tools.factory import (
    _apply_ablation,
    _input_schema_for,
    _description_for,
)
from verdict.tools.schemas import TOOL_SCHEMAS
from verdict.tools.verify._shared import _resolve_callee_contracts_for_wp
from verdict.tools.verify.contract import verify_function_contract


# ── config parsing ──────────────────────────────────────────────────


def test_parse_empty_is_noop():
    assert AblationConfig.parse(None) == AblationConfig()
    assert AblationConfig.parse("") == AblationConfig()
    assert AblationConfig().active() == []


def test_parse_values():
    assert AblationConfig.parse("demands") == AblationConfig(demands=True)
    assert AblationConfig.parse("assume") == AblationConfig(assume=True)
    both = AblationConfig.parse("demands, assume")
    assert both.demands and both.assume
    assert both.active() == ["demands", "assume"]


def test_parse_rejects_unknown():
    with pytest.raises(ValueError, match="unknown --ablate"):
        AblationConfig.parse("demands,typo")


# ── tool surface ────────────────────────────────────────────────────


def test_apply_ablation_drops_demand_tools():
    defs = [("submit_proof_demand", None, None),
            ("resolve_proof_demand", None, None),
            ("verify_function_contract", None, None)]
    kept = _apply_ablation(defs, AblationConfig(demands=True))
    assert [t[0] for t in kept] == ["verify_function_contract"]
    # No-op without the switch — same list object content.
    assert _apply_ablation(defs, AblationConfig()) == defs


def test_schema_gating_removes_assume_callee():
    baseline = _input_schema_for("verify_function_contract", AblationConfig())
    assert baseline is TOOL_SCHEMAS["verify_function_contract"]
    assert "assume_callee" in baseline["properties"]

    ablated = _input_schema_for(
        "verify_function_contract", AblationConfig(assume=True)
    )
    assert "assume_callee" not in ablated["properties"]
    # The shared schema dict must be untouched (deepcopy, not mutation).
    assert "assume_callee" in TOOL_SCHEMAS["verify_function_contract"]["properties"]


def test_description_annotated_only_under_ablation():
    base = _description_for("verify_function_contract", AblationConfig())
    assert "ABLATION" not in base
    ann = _description_for(
        "verify_function_contract", AblationConfig(demands=True, assume=True)
    )
    assert ann.startswith("ABLATION NOTE")
    assert "`assume_callee` / the draft assume set) are DISABLED" in ann
    assert "proof-demand protocol is DISABLED" in ann


# ── resolver ────────────────────────────────────────────────────────


def _registry_with_caller():
    reg = AgentRegistry()
    caller = FunctionInfo(
        name="caller", source_file="m.c", source_code="",
        callees=["helper", "arch_init"], callers=[],
        extern_callees=["arch_init"],
    )
    helper = FunctionInfo(
        name="helper", source_file="m.c", source_code="",
        callees=[], callers=["caller"],
    )
    reg.register(AgentEntry(agent_id="agent_caller", function_info=caller))
    reg.register(AgentEntry(agent_id="agent_helper", function_info=helper))
    return reg, caller


def test_resolver_baseline_hypothesizes_unpublished():
    reg, _ = _registry_with_caller()
    cc = {"helper": {"postconditions": ["ensures \\result >= 0;"], "assigns": []}}
    resolved, overridden, hyp, dropped = _resolve_callee_contracts_for_wp(
        agent_cc=cc, callee_names=["helper"], registry=reg, assume_callee=None,
    )
    assert "helper" in resolved and hyp == ["helper"] and dropped == []


def test_resolver_ablated_drops_agent_hypotheses_keeps_extern():
    reg, caller = _registry_with_caller()
    cc = {
        "helper": {"postconditions": ["ensures \\result >= 0;"], "assigns": []},
        "arch_init": {"postconditions": [], "assigns": ["assigns \\nothing;"]},
    }
    resolved, _, hyp, dropped = _resolve_callee_contracts_for_wp(
        agent_cc=cc,
        callee_names=["helper", "arch_init"],
        registry=reg,
        assume_callee=None,
        allow_hypotheses=False,
        extern_callees=frozenset(caller.extern_callees),
    )
    assert dropped == ["helper"]
    assert "helper" not in resolved
    # Extern axiom survives the ablation.
    assert hyp == ["arch_init"] and "arch_init" in resolved


@pytest.mark.asyncio
async def test_resolver_ablated_published_specs_still_used():
    reg, caller = _registry_with_caller()
    await reg.write_spec("agent_helper", FunctionSpec(
        function_name="helper",
        postconditions=["ensures \\result >= 0;"],
        status=SpecStatus.PUBLISHED,
    ))
    resolved, _, hyp, dropped = _resolve_callee_contracts_for_wp(
        agent_cc={}, callee_names=["helper"], registry=reg,
        assume_callee=None,
        allow_hypotheses=False,
        extern_callees=frozenset(caller.extern_callees),
    )
    assert resolved["helper"]["postconditions"] == ["ensures \\result >= 0;"]
    assert hyp == [] and dropped == []


def test_resolver_ablated_rejects_assume_set():
    reg, caller = _registry_with_caller()
    with pytest.raises(ValueError, match="disabled in this run"):
        _resolve_callee_contracts_for_wp(
            agent_cc={"helper": {"postconditions": [], "assigns": []}},
            callee_names=["helper"], registry=reg,
            assume_callee=["helper"],
            allow_hypotheses=False,
            extern_callees=frozenset(caller.extern_callees),
        )


# ── contract entry gate ─────────────────────────────────────────────


@pytest.mark.asyncio
async def test_contract_rejects_assume_under_ablation():
    reg, caller = _registry_with_caller()
    ctx = ToolContext(
        agent_id="agent_caller",
        function_info=caller,
        registry=reg,
        message_bus=AsyncQueueBus(),
        logger=logging.getLogger("test"),
        ablation=AblationConfig(assume=True),
    )
    result = await verify_function_contract(
        ctx,
        annotations={"postconditions": ["ensures \\true;"]},
        callee_contracts={"helper": {"postconditions": [], "assigns": []}},
        assume_callee=["helper"],
    )
    assert result.success is False
    assert "disabled in this run" in result.feedback
    # demands still on -> the remedy mentions the demand tool
    assert "submit_proof_demand" in result.feedback


# ── prompt gating ───────────────────────────────────────────────────


def _info():
    return FunctionInfo(
        name="f", source_file="m.c", source_code="int f(void);",
        callees=["g"], callers=[],
    )


def test_prompt_baseline_mentions_both_mechanisms():
    p = build_function_agent_prompt(_info())
    assert "proof demands" in p
    assert "Optimistic Hypothesis" in p
    assert "DISABLED" not in p


def test_prompt_demands_ablated():
    p = build_function_agent_prompt(_info(), ablation=AblationConfig(demands=True))
    assert "proof-demand protocol is DISABLED" in p
    assert "demand round-trip" not in p


def test_prompt_assume_ablated():
    p = build_function_agent_prompt(_info(), ablation=AblationConfig(assume=True))
    assert "Published Contracts Only" in p
    assert "Optimistic Hypothesis" not in p
    assert "optimistically hypothesized" not in p


# ── baseline-equivalence pin ────────────────────────────────────────


def test_prompt_baseline_matches_golden():
    """The default-config system prompt is pinned byte-for-byte.

    The ablation refactor split the prompt into fragments; this golden
    snapshot (verified byte-identical against the pre-refactor builder
    at introduction time) guarantees the baseline arm — and every
    normal run — never drifts when fragments are touched. Regenerate
    deliberately ONLY for intentional prompt changes:
      uv run python -c "...build_function_agent_prompt(golden_info)..."
    """
    from pathlib import Path

    info = FunctionInfo(
        name="golden_fn",
        source_file="golden.c",
        source_code="int golden_fn(int x) { return x + 1; }",
        callees=["callee_a", "callee_b"],
        callers=["caller_z"],
    )
    golden_dir = Path(__file__).parent / "golden"
    assert build_function_agent_prompt(info) == (
        golden_dir / "function_agent_prompt_baseline.txt"
    ).read_text()
    # Debug profile and the empty-neighbors rendering are pinned too —
    # the fragments must not perturb ANY baseline-rendered variant.
    assert build_function_agent_prompt(info, tool_profile="debug") == (
        golden_dir / "function_agent_prompt_baseline_debug.txt"
    ).read_text()
    leaf = FunctionInfo(
        name="golden_leaf",
        source_file="golden.c",
        source_code="int golden_leaf(void) { return 0; }",
        callees=[],
        callers=[],
    )
    assert build_function_agent_prompt(leaf) == (
        golden_dir / "function_agent_prompt_baseline_leaf.txt"
    ).read_text()


def test_description_prepends_notes_on_other_demand_tools():
    """Codex audit finding: tools whose guidance references the demand
    protocol (read_function_spec, mark_vo_as_bug, ...) must carry the
    ablation notice too — and PREPENDED, so it wins over stale prose."""
    abl = AblationConfig(demands=True)
    for tool in ("read_function_spec", "mark_vo_as_bug"):
        d = _description_for(tool, abl)
        assert d.startswith("ABLATION NOTE"), tool
    # Untouched when the description never mentions demands.
    assert not _description_for(
        "check_acsl_syntax", abl
    ).startswith("ABLATION NOTE")
    # And never touched in normal operation.
    for tool in ("read_function_spec", "mark_vo_as_bug"):
        assert not _description_for(tool, AblationConfig()).startswith(
            "ABLATION NOTE"
        )


def test_assume_coupled_tools_get_notice_via_membership():
    """Codex re-audit: synonym wording ("draft hypothesis", "assume
    set") escapes substring detection — coupled tools are listed
    explicitly."""
    abl = AblationConfig(assume=True)
    for tool in ("verify_callsite_preconditions", "read_my_spec",
                 "verify_rte_assertions", "apply_precondition_weakening"):
        assert _description_for(tool, abl).startswith("ABLATION NOTE"), tool
    for tool in ("verify_callsite_preconditions", "read_my_spec"):
        assert not _description_for(tool, AblationConfig()).startswith(
            "ABLATION NOTE"
        )


def test_assume_callee_param_description_demand_free_under_demands():
    """Under --ablate demands (assume ON) the assume_callee parameter
    stays but must not teach proof_demand."""
    schema = _input_schema_for(
        "verify_function_contract", AblationConfig(demands=True)
    )
    desc = schema["properties"]["assume_callee"]["description"]
    assert "proof_demand" not in desc
    assert "hypothesis" in desc
    # Shared dict untouched; baseline identity preserved.
    base = _input_schema_for("verify_function_contract", AblationConfig())
    assert base is TOOL_SCHEMAS["verify_function_contract"]
    assert "proof_demand" in base["properties"]["assume_callee"]["description"]


def test_validator_remedy_demand_aware():
    from verdict.tools.verify._shared import _validate_callee_contracts

    bad = {"x": {"preconditions": ["requires p;"]}}
    with pytest.raises(ValueError, match="submit_proof_demand"):
        _validate_callee_contracts(bad)
    with pytest.raises(ValueError) as exc:
        _validate_callee_contracts(bad, demands_available=False)
    assert "submit_proof_demand" not in str(exc.value)
    assert "cannot request changes" in str(exc.value)


# ── exhaustive surface sweep (the audit closer, kept as a test) ─────


def _swept_arms():
    return {
        "no-demands": AblationConfig(demands=True),
        "no-assume": AblationConfig(assume=True),
        "no-both": AblationConfig(demands=True, assume=True),
    }


def test_no_removed_mechanism_escapes_any_surface():
    """For every arm x every delivered tool: a description that still
    references a removed mechanism (including synonym wording) must
    carry the prepended notice, and schema JSON must not teach the
    mechanism at all. This is the codex-audit closer, made permanent."""
    import json

    from verdict.tools.factory import _DEMAND_TOOLS
    from verdict.tools.schemas import TOOL_DESCRIPTIONS

    problems = []
    for arm, abl in _swept_arms().items():
        for name in TOOL_DESCRIPTIONS:
            if abl.demands and name in _DEMAND_TOOLS:
                continue  # tool absent entirely
            desc = _description_for(name, abl)
            schema_text = json.dumps(_input_schema_for(name, abl)).lower()
            noted = desc.startswith("ABLATION NOTE")
            if abl.demands:
                for kw in ("proof_demand", "proof demand"):
                    if kw in desc and not noted:
                        problems.append(f"{arm}/{name}: desc '{kw}' w/o note")
                    if kw in schema_text:
                        problems.append(f"{arm}/{name}: schema teaches '{kw}'")
            if abl.assume:
                for kw in ("assume_callee", "hypothes", "assume set"):
                    if kw in desc and not noted:
                        problems.append(f"{arm}/{name}: desc '{kw}' w/o note")
                    if kw in schema_text:
                        problems.append(f"{arm}/{name}: schema teaches '{kw}'")
    assert not problems, "\n".join(problems)
