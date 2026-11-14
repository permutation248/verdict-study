"""Proof-demand protocol tools.

An agent submits a runtime-owned proof demand to another unit
(`submit_proof_demand`) and resolves demands raised against it
(`resolve_proof_demand`). The orchestrator routes the wakes.
"""

from __future__ import annotations

from verdict.core.types import DemandStatus, ProofDemand
from verdict.tools.context import ToolContext


_ALLOWED_DEMAND_KINDS = {"postcondition", "assigns", "precondition_weakening"}


async def submit_proof_demand(
    ctx: ToolContext,
    target_function: str,
    property_kind: str,
    property_text: str,
    blocking_vo_ids: list[str] | None = None,
    rationale: str | None = None,
) -> ProofDemand:
    """Submit a runtime-owned proof demand for another unit.

    Allowed property_kind values:
    - 'postcondition': request the callee to publish a postcondition.
    - 'assigns': request the callee to publish an assigns clause.
    - 'precondition_weakening': request the callee to weaken or remove
      one of its preconditions (property_text is the specific clause).
      The callee runs apply_precondition_weakening to evaluate the
      cascade and decides whether to accept.

    'precondition' (adding a new precondition) and 'loop_contract' are
    still rejected — adding a precondition only creates a new call-site
    obligation for the caller, and loop contracts are internal.
    """
    if property_kind not in _ALLOWED_DEMAND_KINDS:
        raise ValueError(
            f"property_kind={property_kind!r} is not a valid proof demand. "
            f"Allowed kinds: {sorted(_ALLOWED_DEMAND_KINDS)}. "
            f"'precondition' (add) is rejected because it only adds a new "
            f"call-site obligation to you. Use 'precondition_weakening' to "
            f"request the callee to relax an existing precondition. "
            f"'loop_contract' is internal to the callee body."
        )

    project_ctx = ctx.project_context
    if project_ctx is not None and target_function in project_ctx.extern_functions:
        raise ValueError(
            f"{target_function!r} is an extern environment function "
            f"(prototype only, no body in this project) — it has no "
            f"verification agent and will never publish a spec, so a proof "
            f"demand can never be satisfied. If your proof needs guarantees "
            f"about it, state them yourself as a "
            f"callee_contracts[{target_function!r}] entry "
            f"(postconditions/assigns) when verifying: they are recorded as "
            f"a standing environment axiom and reported with your result."
        )

    demand = await ctx.registry.raise_proof_demand(
        requester_id=ctx.agent_id,
        target_function=target_function,
        property_kind=property_kind,
        property_text=property_text,
        blocking_vo_ids=blocking_vo_ids,
        rationale=rationale,
    )
    ctx.logger.info(
        "Proof demand -> %s: %s '%s'",
        target_function,
        property_kind,
        property_text[:60],
    )
    return demand


async def resolve_proof_demand(
    ctx: ToolContext,
    demand_id: str,
    resolution: str,
    message: str | None = None,
) -> ProofDemand:
    """Resolve an open proof demand as satisfied or rejected."""
    status_map = {
        "satisfied": DemandStatus.SATISFIED,
        "rejected": DemandStatus.REJECTED,
    }
    status = status_map.get(resolution)
    if status is None:
        raise ValueError(f"resolution must be 'satisfied' or 'rejected', got '{resolution}'")

    demand = await ctx.registry.resolve_proof_demand(
        agent_id=ctx.agent_id,
        demand_id=demand_id,
        resolution=status,
        message=message,
    )
    ctx.logger.info("Resolved %s: %s", demand_id, resolution)
    return demand
