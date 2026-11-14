"""Factory: create per-agent MCP server with all tools registered."""

from __future__ import annotations

import asyncio
import dataclasses
import enum
import itertools
import json
import logging
from functools import partial
from typing import Any, Callable

from claude_agent_sdk import SdkMcpTool, create_sdk_mcp_server

from verdict.core.types import FunctionSpec, SpecStatus, ToolProfile
import verdict.tools.context as _ctx
from verdict.tools.context import (
    ToolContext,
    _agent_id_var,
    _completed_seqs,
    _seq_tool_info,
    _tool_call_seq,
)
from verdict.tools.schemas import TOOL_SCHEMAS, TOOL_DESCRIPTIONS


# ---------------------------------------------------------------------------
# Handler adapter: bridge Python handlers ↔ SDK MCP protocol
# ---------------------------------------------------------------------------


_seq_counter = itertools.count()

# The SDK stdio transport rejects any single JSON message over 1 MiB
# ("JSON message exceeded maximum buffer size"), killing the agent's cycle.
# Pathological tool results (e.g. WP obligation dumps for mat4x4_invert's
# polynomial goals) can exceed that. Trim long string fields first (keeps
# the JSON valid and every field present), then hard-cap as a last resort.
_MAX_RESULT_CHARS = 400_000
_MAX_FIELD_CHARS = 60_000


def _trim_long_strings(obj: Any, limit: int) -> Any:
    if isinstance(obj, str) and len(obj) > limit:
        return obj[:limit] + f"... [truncated {len(obj) - limit} chars]"
    if isinstance(obj, dict):
        return {k: _trim_long_strings(v, limit) for k, v in obj.items()}
    if isinstance(obj, (list, tuple)):
        return [_trim_long_strings(v, limit) for v in obj]
    return obj


def _adapt_handler(
    handler: Callable[..., Any],
    deserializers: dict[str, Callable] | None = None,
    tool_name: str = "",
    agent_id: str = "",
) -> Callable[[dict], Any]:
    """Wrap a tool handler for SDK consumption.

    - Unpacks dict args into **kwargs
    - Applies per-param deserializers (e.g. dict → FunctionSpec)
    - Awaits if coroutine, else handles sync
    - Serializes return value to SDK content format
    - Sets _tool_call_seq + _agent_id_var contextvars so logging
      handlers can attribute log records to the owning tool call and
      agent (used by solve mode's PrefixedSink to prefix `▸` lines).
    """
    deserializers = deserializers or {}

    async def adapted(args: dict[str, Any]) -> dict:
        seq = next(_seq_counter)
        seq_token = _tool_call_seq.set(seq)
        agent_token = _agent_id_var.set(agent_id)
        # Store tool info so CLI can print [done] headers
        args_str = json.dumps(args, ensure_ascii=False)
        if len(args_str) > 120:
            args_str = args_str[:117] + "..."
        _seq_tool_info[seq] = (tool_name, args_str)
        try:
            kwargs = dict(args)
            for param, deser in deserializers.items():
                if param in kwargs and kwargs[param] is not None:
                    kwargs[param] = deser(kwargs[param])

            result = handler(**kwargs)
            if asyncio.iscoroutine(result):
                result = await result

            # Serialize return value
            def _default(obj):
                if isinstance(obj, enum.Enum):
                    return obj.value
                raise TypeError(f"Object of type {type(obj).__name__} is not JSON serializable")

            if result is None:
                payload: Any = None
            elif dataclasses.is_dataclass(result) and not isinstance(result, type):
                payload = dataclasses.asdict(result)
            elif isinstance(result, list):
                payload = [
                    dataclasses.asdict(item)
                    if dataclasses.is_dataclass(item) and not isinstance(item, type)
                    else item
                    for item in result
                ]
            else:
                payload = result
            text = json.dumps(payload, default=_default, ensure_ascii=False)
            original_chars = len(text)
            if original_chars > _MAX_RESULT_CHARS:
                payload = _trim_long_strings(payload, _MAX_FIELD_CHARS)
                text = json.dumps(payload, default=_default, ensure_ascii=False)
                if len(text) > _MAX_RESULT_CHARS:
                    # Last resort stays VALID JSON: a minimal envelope with
                    # the head of the oversized result for orientation.
                    text = json.dumps({
                        "truncated": True,
                        "original_chars": original_chars,
                        "message": (
                            "result exceeded the transport size limit even "
                            "after field trimming; re-run with a narrower "
                            "query (fewer VO ids / callees) for details"
                        ),
                        "head": text[:4000],
                    }, ensure_ascii=False)
                logging.getLogger(f"verdict.agent.{agent_id}").info(
                    "tool_result %s trimmed: %d -> %d chars",
                    tool_name, original_chars, len(text),
                )

            logging.getLogger(f"verdict.agent.{agent_id}").debug(
                "tool_result %s (%d chars): %s",
                tool_name, len(text),
                text if len(text) <= 2000 else text[:1997] + "...",
            )
            return {"content": [{"type": "text", "text": text}]}
        finally:
            _completed_seqs.append(seq)
            if _ctx._on_tool_complete is not None:
                _ctx._on_tool_complete(seq)
            _agent_id_var.reset(agent_token)
            _tool_call_seq.reset(seq_token)

    return adapted


# ---------------------------------------------------------------------------
# FunctionSpec deserializer
# ---------------------------------------------------------------------------


def _deserialize_function_spec(data: dict[str, Any]) -> FunctionSpec:
    """Convert a raw dict into a FunctionSpec dataclass."""
    return FunctionSpec(
        function_name=data["function_name"],
        preconditions=data.get("preconditions", []),
        postconditions=data.get("postconditions", []),
        loop_contracts=data.get("loop_contracts", {}),
        assigns=data.get("assigns", []),
        version=data.get("version", 0),
        status=SpecStatus(data.get("status", "draft")),
        based_on_versions=data.get("based_on_versions", {}),
        callsite_pre_deps={
            k: frozenset(v)
            for k, v in (data.get("callsite_pre_deps") or {}).items()
        },
    )


# ---------------------------------------------------------------------------
# MCP server factory
# ---------------------------------------------------------------------------


# The demand protocol is exactly these two tools — ablating it is their
# absence, not a behavioral fork (rq3/README.md).
_DEMAND_TOOLS = ("submit_proof_demand", "resolve_proof_demand")


def _apply_ablation(tool_defs: list, ablation) -> list:
    """Drop the tools belonging to ablated coordination mechanisms."""
    if ablation.demands:
        tool_defs = [t for t in tool_defs if t[0] not in _DEMAND_TOOLS]
    return tool_defs


# The assume_callee parameter description in a demands-ablated (but
# assume-enabled) run: same semantics, no proof_demand reference.
_ASSUME_CALLEE_DESC_NO_DEMANDS = (
    "Opt-out list: callee names to verify under YOUR hypothesis "
    "instead of their published spec. Default empty (published "
    "wins for soundness). Use when a callee has not yet published "
    "the guarantee you need and you want to keep your own "
    "postconditions provable in the meantime — each name in this "
    "list must also appear in `callee_contracts` with your assumed "
    "clauses (include the published clauses you still rely on: your "
    "hypothesis REPLACES the published spec, it is not merged). This "
    "is the ONLY place the assume set is chosen; it is saved into "
    "your draft and inherited by every other verify tool."
)


# Property-description variants for the assume-ablated arm: schema
# field metadata must not teach hypothesis semantics either (audit
# finding: synonym wording in schema fields escaped the description
# sweep).
_CC_PARAM_DESC_NO_ASSUME = (
    "Callee contracts: {callee_name: {postconditions: [...], assigns: "
    "[...]}}. Optimistic entries are DISABLED in this run — entries "
    "for unpublished (non-extern) callees are ignored (published callees "
    "always use their published spec). Extern environment callees are "
    "the exception: entries for them are standing axioms."
)
_UPDATE_SPEC_DESC_NO_ASSUME = (
    "FunctionSpec as a dict with keys: function_name (str), "
    "preconditions (list[str]), postconditions (list[str]), "
    "loop_contracts (dict), assigns (list[str]), "
    "status ('draft' or 'published'). (callee_contracts: managed by "
    "the runtime in this run.)"
)


def _input_schema_for(name: str, ablation) -> dict:
    """Tool input schema, with ablated parameters removed.

    Under ``--ablate assume`` the ``assume_callee`` parameter of
    verify_function_contract is removed from the schema entirely so
    agents never see the option (entry validation also rejects it as
    defense in depth), and hypothesis-teaching FIELD descriptions are
    swapped for ablation-aware variants. Under ``--ablate demands``
    (assume still on) the parameter stays but its description's
    proof_demand reference is replaced — the schema must not teach a
    removed tool.
    """
    schema = TOOL_SCHEMAS[name]
    if name == "verify_function_contract" and (ablation.assume or ablation.demands):
        import copy

        schema = copy.deepcopy(schema)
        props = schema.get("properties", {})
        if ablation.assume:
            props.pop("assume_callee", None)
            if "callee_contracts" in props:
                props["callee_contracts"]["description"] = _CC_PARAM_DESC_NO_ASSUME
        elif ablation.demands:
            prop = props.get("assume_callee")
            if prop is not None:
                prop["description"] = _ASSUME_CALLEE_DESC_NO_DEMANDS
    elif name == "update_my_spec" and ablation.assume:
        import copy

        schema = copy.deepcopy(schema)
        prop = schema.get("properties", {}).get("spec")
        if prop is not None:
            prop["description"] = _UPDATE_SPEC_DESC_NO_ASSUME
    return schema


# Tools whose descriptions discuss the draft hypothesis / assume set in
# prose that doesn't contain the literal "assume_callee" (audit finding:
# substring detection alone misses synonym wording like "draft
# hypothesis" / "assume set").
_ASSUME_COUPLED_TOOLS = frozenset({
    "verify_function_contract",
    "verify_rte_assertions",
    "verify_callsite_preconditions",
    "apply_precondition_weakening",
    "read_my_spec",
    "update_my_spec",
})


def _description_for(name: str, ablation) -> str:
    """Tool description, annotated when related mechanisms are ablated.

    Many descriptions reference the demand tools or ``assume_callee`` as
    part of their guidance. Under an ablation those references would
    instruct agents to use a mechanism that does not exist, so any
    description mentioning a removed mechanism gets a PREPENDED notice
    (prepended, not appended — the contradiction must be resolved before
    the agent reads the stale instruction).
    """
    desc = TOOL_DESCRIPTIONS[name]
    notes = []
    if ablation.demands and (
        "proof_demand" in desc or "proof demand" in desc
    ):
        notes.append(
            "ABLATION NOTE: the proof-demand protocol is DISABLED in this "
            "run — submit_proof_demand/resolve_proof_demand do not exist. "
            "Ignore any mention of them below; work with what callees "
            "publish."
        )
    if ablation.assume and (
        "assume_callee" in desc or name in _ASSUME_COUPLED_TOOLS
    ):
        notes.append(
            "ABLATION NOTE: optimistic callee hypotheses (`assume_callee` "
            "/ the draft assume set) are DISABLED in this run. Ignore any "
            "mention of them below. Verify against published callee "
            "contracts only; callee_contracts entries for unpublished "
            "(non-extern) callees are ignored — wait for the callee's "
            "publish."
        )
    if notes:
        desc = "\n".join(notes) + "\n\n" + desc
    return desc


def create_agent_mcp_server(
    ctx: ToolContext,
    tool_profile: ToolProfile | None = None,
) -> "McpSdkServerConfig":
    """Build per-agent MCP server with tools for the requested profile.

    Returns an McpSdkServerConfig ready for ClaudeAgentOptions.mcp_servers.
    """
    if tool_profile is None:
        tool_profile = ctx.tool_profile

    from verdict.tools.verify.contract import verify_function_contract
    from verdict.tools.verify.inspection import check_acsl_syntax, get_function_context
    from verdict.tools.verify.weakening import apply_precondition_weakening
    from verdict.tools.verify.rte import verify_rte_assertions
    from verdict.tools.verify.callsite import verify_callsite_preconditions
    from verdict.tools.verify.spec_deps import build_spec_deps
    from verdict.tools.runtime.spec_io import (
        read_function_spec,
        read_my_spec,
        update_my_spec,
    )
    from verdict.tools.runtime.proof_demand import (
        resolve_proof_demand,
        submit_proof_demand,
    )
    from verdict.tools.runtime.vo_actions import mark_vo_as_bug
    from verdict.tools.debug.communication import (
        check_inbox,
        send_spec_request,
        send_spec_response,
        notify_spec_update,
    )

    core_tool_defs: list[tuple[str, Callable, dict[str, Callable] | None]] = [
        ("verify_function_contract", partial(verify_function_contract, ctx), None),
        ("check_acsl_syntax", partial(check_acsl_syntax, ctx), None),
        ("get_function_context", partial(get_function_context, ctx), None),
        ("apply_precondition_weakening", partial(apply_precondition_weakening, ctx), None),
        ("verify_rte_assertions", partial(verify_rte_assertions, ctx), None),
        ("verify_callsite_preconditions", partial(verify_callsite_preconditions, ctx), None),
        ("read_function_spec", partial(read_function_spec, ctx), None),
        ("read_my_spec", partial(read_my_spec, ctx), None),
        ("submit_proof_demand", partial(submit_proof_demand, ctx), None),
        ("resolve_proof_demand", partial(resolve_proof_demand, ctx), None),
        ("mark_vo_as_bug", partial(mark_vo_as_bug, ctx), None),
        ("build_spec_deps", partial(build_spec_deps, ctx), None),
        ("update_my_spec", partial(update_my_spec, ctx), {"spec": _deserialize_function_spec}),
    ]
    debug_tool_defs: list[tuple[str, Callable, dict[str, Callable] | None]] = [
        ("check_inbox", partial(check_inbox, ctx), None),
        ("send_spec_request", partial(send_spec_request, ctx), None),
        ("send_spec_response", partial(send_spec_response, ctx), None),
        ("notify_spec_update", partial(notify_spec_update, ctx), None),
    ]
    tool_defs = list(core_tool_defs)
    if tool_profile == "debug":
        tool_defs.extend(debug_tool_defs)
    tool_defs = _apply_ablation(tool_defs, ctx.ablation)

    sdk_tools: list[SdkMcpTool] = []
    for name, handler, deserializers in tool_defs:
        sdk_tools.append(
            SdkMcpTool(
                name=name,
                description=_description_for(name, ctx.ablation),
                input_schema=_input_schema_for(name, ctx.ablation),
                handler=_adapt_handler(
                    handler,
                    deserializers,
                    tool_name=name,
                    agent_id=ctx.agent_id,
                ),
            )
        )

    return create_sdk_mcp_server(
        name=f"verdict-agent-{ctx.agent_id}-{tool_profile}",
        tools=sdk_tools,
    )
