"""VO re-verification after precondition weakening.

Re-verifies VERIFIED RTE and call-site VOs after a precondition is
weakened/removed, and re-runs the worklist algorithm for surviving specs.
"""

from __future__ import annotations

import logging
from dataclasses import dataclass, field
from typing import Any

from verdict.tools._cil_builder import (
    build_annotated_cil_file,
    build_annotated_cil_file_with_labeled_callee,
)
from verdict.tools._wp_runner import (
    _WP_TIMEOUT_DEFAULT,
    parse_wp_callsite_result,
    parse_wp_result,
    run_wp_callsite_targeted,
    run_wp_rte_targeted,
)
from verdict.tools._worklist import worklist_verify_specs


@dataclass
class VOReVerificationResult:
    """Result of re-verifying internal VOs after precondition weakening."""

    rte_transitions: dict[str, dict[str, Any]] = field(default_factory=dict)
    callsite_transitions: dict[str, dict[str, Any]] = field(default_factory=dict)
    total_reverified: int = 0
    rte_newly_failed: int = 0
    callsite_newly_failed: int = 0
    log: str = ""


async def reverify_internal_vos(
    project_ctx: Any,
    func_name: str,
    surviving_annotations: dict[str, Any],
    callee_contracts: dict[str, dict[str, Any]],
    agent_id: str,
    vo_tracker: Any,
    registry: Any,
    timeout: int | None = None,
    logger: logging.Logger | None = None,
) -> VOReVerificationResult:
    """Re-verify VERIFIED RTE and call-site VOs after precondition weakening.

    Step A: Re-verify VERIFIED RTE VOs using -wp-prop targeting.
    Step B: Re-verify VERIFIED call-site VOs (parent-child model).
    Step C: Build log and return result.
    """
    if logger is None:
        logger = logging.getLogger(f"verdict.reverify_vos.{agent_id}")
    if timeout is None:
        timeout = _WP_TIMEOUT_DEFAULT

    from verdict.core.vo_tracker import VOStatus

    result = VOReVerificationResult()
    log_lines: list[str] = ["=== VO Re-verification After Weakening ===", ""]

    # -- Step A: Re-verify VERIFIED RTE VOs --
    verified_rte_ids = [
        vo_id for vo_id, entry in vo_tracker.rte_vos.items()
        if entry.status == VOStatus.VERIFIED
    ]

    if verified_rte_ids:
        log_lines.append(f"--- RTE Assertions ({len(verified_rte_ids)} VERIFIED) ---")
        logger.info("Re-verifying %d VERIFIED RTE VOs", len(verified_rte_ids))

        # Build CIL with surviving annotations
        cil_path = build_annotated_cil_file(
            project_ctx, func_name, surviving_annotations,
            callee_contracts, agent_id,
        )

        # Run WP targeting all verified RTE VOs
        success, wp_output, command = await run_wp_rte_targeted(
            cil_path, project_ctx.include_paths, verified_rte_ids, timeout,
        )

        if success:
            parsed = parse_wp_result(wp_output)
            annotation_status = parsed.get("annotation_status", {})

            rte_still_verified = 0
            for vo_id in verified_rte_ids:
                still_ok = annotation_status.get(vo_id, False)
                result.total_reverified += 1
                if still_ok:
                    rte_still_verified += 1
                    result.rte_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "verified",
                    }
                    log_lines.append(f"  {vo_id}: STILL VERIFIED")
                    logger.info("  %s: STILL VERIFIED", vo_id)
                else:
                    result.rte_newly_failed += 1
                    result.rte_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "failed",
                    }
                    vo_tracker.mark_failed(vo_id)
                    log_lines.append(f"  {vo_id}: FAILED (was VERIFIED)")
                    logger.info("  %s: FAILED (was VERIFIED)", vo_id)

            rte_summary = (
                f"RTE summary: {rte_still_verified} still verified, "
                f"{result.rte_newly_failed} newly failed"
            )
            log_lines.append(rte_summary)
            logger.info(rte_summary)
        else:
            # WP execution failed -- mark all as failed
            for vo_id in verified_rte_ids:
                result.total_reverified += 1
                result.rte_newly_failed += 1
                result.rte_transitions[vo_id] = {
                    "status_before": "verified",
                    "status_after": "failed",
                }
                vo_tracker.mark_failed(vo_id)
            log_lines.append(f"RTE: WP execution failed, all {len(verified_rte_ids)} marked FAILED")
    else:
        log_lines.append("--- RTE Assertions (0 VERIFIED — nothing to re-verify) ---")

    log_lines.append("")

    # -- Step B: Re-verify VERIFIED call-site child VOs --
    # Collect VERIFIED leaf call-site VOs (children + childless parents).
    verified_cs = {
        vo_id: entry for vo_id, entry in vo_tracker.leaf_call_site_vos().items()
        if entry.status == VOStatus.VERIFIED
    }

    if verified_cs:
        log_lines.append(f"--- Call-Site Preconditions ({len(verified_cs)} VERIFIED leaves) ---")
        logger.info("Re-verifying %d VERIFIED call-site leaf VOs", len(verified_cs))

        # Group by callee
        by_callee: dict[str, list[tuple[str, Any]]] = {}
        for vo_id, entry in verified_cs.items():
            by_callee.setdefault(entry.related_function, []).append((vo_id, entry))

        cs_still_verified = 0
        for callee_name, vo_entries in by_callee.items():
            callee_spec = registry.read_spec(callee_name)
            if not callee_spec or not callee_spec.preconditions:
                for vo_id, entry in vo_entries:
                    result.total_reverified += 1
                    cs_still_verified += 1
                    result.callsite_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "verified",
                        "callee": callee_name,
                        "reason": "no preconditions",
                    }
                    log_lines.append(
                        f"  {vo_id} (-> {callee_name}): no preconditions -> STILL VERIFIED"
                    )
                continue

            callee_contract = {
                "preconditions": callee_spec.preconditions,
                "postconditions": callee_spec.postconditions,
                "loop_contracts": callee_spec.loop_contracts,
                "assigns": callee_spec.assigns,
            }

            # Sibling callee contracts must stay visible: a call-site proof
            # that needs an earlier sibling's frame postcondition would
            # spuriously fail without them. This mirrors the environment
            # verify_callsite_preconditions originally proved the VO in.
            sibling_cc = {
                name: cc
                for name, cc in callee_contracts.items()
                if name != callee_name
            }
            cil_path = build_annotated_cil_file_with_labeled_callee(
                project_ctx, func_name, surviving_annotations,
                callee_name, callee_contract, agent_id,
                extra_callee_contracts=sibling_cc,
            )

            success, wp_output, command = await run_wp_callsite_targeted(
                cil_path, project_ctx.include_paths, timeout,
            )

            if success:
                parsed = parse_wp_callsite_result(
                    wp_output,
                    expected_preconditions=callee_spec.preconditions,
                )
            else:
                parsed = []

            # Snapshot VERIFIED states before update
            pre_states = {
                vo_id: entry.status for vo_id, entry in vo_entries
            }

            # Use the same matching logic as verify_callsite_preconditions:
            # convert parsed dicts to PreconditionResult and delegate to
            # _update_child_vos for unified (line, index) matching.
            from verdict.tools.result_types import PreconditionResult
            from verdict.tools.verify._shared import _update_child_vos

            pr_results = [
                PreconditionResult(
                    precondition=pr.get("precondition", ""),
                    verified=pr.get("verified", False),
                    obligation=pr.get("obligation", ""),
                    call_site_line=pr.get("call_site_line", 0),
                    precondition_index=pr.get("precondition_index", 0),
                )
                for pr in parsed
            ]

            if not success or not pr_results:
                # WP failed — mark all VERIFIED leaves for this callee as FAILED
                for vo_id, entry in vo_entries:
                    result.total_reverified += 1
                    result.callsite_newly_failed += 1
                    result.callsite_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "failed",
                        "callee": callee_name,
                    }
                    vo_tracker.mark_failed(vo_id)
                    log_lines.append(f"  {vo_id} (-> {callee_name}): WP failed -> FAILED")
                continue

            _update_child_vos(vo_tracker, callee_name, pr_results)

            # Record transitions
            for vo_id, entry in vo_entries:
                result.total_reverified += 1
                new_status = entry.status
                if new_status == VOStatus.VERIFIED:
                    cs_still_verified += 1
                    result.callsite_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "verified",
                        "callee": callee_name,
                    }
                    log_lines.append(f"  {vo_id} (-> {callee_name}): STILL VERIFIED")
                    logger.info("  %s (-> %s): STILL VERIFIED", vo_id, callee_name)
                else:
                    result.callsite_newly_failed += 1
                    result.callsite_transitions[vo_id] = {
                        "status_before": "verified",
                        "status_after": "failed",
                        "callee": callee_name,
                    }
                    log_lines.append(f"  {vo_id} (-> {callee_name}): FAILED (was VERIFIED)")
                    logger.info("  %s (-> %s): FAILED", vo_id, callee_name)

        cs_summary = (
            f"Call-site summary: {cs_still_verified} still verified, "
            f"{result.callsite_newly_failed} newly failed"
        )
        log_lines.append(cs_summary)
        logger.info(cs_summary)
    else:
        log_lines.append("--- Call-Site Preconditions (0 VERIFIED — nothing to re-verify) ---")

    # -- Step C: Summary --
    total_still = result.total_reverified - result.rte_newly_failed - result.callsite_newly_failed
    total_failed = result.rte_newly_failed + result.callsite_newly_failed
    log_lines.append("")
    log_lines.append(f"=== TOTAL: {total_still} still verified, {total_failed} newly failed ===")

    result.log = "\n".join(log_lines)
    logger.info(
        "VO re-verification complete: %d still verified, %d newly failed",
        total_still, total_failed,
    )
    return result


async def reverify_specs_after_weakening(
    project_ctx: Any,
    func_name: str,
    weakened_annotations: dict[str, Any],
    callee_contracts: dict[str, dict[str, Any]],
    agent_id: str,
    timeout: int | None = None,
) -> dict[str, Any]:
    """Re-verify after precondition weakening using the worklist algorithm.

    Delegates to :func:`worklist_verify_specs` so that both
    ``verify_function_contract`` and ``apply_precondition_weakening``
    share the same verification logic.
    """
    result = await worklist_verify_specs(
        project_ctx, func_name, weakened_annotations,
        callee_contracts, agent_id, timeout,
    )
    return {
        "removed_loop_contracts": result.failed_loop_contracts,
        "removed_postconditions": result.failed_postconditions,
        "removed_assigns": result.failed_assigns,
        "wp_runs": result.wp_runs,
        "worklist_result": result,
    }
