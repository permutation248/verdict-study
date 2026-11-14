"""Autonomous solve CLI mode."""

from __future__ import annotations

import argparse
import asyncio
import logging

from verdict.cli._setup import build_solve_context
from verdict.core.types import AgentResult, VOSummary


def cmd_solve(args: argparse.Namespace) -> None:
    """Run the centralized solve loop to quiescence or timeout."""
    ctx = build_solve_context(args, on_batch_complete=_print_batch_progress)
    orch = ctx.orch
    run_dir = ctx.run_dir
    sink = ctx.sink
    function_names = ctx.function_names

    print(f"Specs: {run_dir}/current_specs.c")
    if args.scope == "functions":
        print(f"Scoped solve for functions: {', '.join(function_names)}")
    else:
        print(f"Solving {len(function_names)} functions...")
    print()

    sink.install_log_capture(
        level=logging.DEBUG if args.verbose else logging.INFO,
    )

    async def _run_with_seed():
        if getattr(args, "resume_from", None):
            import json
            from pathlib import Path

            from verdict.cli._setup import _parse_ablation
            ablation = _parse_ablation(getattr(args, "ablate", None))
            if ablation.active():
                # Seeded specs from a (potentially non-ablated) previous run
                # carry optimistic state — assumed_callees, hypothesis-based
                # cc, restored LOCAL_OK/GLOBAL_OK — that the initial batch
                # then SKIPS re-verifying, bypassing the ablated semantics
                # entirely. An ablation arm must be a fresh run.
                raise SystemExit(
                    "--resume-from is incompatible with --ablate: seeded "
                    "specs carry optimistic state (assumed_callees, "
                    "hypothesis contracts, restored proof states) that "
                    "bypasses the ablated semantics. Start a fresh run."
                )
            prev_path = Path("logs") / args.resume_from / "run.json"
            if not prev_path.exists():
                raise SystemExit(f"Previous run not found: {prev_path}")
            prev_run = json.loads(prev_path.read_text(encoding="utf-8"))
            restore_sessions = not getattr(args, "no_history", False)
            await orch.seed_from_previous_run(prev_run, restore_sessions=restore_sessions)
            print(f"Resumed from {args.resume_from} (history={'yes' if restore_sessions else 'no'})")
        return await orch.run()

    try:
        results = asyncio.run(_run_with_seed())
    finally:
        sink.uninstall_log_capture()
    _print_solve_summary(results, scope=args.scope)


def _format_vo_inline(summary: VOSummary | None) -> str:
    """Compact single-line VO summary for progress/summary rows."""
    if summary is None or summary.total == 0:
        return "no VOs"
    rte_total = (
        summary.rte_verified + summary.rte_pending
        + summary.rte_failed + summary.rte_bug
    )
    cs_total = (
        summary.callsite_verified + summary.callsite_pending
        + summary.callsite_failed + summary.callsite_bug
    )
    parts = [
        f"rte {summary.rte_verified}/{rte_total}",
        f"cs {summary.callsite_verified}/{cs_total}",
    ]
    extras: list[str] = []
    bug_total = summary.rte_bug + summary.callsite_bug
    fail_total = summary.rte_failed + summary.callsite_failed
    if bug_total:
        extras.append(f"{bug_total} bug")
    if fail_total:
        extras.append(f"{fail_total} failed")
    if extras:
        parts.append("(" + ", ".join(extras) + ")")
    if summary.complete:
        parts.append("✓")
    return " ".join(parts)


def _print_batch_progress(phase: str, snapshots: list[AgentResult]) -> None:
    """Called by the orchestrator after each initial/reactivation batch."""
    if not snapshots:
        return
    for result in sorted(snapshots, key=lambda item: item.function_name):
        proof = result.proof_state.value if result.proof_state is not None else "-"
        vo_str = _format_vo_inline(result.vo_summary)
        limit_marker = " [cycle limit]" if result.cycle_limit_reached else ""
        print(
            f"  [{phase}] {result.function_name}: "
            f"cycle={result.cycle_count} proof={proof} "
            f"VOs: {vo_str}{limit_marker}"
        )


def _print_solve_summary(results, scope: str) -> None:
    """Render a compact end-of-run summary."""
    print()
    print("Solve Summary")
    print("=" * 72)

    for result in sorted(results, key=lambda item: item.function_name):
        proof = result.proof_state.value if result.proof_state is not None else "-"
        version = result.spec.version if result.spec is not None else 0
        vo_str = _format_vo_inline(result.vo_summary)
        limit_marker = " [cycle limit]" if result.cycle_limit_reached else ""
        if result.settled_assumes:
            limit_marker += (
                f" [conditional: assumes {', '.join(result.settled_assumes)}]"
            )
        if result.extern_axioms:
            limit_marker += (
                f" [env axioms: {', '.join(result.extern_axioms)}]"
            )
        line = (
            f"{result.function_name}: "
            f"proof={proof} "
            f"version=v{version} "
            f"cycles={result.cycle_count} "
            f"VOs: {vo_str}{limit_marker}"
        )
        if result.error:
            line += f"\n    error: {result.error}"
        print(line)

        summary = result.vo_summary
        if summary is not None:
            if summary.failed_ids:
                print(f"    failed VOs: {', '.join(summary.failed_ids)}")
            if summary.pending_ids:
                print(f"    pending VOs: {', '.join(summary.pending_ids)}")
            if summary.bugs:
                for vo_id, reason in summary.bugs.items():
                    reason_str = f": {reason}" if reason else ""
                    print(f"    bug {vo_id}{reason_str}")

    failures = [result for result in results if result.error]
    global_ok = [
        result for result in results
        if result.proof_state is not None and result.proof_state.value == "global_ok"
    ]
    local_ok = [
        result for result in results
        if result.proof_state is not None and result.proof_state.value == "local_ok"
    ]
    cycle_limited = [r for r in results if r.cycle_limit_reached]
    bugged = [
        r for r in results
        if r.vo_summary is not None
        and (r.vo_summary.rte_bug + r.vo_summary.callsite_bug) > 0
    ]
    total_bugs = sum(
        r.vo_summary.rte_bug + r.vo_summary.callsite_bug for r in bugged
    )

    print()
    if scope == "global":
        print(
            f"Completed {len(results)} units, "
            f"{len(global_ok)} reached global_ok, "
            f"{len(failures)} ended with errors."
        )
    else:
        print(
            f"Completed {len(results)} scoped units, "
            f"{len(local_ok)} reached local_ok, "
            f"{len(failures)} ended with errors."
        )

    # mark_vo_as_bug count: these VOs are accepted as known bugs and counted as
    # discharged, so they inflate the global_ok/local_ok totals above. Always
    # surface the number so a clean-looking run is not silently hiding them.
    if total_bugs:
        names = ", ".join(sorted(r.function_name for r in bugged))
        print(
            f"VOs accepted as bugs (mark_vo_as_bug): {total_bugs} "
            f"in {len(bugged)} function(s), counted toward the states above: {names}"
        )
    else:
        print("VOs accepted as bugs (mark_vo_as_bug): 0")

    # Settled assume debts: LOCAL_OK modulo residual hypotheses on callees
    # (assume-debt wake budget spent). Surface them so a "local_ok" total is
    # not silently hiding unverified assumptions.
    conditional = [r for r in results if r.settled_assumes]
    if conditional:
        parts = ", ".join(
            f"{r.function_name} (assumes {', '.join(r.settled_assumes)})"
            for r in sorted(conditional, key=lambda r: r.function_name)
        )
        print(f"Conditional OK (assume budget spent): {parts}")

    # Environment axioms: contracts assumed for extern (prototype-only)
    # callees. Nothing can ever publish for them, so these are standing
    # axioms of the verification result — listed so the proof's hypothesis
    # ledger is explicit (the baseline assumes the same silently).
    axiomatic = [r for r in results if r.extern_axioms]
    if axiomatic:
        parts = ", ".join(
            f"{r.function_name} (axioms for {', '.join(r.extern_axioms)})"
            for r in sorted(axiomatic, key=lambda r: r.function_name)
        )
        print(f"Environment axioms (extern callees, contracts assumed): {parts}")

    if cycle_limited:
        names = ", ".join(sorted(r.function_name for r in cycle_limited))
        print(f"Cycle limit reached for: {names}")
