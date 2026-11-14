"""Worklist-based verification algorithm.

3-phase verification with per-spec granularity:
  Phase 1: Loop contracts  (context = preconditions only)
  Phase 2: Postconditions  (context = preconditions + verified loop contracts)
  Phase 3: Assigns         (context = preconditions + verified loops + posts)

Each phase uses the worklist algorithm: apply all remaining specs (labeled)
-> test each with -wp-prop -> remove failed -> rebuild -> repeat until
convergence.
"""

from __future__ import annotations

import asyncio
import copy
import logging
from dataclasses import dataclass, field
from typing import Any

from verdict.tools._cil_builder import build_annotated_cil_file
from verdict.tools._wp_runner import (
    wp_output_has_counterexample,
    _run_wp_targeted_async,
    extract_wp_warnings,
    parse_wp_proof_status,
)


def _label_contract(contract: str, contract_id: str) -> str | None:
    """Inject a label into an ACSL clause string for ``-wp-prop`` targeting.

    Transforms::

        'loop invariant expr;'  -> 'loop invariant contract_id: expr;'
        'loop assigns lvals;'   -> 'loop assigns contract_id: lvals;'
        'loop variant expr;'    -> 'loop variant contract_id: expr;'
        'ensures expr;'         -> 'ensures contract_id: expr;'
        'assigns lvals;'        -> 'assigns contract_id: lvals;'
        'assigns \\nothing;'    -> unchanged (uses @assigns for verification)

    Returns *None* if no known keyword is found.
    """
    # assigns \nothing -- cannot embed label; use @assigns as wp-prop target
    if "assigns" in contract and "\\nothing" in contract and "loop" not in contract:
        return contract

    keywords = ["loop invariant", "loop assigns", "loop variant", "ensures", "assigns"]
    for kw in keywords:
        idx = contract.find(kw)
        if idx >= 0:
            after = idx + len(kw)
            return f"{contract[:after]} {contract_id}: {contract[after:].lstrip()}"
    return None


def _generate_contract_ids(
    annotations: dict[str, Any],
) -> tuple[
    dict[tuple[str, str, str], str],
    dict[str, tuple[str, str, str]],
]:
    """Assign unique IDs to all contracts in *annotations*.

    Returns ``(contract_to_id, id_to_contract)`` where keys are
    ``(category, loop_name_or_empty, contract_text)`` and values are
    string IDs like ``loop_0_0``, ``postcondition_0``, ``assign_0``.
    """
    contract_to_id: dict[tuple[str, str, str], str] = {}
    id_to_contract: dict[str, tuple[str, str, str]] = {}

    for loop_name, contracts in annotations.get("loop_contracts", {}).items():
        for i, contract in enumerate(contracts):
            cid = f"{loop_name}_{i}"
            key = ("loop_contracts", loop_name, contract)
            contract_to_id[key] = cid
            id_to_contract[cid] = key

    for i, post in enumerate(annotations.get("postconditions", [])):
        cid = f"postcondition_{i}"
        key = ("postconditions", "", post)
        contract_to_id[key] = cid
        id_to_contract[cid] = key

    for i, assign in enumerate(annotations.get("assigns", [])):
        cid = f"assign_{i}"
        key = ("assigns", "", assign)
        contract_to_id[key] = cid
        id_to_contract[cid] = key

    return contract_to_id, id_to_contract


def label_all_contracts(
    annotations: dict[str, Any],
) -> tuple[dict[str, Any], list[str]]:
    """Label every contract in *annotations* for ``-wp-prop`` targeting.

    Returns ``(labeled_annotations, target_ids)`` where
    *labeled_annotations* is a copy with every clause injected with its
    contract ID, and *target_ids* lists all IDs (suitable for passing
    to ``-wp-prop`` in a single WP call).

    Used by spec-dep verification to check all targets in one shot.
    """
    contract_to_id, id_to_contract = _generate_contract_ids(annotations)
    target_ids = list(id_to_contract.keys())

    labeled: dict[str, Any] = {
        "preconditions": list(annotations.get("preconditions", [])),
        "loop_contracts": {},
        "postconditions": [],
        "assigns": [],
    }

    for loop_name, contracts in annotations.get("loop_contracts", {}).items():
        labeled_loop: list[str] = []
        for contract in contracts:
            key = ("loop_contracts", loop_name, contract)
            cid = contract_to_id.get(key)
            if cid:
                lbl = _label_contract(contract, cid)
                labeled_loop.append(lbl if lbl else contract)
            else:
                labeled_loop.append(contract)
        labeled["loop_contracts"][loop_name] = labeled_loop

    for post in annotations.get("postconditions", []):
        key = ("postconditions", "", post)
        cid = contract_to_id.get(key)
        if cid:
            lbl = _label_contract(post, cid)
            labeled["postconditions"].append(lbl if lbl else post)
        else:
            labeled["postconditions"].append(post)

    for assign in annotations.get("assigns", []):
        key = ("assigns", "", assign)
        cid = contract_to_id.get(key)
        if cid:
            lbl = _label_contract(assign, cid)
            labeled["assigns"].append(lbl if lbl else assign)
        else:
            labeled["assigns"].append(assign)

    return labeled, target_ids


def _rebuild_phase_annotations(
    preconditions: list[str],
    remaining_ids: set[str],
    id_to_contract: dict[str, tuple[str, str, str]],
    contract_to_id: dict[tuple[str, str, str], str],
    phase_name: str,
    context_loop_contracts: dict[str, list[str]] | None = None,
    context_postconditions: list[str] | None = None,
) -> dict[str, Any]:
    """Build annotations dict with labeled remaining contracts for one phase.

    Only contracts whose IDs are in *remaining_ids* are included (labeled).
    Context specs (already-verified from prior phases) are included unlabeled.
    """
    result: dict[str, Any] = {
        "preconditions": list(preconditions),
        "loop_contracts": {},
        "postconditions": [],
        "assigns": [],
    }

    if phase_name == "loop_contracts":
        lc: dict[str, list[str]] = {}
        for cid in remaining_ids:
            cat, loop_name, contract = id_to_contract[cid]
            if cat != "loop_contracts":
                continue
            labeled = _label_contract(contract, cid)
            lc.setdefault(loop_name, []).append(labeled if labeled else contract)
        result["loop_contracts"] = lc

    elif phase_name == "postconditions":
        # Context: verified loop contracts
        if context_loop_contracts:
            result["loop_contracts"] = copy.deepcopy(context_loop_contracts)
        for cid in remaining_ids:
            cat, _, contract = id_to_contract[cid]
            if cat != "postconditions":
                continue
            labeled = _label_contract(contract, cid)
            result["postconditions"].append(labeled if labeled else contract)

    elif phase_name == "assigns":
        # Context: verified loop contracts + verified postconditions
        if context_loop_contracts:
            result["loop_contracts"] = copy.deepcopy(context_loop_contracts)
        if context_postconditions:
            result["postconditions"] = list(context_postconditions)

    return result


@dataclass
class WorklistResult:
    """Result of worklist-based verification with per-spec granularity."""

    verified_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    failed_loop_contracts: dict[str, list[str]] = field(default_factory=dict)
    verified_postconditions: list[str] = field(default_factory=list)
    failed_postconditions: list[str] = field(default_factory=list)
    verified_assigns: list[str] = field(default_factory=list)
    failed_assigns: list[str] = field(default_factory=list)
    wp_runs: list[dict[str, Any]] = field(default_factory=list)
    worklist_log: str = ""


async def _run_worklist_phase(
    project_ctx: Any,
    func_name: str,
    preconditions: list[str],
    callee_contracts: dict[str, dict[str, Any]],
    agent_id: str,
    remaining_ids: list[str],
    id_to_contract: dict[str, tuple[str, str, str]],
    contract_to_id: dict[tuple[str, str, str], str],
    timeout: int,
    logger: logging.Logger,
    phase_name: str,
    context_loop_contracts: dict[str, list[str]] | None = None,
    context_postconditions: list[str] | None = None,
) -> tuple[list[str], list[str], list[dict[str, Any]], list[str]]:
    """Run one worklist phase. Returns (verified_ids, failed_ids, wp_runs, log_lines)."""
    wp_runs: list[dict[str, Any]] = []
    failed_ids: list[str] = []
    log_lines: list[str] = []
    remaining = list(remaining_ids)

    if phase_name == "assigns":
        # Assigns: test each individually (Frama-C merges assigns internally)
        for cid in list(remaining):
            cat, _, contract = id_to_contract[cid]
            is_nothing = "\\nothing" in contract
            logger.info("  Verifying %s: %s", cid, contract[:60])

            # Build CIL with single assign
            ann = _rebuild_phase_annotations(
                preconditions, {cid}, id_to_contract, contract_to_id,
                "assigns", context_loop_contracts, context_postconditions,
            )
            labeled = _label_contract(contract, cid)
            ann["assigns"] = [labeled if labeled else contract]

            cil_path = build_annotated_cil_file(
                project_ctx, func_name, ann, callee_contracts, agent_id,
            )
            wp_target = "@assigns" if is_nothing else cid
            success, wp_output, command = await _run_wp_targeted_async(
                cil_path, project_ctx.include_paths, [wp_target], timeout,
            )
            proved, total = parse_wp_proof_status(wp_output)
            warnings = extract_wp_warnings(wp_output)
            passed = success and proved == total and total > 0
            wp_runs.append({
                "phase": phase_name, "contract_id": cid,
                "proved": proved, "total": total,
                "frama_c_success": success, "warnings": warnings,
            })
            status_str = "PASSED" if passed else "FAILED"
            if not passed and wp_output_has_counterexample(wp_output):
                status_str = "FAILED [Invalid]"
            log_lines.append(
                f"  {cid} ({contract[:60]}): {status_str} [proved {proved}/{total}]"
            )
            logger.debug("%s: %s '%s' [%d/%d]", status_str, phase_name, cid, proved, total)
            if not passed:
                remaining.remove(cid)
                failed_ids.append(cid)

        return remaining, failed_ids, wp_runs, log_lines

    # Loop contracts / postconditions: batch worklist
    iteration = 0
    while remaining:
        iteration += 1
        logger.info("  Iteration %d: %d remaining", iteration, len(remaining))
        log_lines.append(f"--- Iteration {iteration}: {len(remaining)} contracts ---")

        # Build CIL with ALL remaining contracts (labeled)
        ann = _rebuild_phase_annotations(
            preconditions, set(remaining), id_to_contract, contract_to_id,
            phase_name, context_loop_contracts, context_postconditions,
        )
        cil_path = build_annotated_cil_file(
            project_ctx, func_name, ann, callee_contracts, agent_id,
        )

        # Test each contract individually
        newly_failed: list[str] = []
        for cid in list(remaining):
            _, _, contract_text = id_to_contract[cid]
            logger.info("  Verifying %s: %s", cid, contract_text[:60])
            success, wp_output, command = await _run_wp_targeted_async(
                cil_path, project_ctx.include_paths, [cid], timeout,
            )
            proved, total = parse_wp_proof_status(wp_output)
            warnings = extract_wp_warnings(wp_output)
            passed = success and proved == total and total > 0
            wp_runs.append({
                "phase": phase_name, "iteration": iteration,
                "contract_id": cid, "proved": proved, "total": total,
                "frama_c_success": success, "warnings": warnings,
            })
            _, _, contract = id_to_contract[cid]
            status_str = "PASSED" if passed else "FAILED"
            if not passed and wp_output_has_counterexample(wp_output):
                status_str = "FAILED [Invalid]"
            log_lines.append(
                f"  {cid} ({contract[:60]}): {status_str} [proved {proved}/{total}]"
            )
            logger.debug("%s: %s '%s' [%d/%d]", status_str, phase_name, cid, proved, total)
            if not passed:
                newly_failed.append(cid)

        if not newly_failed:
            logger.info("  0 failed -> converged")
            log_lines.append(f"  -> 0 failed -> converged")
            break

        for cid in newly_failed:
            remaining.remove(cid)
            failed_ids.append(cid)
        logger.info("  %d failed, %d remaining", len(newly_failed), len(remaining))
        log_lines.append(
            f"  -> {len(newly_failed)} failed, {len(remaining)} remaining"
        )

    return remaining, failed_ids, wp_runs, log_lines


async def worklist_verify_specs(
    project_ctx: Any,
    func_name: str,
    annotations: dict[str, Any],
    callee_contracts: dict[str, dict[str, Any]],
    agent_id: str,
    timeout: int | None = None,
    logger: logging.Logger | None = None,
) -> WorklistResult:
    """3-phase worklist verification with per-spec granularity.

    Phase 1: Loop contracts  (context = preconditions only)
    Phase 2: Postconditions  (context = preconditions + verified loop contracts)
    Phase 3: Assigns         (context = preconditions + verified loops + posts)

    Each phase uses the worklist algorithm from the reference implementation:
    apply all remaining specs (labeled) -> test each with ``-wp-prop`` ->
    remove failed -> rebuild -> repeat until convergence.

    Used by both ``verify_function_contract`` and ``apply_precondition_weakening``.
    """
    if logger is None:
        logger = logging.getLogger(f"verdict.worklist.{agent_id}")
    if timeout is None:
        from verdict.tools._wp_runner import _WP_TIMEOUT_DEFAULT
        timeout = _WP_TIMEOUT_DEFAULT

    log_lines: list[str] = []
    all_wp_runs: list[dict[str, Any]] = []

    contract_to_id, id_to_contract = _generate_contract_ids(annotations)
    preconditions = list(annotations.get("preconditions", []))

    verified_lc: dict[str, list[str]] = {}
    failed_lc: dict[str, list[str]] = {}
    verified_post: list[str] = []
    failed_post: list[str] = []
    verified_assigns: list[str] = []
    failed_assigns: list[str] = []

    # -- Phase 1: Loop contracts --
    lc_ids = [
        cid for (cat, _, _), cid in contract_to_id.items()
        if cat == "loop_contracts"
    ]
    if lc_ids:
        log_lines.append(f"=== Phase 1: Loop Contracts ({len(lc_ids)} contracts) ===")
        logger.info("Phase 1: Verifying %d loop contracts", len(lc_ids))

        v_ids, f_ids, runs, phase_log = await _run_worklist_phase(
            project_ctx, func_name, preconditions, callee_contracts,
            agent_id, list(lc_ids), id_to_contract, contract_to_id,
            timeout, logger, "loop_contracts",
        )
        all_wp_runs.extend(runs)
        log_lines.extend(phase_log)

        for cid in v_ids:
            _, loop_name, contract = id_to_contract[cid]
            verified_lc.setdefault(loop_name, []).append(contract)
        for cid in f_ids:
            _, loop_name, contract = id_to_contract[cid]
            failed_lc.setdefault(loop_name, []).append(contract)

        log_lines.append(
            f"Phase 1 result: {len(v_ids)} verified, {len(f_ids)} failed"
        )

    # -- Phase 2: Postconditions --
    post_ids = [
        cid for (cat, _, _), cid in contract_to_id.items()
        if cat == "postconditions"
    ]
    if post_ids:
        log_lines.append(f"\n=== Phase 2: Postconditions ({len(post_ids)} contracts) ===")
        logger.info("Phase 2: Verifying %d postconditions", len(post_ids))

        v_ids, f_ids, runs, phase_log = await _run_worklist_phase(
            project_ctx, func_name, preconditions, callee_contracts,
            agent_id, list(post_ids), id_to_contract, contract_to_id,
            timeout, logger, "postconditions",
            context_loop_contracts=verified_lc,
        )
        all_wp_runs.extend(runs)
        log_lines.extend(phase_log)

        for cid in v_ids:
            _, _, contract = id_to_contract[cid]
            verified_post.append(contract)
        for cid in f_ids:
            _, _, contract = id_to_contract[cid]
            failed_post.append(contract)

        log_lines.append(
            f"Phase 2 result: {len(v_ids)} verified, {len(f_ids)} failed"
        )

    # -- Phase 3: Assigns --
    assign_ids = [
        cid for (cat, _, _), cid in contract_to_id.items()
        if cat == "assigns"
    ]
    if assign_ids:
        log_lines.append(f"\n=== Phase 3: Assigns ({len(assign_ids)} contracts) ===")
        logger.info("Phase 3: Verifying %d assigns", len(assign_ids))

        v_ids, f_ids, runs, phase_log = await _run_worklist_phase(
            project_ctx, func_name, preconditions, callee_contracts,
            agent_id, list(assign_ids), id_to_contract, contract_to_id,
            timeout, logger, "assigns",
            context_loop_contracts=verified_lc,
            context_postconditions=verified_post,
        )
        all_wp_runs.extend(runs)
        log_lines.extend(phase_log)

        for cid in v_ids:
            _, _, contract = id_to_contract[cid]
            verified_assigns.append(contract)
        for cid in f_ids:
            _, _, contract = id_to_contract[cid]
            failed_assigns.append(contract)

        log_lines.append(
            f"Phase 3 result: {len(v_ids)} verified, {len(f_ids)} failed"
        )

    # Summary
    total_v = (
        sum(len(v) for v in verified_lc.values())
        + len(verified_post) + len(verified_assigns)
    )
    total_f = (
        sum(len(v) for v in failed_lc.values())
        + len(failed_post) + len(failed_assigns)
    )
    log_lines.append(f"\n=== SUMMARY ===")
    log_lines.append(f"Verified: {total_v}/{total_v + total_f}")
    if total_f:
        all_failed_ids = (
            [cid for cid in contract_to_id.values()
             if id_to_contract[cid][0] == "loop_contracts"
             and id_to_contract[cid][2] in
             sum(failed_lc.values(), [])]
            + [cid for cid in contract_to_id.values()
               if id_to_contract[cid][0] == "postconditions"
               and id_to_contract[cid][2] in failed_post]
            + [cid for cid in contract_to_id.values()
               if id_to_contract[cid][0] == "assigns"
               and id_to_contract[cid][2] in failed_assigns]
        )
        log_lines.append(f"Failed: {total_f} ({', '.join(all_failed_ids)})")

    logger.info("Worklist completed: %d/%d verified", total_v, total_v + total_f)

    return WorklistResult(
        verified_loop_contracts=verified_lc,
        failed_loop_contracts=failed_lc,
        verified_postconditions=verified_post,
        failed_postconditions=failed_post,
        verified_assigns=verified_assigns,
        failed_assigns=failed_assigns,
        wp_runs=all_wp_runs,
        worklist_log="\n".join(log_lines),
    )
