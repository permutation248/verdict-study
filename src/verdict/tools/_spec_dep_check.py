"""Spec dependency verification (internal).

Verifies an agent-declared spec dependency via Layer-0 sufficiency check:
keep only the declared callee's postconditions (+ all callees' assigns as
upper bound), ablate other callees' postconditions to ``ensures \\true``,
and run WP over all of the caller's proof targets in one call.

Cost: 1 WP call per (caller, callee) declaration.
"""

from __future__ import annotations

import copy
import logging
import re
from collections import defaultdict
from dataclasses import dataclass, field
from typing import Any

from verdict.tools._cil_builder import build_annotated_cil_file
from verdict.tools._wp_runner import parse_wp_proof_status, run_wp_rte_targeted
from verdict.tools._worklist import label_all_contracts


_POST_TOP = ["ensures \\true;"]

_FAILED_GOAL_RE = re.compile(r"\[\s*-\s*\]\s*(.*?)$", re.MULTILINE)
_SKIP_GOALS = {"Default behavior", "Termination-condition (generated)"}


def _parse_failed_goals(
    wp_output: str, source_basename: str | None = None
) -> list[str]:
    """Extract descriptions of unproven goals from WP -report output.

    The report section lists every property in the translation unit,
    including Frama-C libc contract clauses (``result_uchar_of_eof``,
    ``correct_div``, ...) that were never proof targets. When
    ``source_basename`` is given, keep only entries that reference the
    generated CIL file, so the caller's real failures are not buried
    under libc noise. Dedupes while preserving order.
    """
    failed: list[str] = []
    seen: set[str] = set()
    for m in _FAILED_GOAL_RE.finditer(wp_output):
        desc = m.group(1).strip()
        if not desc or desc in seen:
            continue
        if any(skip in desc for skip in _SKIP_GOALS):
            continue
        if source_basename is not None and source_basename not in desc:
            continue
        seen.add(desc)
        failed.append(desc)
    return failed


# ---------------------------------------------------------------------------
# Structured sufficiency-failure parser
# ---------------------------------------------------------------------------

@dataclass
class SufficiencyFailure:
    """One failed goal from a sufficiency check, classified by type."""
    kind: str                       # "spec" | "rte" | "callsite"
    label: str                      # "postcondition_0" / "rte_3" / "precondition_test"
    clause: str                     # human-readable: clause text or raw goal desc
    callee: str | None = None       # callsite only: which callee
    precondition_index: int | None = None  # callsite only: 0-based
    call_site_line: int | None = None      # callsite only: source line


# WP report status line patterns (one per goal in the [report] section)
_STATUS_SPEC_RE = re.compile(
    r"\[\s*([^\]]+)\s*\]\s*"
    r"(Post-condition|Assigns|Loop variant|Loop invariant)\s+'([^']+)'"
)
_STATUS_RTE_RE = re.compile(
    r"\[\s*([^\]]+)\s*\]\s*Assertion\s+'((?:rte_\d+|Eva_\d+))(?:,[^']*)?' "
)
_STATUS_CALLSITE_RE = re.compile(
    r"\[\s*([^\]]+)\s*\]\s*Instance of 'Pre-condition '[^']*''"
    r" at (?:call '([^']+)'|initialization of '[^']+')"
    r" \(file [^,]+, line (\d+)\)"
)
# Goal block header for call-site goals (carries callee name)
_GOAL_CALLSITE_RE = re.compile(
    r"Goal Instance of 'Pre-condition '[^']*' in '([^']+)''"
    r" in '([^']+)'"
    r" at (?:call '[^']+'|initialization of '[^']+')"
    r" \(file [^,]+, line (\d+)\)"
)


def parse_sufficiency_failures(
    wp_output: str,
    annotations: dict[str, Any],
    rte_target_ids: list[str],
    callee_preconditions: dict[str, list[str]],
) -> list[SufficiencyFailure]:
    """Parse a sufficiency-check WP output into classified failures.

    This is the dedicated parser for the mixed-target WP run used by
    ``verify_spec_dep``.  It classifies each failed goal into spec / rte /
    callsite and resolves it to a human-readable clause.
    """
    parts = wp_output.split("[report] Computing properties status...")
    goals_part = parts[0]
    report_part = parts[1] if len(parts) > 1 else ""

    postconditions = annotations.get("postconditions", [])
    assigns = annotations.get("assigns", [])
    rte_set = set(rte_target_ids)

    # ── Build call-site goal ordering from Goal blocks ──
    # Goal blocks appear in order: for each (callee, line) group the
    # i-th goal maps to the i-th precondition of that callee.
    callsite_goal_order: list[tuple[str, int]] = []  # (callee, line)
    callsite_goal_counts: dict[tuple[str, int], int] = defaultdict(int)
    for m in _GOAL_CALLSITE_RE.finditer(goals_part):
        callee = m.group(1)
        line = int(m.group(3))
        key = (callee, line)
        callsite_goal_counts[key] += 1
        callsite_goal_order.append(key)

    # ── Parse [report] status lines ──
    failures: list[SufficiencyFailure] = []

    # spec / assigns / loop
    for m in _STATUS_SPEC_RE.finditer(report_part):
        status = m.group(1).strip()
        if status in ("Valid", "Dead", "Partial"):
            continue
        label = m.group(3)
        clause = label  # default
        if label.startswith("postcondition_"):
            idx = int(label.split("_", 1)[1])
            if idx < len(postconditions):
                clause = postconditions[idx]
        elif label.startswith("assign_"):
            idx = int(label.split("_", 1)[1])
            if idx < len(assigns):
                clause = assigns[idx]
        failures.append(SufficiencyFailure(kind="spec", label=label, clause=clause))

    # rte
    for m in _STATUS_RTE_RE.finditer(report_part):
        status = m.group(1).strip()
        if status in ("Valid", "Dead", "Partial"):
            continue
        label = m.group(2)
        if label in rte_set:
            failures.append(SufficiencyFailure(kind="rte", label=label, clause=label))

    # call-site: match status lines in order, map via goal-block ordering
    callsite_counter: dict[tuple[str, int], int] = defaultdict(int)
    # We need the status lines to appear in the same order as goal blocks.
    # The status pattern doesn't carry the callee name, but it carries the
    # call-site line + the callee name is in the 'at call' part.
    for m in _STATUS_CALLSITE_RE.finditer(report_part):
        status = m.group(1).strip()
        callee_from_status = m.group(2) or ""
        line = int(m.group(3))
        key = (callee_from_status, line)
        idx = callsite_counter[key]
        callsite_counter[key] += 1
        if status in ("Valid", "Dead", "Partial"):
            continue
        callee_pres = callee_preconditions.get(callee_from_status, [])
        clause = callee_pres[idx] if idx < len(callee_pres) else f"precondition #{idx}"
        failures.append(SufficiencyFailure(
            kind="callsite",
            label="precondition_test",
            clause=clause,
            callee=callee_from_status,
            precondition_index=idx,
            call_site_line=line,
        ))

    return failures


@dataclass
class SpecDepResult:
    """Detailed result of a spec dependency verification."""
    verified: bool
    proved: int = 0
    total: int = 0
    failed_goals: list[str] = field(default_factory=list)
    structured_failures: list[SufficiencyFailure] = field(default_factory=list)


async def verify_spec_dep(
    project_context: Any,
    func_name: str,
    annotations: dict[str, Any],
    resolved_cc: dict[str, dict[str, Any]],
    declared_callees: set[str],
    rte_target_ids: list[str],
    agent_id: str,
    logger: logging.Logger,
    include_callsites: bool = False,
    callee_preconditions: dict[str, list[str]] | None = None,
    timeout: int = 10,
) -> SpecDepResult:
    """Verify that the declared callees' postconditions are jointly
    sufficient for all of *func_name*'s proof targets.

    Uses Layer-0 sufficiency: keep postconditions for all callees in
    *declared_callees*, ablate all OTHER callees' postconditions to
    ``ensures \\true``. All assigns are retained (upper bound).
    Labels all caller contracts with wp-prop IDs (reusing the worklist's
    ``label_all_contracts``), then runs one WP call over all targets.

    When *include_callsites* is True, ``resolved_cc`` carries each callee's
    preconditions labeled with ``precondition_test``; the caller's call-site
    VOs are then added to the target set, so a call-site goal that needs a
    sibling callee's (ablated) frame postcondition will fail the check —
    pulling that frame postcondition into the dependency. This closes the
    gap where call-site frame dependencies were invisible to sufficiency.
    """
    label_str = "+".join(sorted(declared_callees))
    labeled_annotations, spec_target_ids = label_all_contracts(annotations)
    all_target_ids = spec_target_ids + rte_target_ids
    if include_callsites:
        all_target_ids = all_target_ids + ["precondition_test"]

    if not all_target_ids:
        logger.debug(
            "verify_spec_dep %s/{%s}: no targets, trivially verified",
            func_name, label_str,
        )
        return SpecDepResult(verified=True)

    ablated_cc = copy.deepcopy(resolved_cc)
    for callee_name, cc in ablated_cc.items():
        if callee_name in declared_callees:
            continue
        cc["postconditions"] = list(_POST_TOP)

    cil_path = build_annotated_cil_file(
        project_context,
        func_name,
        labeled_annotations,
        ablated_cc,
        agent_id,
    )

    success, wp_output, _command = await run_wp_rte_targeted(
        cil_path, project_context.include_paths, all_target_ids, timeout=timeout
    )
    if not success:
        logger.debug(
            "verify_spec_dep %s/{%s}: WP invocation failed",
            func_name, label_str,
        )
        return SpecDepResult(verified=False)

    proved, total = parse_wp_proof_status(wp_output)
    all_pass = proved == total and total > 0
    failed_goals = (
        _parse_failed_goals(wp_output, cil_path.split("/")[-1])
        if not all_pass
        else []
    )

    structured: list[SufficiencyFailure] = []
    if not all_pass and callee_preconditions is not None:
        structured = parse_sufficiency_failures(
            wp_output, annotations, rte_target_ids,
            callee_preconditions,
        )

    logger.debug(
        "verify_spec_dep %s/{%s}: %s (sufficiency check %d/%d goals proved)",
        func_name,
        label_str,
        "VERIFIED" if all_pass else "REJECTED",
        proved,
        total,
    )
    return SpecDepResult(
        verified=all_pass,
        proved=proved,
        total=total,
        failed_goals=failed_goals,
        structured_failures=structured,
    )
