"""Frama-C WP invocation and result parsing.

Provides async wrappers around FramaCRunner for WP verification,
targeted property verification, and WP output parsing.
"""

from __future__ import annotations

import asyncio
import re
from typing import Any


# ── WP output warning extraction ────────────────────────────────────

_WP_WARNING_RE = re.compile(
    r"\[wp\] Warning:.*|"
    r"\[kernel:annot-error\].*|"
    r"\[wp\] No proof obligations|"
    r"\[wp\] .*No goal generated",
    re.MULTILINE,
)


def extract_wp_warnings(wp_output: str) -> list[str]:
    """Extract actionable warnings from raw WP output.

    Keeps only lines that help the agent fix its annotations (missing
    assigns, no goals generated, annotation errors). Discards the
    ~1600 lines of boilerplate per WP call that inflate tool-return size.
    """
    return [m.group(0).strip() for m in _WP_WARNING_RE.finditer(wp_output)]


# A goal is REFUTED (genuinely false, counterexample found) when WP reports
# one of these verdicts — as opposed to merely failing to prove it
# (Timeout/Unknown/Stepout: likely true, the prover just needs more
# hypotheses). Only the refuted case is worth flagging to the agent; for
# everything else "failed" + the obligation is enough.
_REFUTED_STATUSES = frozenset({"Invalid", "Valid_but_dead", "Failed"})


def wp_output_has_counterexample(wp_output: str) -> bool:
    """True iff WP refuted a goal (verdict Invalid/Failed) in this output."""
    return bool(re.search(r"\[\s*(?:Invalid|Failed)\s*\]", wp_output))


# ``User Error`` carries its message on the following indented line, so we
# capture that continuation too. ``aborted`` lines explain why WP bailed.
_WP_ERROR_RE = re.compile(
    r"\[\w+(?::[\w:]+)?\] [^\n]*User Error:[^\n]*(?:\n\s+[^\n]+)?|"
    r"\[\w+\] Plug-in \w+ aborted[^\n]*|"
    r"\[\w+\] Frama-C aborted[^\n]*",
    re.MULTILINE,
)


def extract_wp_errors(wp_output: str) -> list[str]:
    """Extract the meaningful failure lines from a non-zero WP run.

    When WP aborts (e.g. ``Address of logic value`` from ``&param`` on a
    by-value parameter), the real cause is a ``User Error`` / ``aborted``
    line that may appear far down the output — after benign parse-stage
    warnings. Blind truncation hides it; this surfaces it for the agent.
    """
    errors = [
        " ".join(m.group(0).split())
        for m in _WP_ERROR_RE.finditer(wp_output)
    ]
    if errors:
        return errors
    # Fallback: tail of the output (last meaningful lines) rather than head.
    tail = [ln.strip() for ln in wp_output.strip().splitlines() if ln.strip()]
    return tail[-3:]


# ── WP flags ────────────────────────────────────────────────────────
def _wp_core() -> list[str]:
    """Core WP flags. ``-wp-par`` (prover processes per run) comes from the
    shared :mod:`verdict.framac.wp_resources` budget.

    ``-wp-no-init-const`` is ON (2026-06-11 decision, reversing the
    2026-06-10 removal): the x509-full A/B showed initializer axioms
    inflate the heavy tail ~3x on table-adjacent upper layers (algid
    family, top-level parsers: >120s WP calls 20-35 -> 145, single-call
    max 480s -> 1172s) while complete prior runs converged 205-208/209
    WITHOUT initializer visibility. The known-curves/gost value-dependence
    concern stays open — if specific functions prove blocked on const
    VALUES, revisit as a per-benchmark knob rather than a global flip.
    """
    from verdict.framac import wp_resources
    cmd = [
        "-wp", "-wp-prover", "z3,cvc5,alt-ergo",
        "-wp-par", str(wp_resources.procs()),
        "-wp-no-init-const",
    ]
    # Optional proving-semantics override (evaluation-model ablations,
    # e.g. SpecSyn's Typed+var+int+real). Default: no flag → sound Typed.
    if wp_resources.model():
        cmd += ["-wp-model", wp_resources.model()]
    return cmd


_WP_TIMEOUT_DEFAULT = 10
_WP_TIMEOUT_LARGE = 30
_LARGE_DECL_RATIO = 0.5
_LARGE_DECL_MIN_LINES = 500


def compute_wp_timeout(project_context: Any, func_name: str) -> int:
    """Per-function WP timeout based on filtered declaration volume.

    Returns the large timeout (30s) when BOTH:
      1. The function's filtered declarations ≥ 50% of the full set
      2. The filtered declarations ≥ 200 lines
    Otherwise returns the default (10s).
    """
    ordered = getattr(project_context, "ordered_declarations", None)
    if not ordered:
        return _WP_TIMEOUT_DEFAULT
    full_lines = sum(d.count("\n") for d in ordered)
    if full_lines == 0:
        return _WP_TIMEOUT_DEFAULT
    filtered = getattr(project_context, "filtered_declarations", {})
    filt_decls = filtered.get(func_name)
    if not filt_decls:
        return _WP_TIMEOUT_DEFAULT
    filt_lines = sum(d.count("\n") for d in filt_decls)
    if filt_lines >= full_lines * _LARGE_DECL_RATIO and filt_lines >= _LARGE_DECL_MIN_LINES:
        return _WP_TIMEOUT_LARGE
    return _WP_TIMEOUT_DEFAULT


def _build_wp_command(
    timeout: int = _WP_TIMEOUT_DEFAULT,
    wp_prop_targets: list[str] | None = None,
) -> list[str]:
    """Assemble a WP command: core + -wp-print + -wp-timeout + [targets] + -then -report."""
    cmd = _wp_core() + ["-wp-print", "-wp-timeout", str(timeout)]
    if wp_prop_targets:
        cmd += ["-wp-prop", ",".join(wp_prop_targets)]
    cmd += ["-then", "-report"]
    return cmd


async def _run_wp_targeted_async(
    cil_file_path: str,
    include_paths: list[str] | None,
    wp_prop_targets: list[str],
    timeout: int = _WP_TIMEOUT_DEFAULT,
) -> tuple[bool, str, list[str]]:
    """Run Frama-C WP targeting specific properties via -wp-prop."""
    from verdict.framac.runner import FramaCRunner

    runner = FramaCRunner(cil_file_path, include_paths)
    command = _build_wp_command(timeout=timeout, wp_prop_targets=wp_prop_targets)
    success, output = await runner.run_FramaC_async(command)
    return success, output, command


async def run_wp_rte_targeted(
    cil_file_path: str,
    include_paths: list[str] | None,
    vo_ids: list[str],
    timeout: int = _WP_TIMEOUT_DEFAULT,
) -> tuple[bool, str, list[str]]:
    """Run Frama-C WP targeting specific RTE assertions by vo_id."""
    return await _run_wp_targeted_async(
        cil_file_path, include_paths, vo_ids, timeout
    )


async def run_wp_callsite_targeted(
    cil_file_path: str,
    include_paths: list[str] | None,
    timeout: int = _WP_TIMEOUT_DEFAULT,
) -> tuple[bool, str, list[str]]:
    """Target preconditions labeled with 'precondition_test' at call sites."""
    return await _run_wp_targeted_async(
        cil_file_path, include_paths, ["precondition_test"], timeout
    )


def parse_wp_result(wp_output: str) -> dict[str, Any]:
    """Parse WP output into structured verification results."""
    from verdict.framac.wp_result_analyzer import WPResultAnalyzer

    analyzer = WPResultAnalyzer()
    analyzer.set_wp_result(wp_output)
    rte_results = analyzer.analyze_wp_rte_result()

    proven: list[str] = []
    failed: list[str] = []
    annotation_status: dict[str, bool] = {}
    statuses: dict[str, str] = {}
    obligations: dict[str, str] = {}

    for assertion_id, (is_valid, status_str, obligation) in rte_results.items():
        annotation_status[assertion_id] = is_valid
        statuses[assertion_id] = status_str
        obligations[assertion_id] = obligation
        if is_valid:
            proven.append(assertion_id)
        else:
            tag = " [Invalid]" if status_str in _REFUTED_STATUSES else ""
            failed.append(f"{assertion_id} failed{tag}")

    total = len(proven) + len(failed)
    success = len(failed) == 0 and total > 0

    # Core feedback states verified/failed counts only; the per-VO status and
    # full obligation live in the secondary fields (statuses / obligations).
    if success:
        feedback = f"All {total} goals proven."
    elif total == 0:
        feedback = "No verification goals found in WP output."
    else:
        feedback = f"{len(proven)}/{total} goals proven. Failed: " + ", ".join(
            failed[:8]
        )

    return {
        "success": success,
        "proven_goals": proven,
        "failed_goals": failed,
        "total_goals": total,
        "proven_count": len(proven),
        "annotation_status": annotation_status,
        "statuses": statuses,
        "obligations": obligations,
        "feedback": feedback,
    }


def parse_wp_callsite_result(
    wp_output: str,
    expected_preconditions: list[str] | None = None,
) -> list[dict[str, Any]]:
    """Parse per-(call-site, precondition) verification results.

    Returns one dict per (call_site_line, precondition_index) pair::

        {
            "call_site_line": int,      # source line of the call site
            "precondition_index": int,   # 0-based index in callee preconditions
            "precondition": str,         # the ACSL clause text
            "verified": bool,
            "obligation": str,           # human-readable status
        }

    This matches the reference implementation's 4-level indexing:
    ``results[caller][callee][call_site_order][precondition_order]``.

    Structured parse
    ~~~~~~~~~~~~~~~~
    WP emits per (call site × precondition) a Goal block and a
    matching status line. Goals and statuses appear in the same order.
    Within one call site, the i-th goal corresponds to the i-th
    precondition in *expected_preconditions*.

    Fallback
    ~~~~~~~~
    When the structured parse finds no goal blocks, falls back to the
    aggregate ``[wp] Proved goals: N / M`` line. In fallback mode,
    ``call_site_line`` is set to 0 (unknown).

    Returns ``[]`` when nothing can be extracted.
    """
    if not wp_output or "No verification obligation" in wp_output:
        return []

    parts = wp_output.split("[report] Computing properties status...")
    goals_part = parts[0]
    properties_part = parts[1] if len(parts) > 1 else ""

    # ── Structured parse: per-call-site goal BLOCKS + statuses ──
    # Capture the FULL goal block (header + Assume/Prove) up to the 60-dash
    # separator WP emits between goals — same shape WPResultAnalyzer uses for
    # RTE (`extract_detailed_obligations`), just keyed on the call-site
    # 'Pre-condition' header instead of 'Assertion'. A failed precondition can
    # then carry its real verification condition (which hypotheses are in
    # scope, what must be proven) rather than a bare "Unknown at line N".
    goal_block_pattern = re.compile(
        r"Goal Instance of 'Pre-condition '[^']*' in '([^']+)''"   # callee
        r" in '([^']+)'"                                             # caller
        r" at (?:call '[^']+'|initialization of '[^']+')"
        r" \(file [^,]+, line (\d+)\):"                              # call-site line
        r"(.*?)(?=\n-{4,}|\nGoal |\Z)",                             # full body
        re.DOTALL,
    )
    status_pattern = re.compile(
        r"\[\s*([^\]]+)\s*\]\s*Instance of 'Pre-condition '[^']*''"
        r" at [^(]+ \(file [^,]+, line (\d+)\)"
    )

    from collections import defaultdict

    site_order: list[int] = []
    site_goal_blocks: dict[int, list[str]] = defaultdict(list)
    for m in goal_block_pattern.finditer(goals_part):
        line = int(m.group(3))
        if line not in site_goal_blocks:
            site_order.append(line)
        site_goal_blocks[line].append(m.group(0).strip())

    # Keep the RAW WP verdict per goal (Valid / Unknown / Timeout / Invalid),
    # not just a bool. Collapsing the verdict to a bool erased the
    # Unknown/Timeout (provable, needs hypotheses) vs Invalid (counterexample)
    # distinction.
    site_statuses: dict[int, list[str]] = defaultdict(list)
    for m in status_pattern.finditer(properties_part):
        site_statuses[int(m.group(2))].append(m.group(1).strip())

    # Determine N = number of preconditions per call site.
    if expected_preconditions is not None and expected_preconditions:
        n = len(expected_preconditions)
    elif site_goal_blocks:
        n = len(site_goal_blocks[site_order[0]]) if site_order else 0
    else:
        n = 0

    if n > 0 and (site_order or site_statuses):
        # Emit one result per (call_site_line, precondition_index).
        results: list[dict[str, Any]] = []
        for line in site_order or sorted(site_statuses.keys()):
            statuses = site_statuses.get(line, [])
            blocks = site_goal_blocks.get(line, [])
            for i in range(n):
                status = statuses[i] if i < len(statuses) else "Unknown"
                verified = status in ("Valid", "Dead", "Partial")
                pre_text = (
                    expected_preconditions[i]
                    if expected_preconditions and i < len(expected_preconditions)
                    else f"precondition_{i}"
                )
                # Core verdict is verified/failed. Failed carries the full WP
                # obligation block (the VC: hypotheses in scope + goal to
                # prove) so the agent sees what is missing. Only a genuine
                # refutation is additionally flagged [Invalid]; Timeout/Unknown
                # need no tag (the obligation already shows what to strengthen).
                tag = " [Invalid]" if status in _REFUTED_STATUSES else ""
                if verified:
                    obligation = "verified"
                elif i < len(blocks):
                    obligation = f"failed{tag}: {blocks[i]}"
                else:
                    obligation = f"failed{tag} at line {line}"
                results.append(
                    {
                        "call_site_line": line,
                        "precondition_index": i,
                        "precondition": pre_text,
                        "verified": verified,
                        "obligation": obligation,
                    }
                )
        return results

    # ── Fallback: aggregate `[wp] Proved goals: N / M` line ──
    proved, total = parse_wp_proof_status(wp_output)
    if total == 0:
        return []

    all_verified = proved == total
    obligation = f"WP proved {proved}/{total} call-site precondition goals"

    if expected_preconditions:
        return [
            {
                "call_site_line": 0,
                "precondition_index": i,
                "precondition": pre,
                "verified": all_verified,
                "obligation": obligation,
            }
            for i, pre in enumerate(expected_preconditions)
        ]

    return [
        {
            "call_site_line": 0,
            "precondition_index": 0,
            "precondition": "(aggregate)",
            "verified": all_verified,
            "obligation": obligation,
        }
    ]


def parse_wp_proof_status(wp_output: str) -> tuple[int, int]:
    """Parse WP proof status: ``[wp] Proved goals: N / M``.

    Returns ``(proved, total)``.  Returns ``(0, 0)`` if the line is not found.
    """
    match = re.search(r"\[wp\] Proved goals:\s*(\d+)\s*/\s*(\d+)", wp_output)
    if match:
        return int(match.group(1)), int(match.group(2))
    return 0, 0
