"""CLI entry point for Verdict modes."""

from __future__ import annotations

import argparse
import sys

import pyfiglet

from verdict.cli.chat_mode import cmd_chat
from verdict.cli.display import _build_call_chain_display
from verdict.cli.doctor import cmd_doctor
from verdict.cli.output import _init_colors
from verdict.cli.solve_mode import cmd_solve
from verdict.cli.step_mode import cmd_step
from verdict.logging.logger import create_run_dir, setup_logging
from verdict.preprocessing.pipeline import preprocess


def cmd_info(args: argparse.Namespace) -> None:
    """Preprocess and list all functions."""
    run_dir = create_run_dir(args.file)
    setup_logging(run_dir)

    print(f"Preprocessing {args.file}...")
    project_ctx, _ = preprocess(
        args.file,
        header_files=args.header or None,
        cpp_extra_args=args.cpp_extra_args,
    )

    display = _build_call_chain_display(
        project_ctx.call_graph_dict,
        project_ctx.rte_vos_by_function,
    )

    print(f"\nFunctions ({len(project_ctx.call_graph_dict)}):\n")
    for _name, line in display:
        print(line)
    print(f"\nWork dir: {project_ctx.work_dir}")
    print(f"Logs: {run_dir}")


def main() -> None:
    """Main CLI entry point."""
    _init_colors()

    banner = pyfiglet.figlet_format("VERDICT", font="slant")
    print(banner)
    print("Multi-agent formal verification for C programs\n")

    parser = argparse.ArgumentParser(
        prog="verdict",
        description="CLI for Verdict solve, chat, and inspection workflows",
    )
    subparsers = parser.add_subparsers(dest="command")

    info_p = subparsers.add_parser("info", help="Preprocess & list functions")
    info_p.add_argument("-f", "--file", required=True, help="C source file")
    info_p.add_argument(
        "-H",
        "--header",
        action="append",
        default=[],
        help="Extra header files (repeatable)",
    )
    info_p.add_argument(
        "--cpp-extra-args",
        default=None,
        help="Extra Frama-C preprocessor flags",
    )

    solve_p = subparsers.add_parser("solve", help="Run the autonomous solve loop")
    solve_p.add_argument("-f", "--file", required=True, help="C source file")
    solve_p.add_argument(
        "-H",
        "--header",
        action="append",
        default=[],
        help="Extra header files (repeatable)",
    )
    solve_p.add_argument(
        "--cpp-extra-args",
        default=None,
        help="Extra Frama-C preprocessor flags",
    )
    solve_p.add_argument(
        "--scope",
        choices=["global", "functions"],
        default="global",
        help="Solve the whole project or only an explicit function subset",
    )
    solve_p.add_argument(
        "--function",
        action="append",
        default=[],
        help="Function name to include in function-scoped solve (repeatable)",
    )
    solve_p.add_argument(
        "--model",
        default=None,
        help="Claude model (default: claude-sonnet-4-20250514)",
    )
    solve_p.add_argument(
        "--max-turns",
        type=int,
        default=30,
        help="Max tool-call turns per agent cycle",
    )
    solve_p.add_argument(
        "--max-parallel",
        type=int,
        default=8,
        help="Limit parallel agent cycles (default 8)",
    )
    solve_p.add_argument(
        "--wp-procs",
        type=int,
        default=8,
        help="Prover processes per WP run (frama-c -wp-par). Default 8",
    )
    solve_p.add_argument(
        "--wp-jobs",
        type=int,
        default=None,
        help="Max concurrent WP runs. Default: same as --max-parallel",
    )
    solve_p.add_argument(
        "--wp-model",
        default=None,
        help="Frama-C -wp-model override (e.g. Typed+var+int+real for the "
             "SpecSyn-evaluation semantics ablation: unbounded ints + real "
             "floats — unsound vs machine semantics). Default: none (sound "
             "Typed model)",
    )
    solve_p.add_argument(
        "--ablate",
        default=None,
        help="RQ3 coordination ablation: comma-separated mechanisms to "
             "disable. Values: demands (proof-demand protocol), assume "
             "(optimistic callee hypotheses; extern axioms stay). Default: "
             "none (normal operation)",
    )
    solve_p.add_argument(
        "--schedule",
        choices=["leaf-first", "declared"],
        default="leaf-first",
        help="Agent launch order. leaf-first: reverse-topological (callees "
             "before callers) so callees publish specs before callers run, "
             "cutting optimistic demand cascades when max-parallel < #functions. "
             "declared: call-graph/alphabetical order. Default leaf-first",
    )
    solve_p.add_argument(
        "--max-cycles",
        type=int,
        default=25,
        help="Per-agent reactivation ceiling (initial solve + wake-ups)",
    )
    solve_p.add_argument(
        "--timeout",
        type=float,
        default=3600,
        help="Overall solve timeout in seconds (default 3600s / 1h)",
    )
    solve_p.add_argument(
        "--resume-from",
        default=None,
        help="Resume from a previous run ID (loads specs and optionally sessions)",
    )
    solve_p.add_argument(
        "--no-history",
        action="store_true",
        help="With --resume-from: load specs but not conversation history",
    )
    solve_p.add_argument(
        "--enable-dep-analysis",
        action="store_true",
        help="Enable AUTO callee-support ablation: agents that re-publish "
        "discharge unaffected callers without re-verification (default off)",
    )
    solve_p.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="DEBUG-level logging",
    )

    step_p = subparsers.add_parser(
        "step", help="Interactive step-through solve (you control agent wakes)"
    )
    step_p.add_argument("-f", "--file", required=True, help="C source file")
    step_p.add_argument(
        "-H", "--header", action="append", default=[],
        help="Extra header files (repeatable)",
    )
    step_p.add_argument("--cpp-extra-args", default=None, help="Extra preprocessor flags")
    step_p.add_argument(
        "--scope", choices=["global", "functions"], default="global",
        help="Solve scope",
    )
    step_p.add_argument(
        "--function", action="append", default=[],
        help="Function name (repeatable, for function-scoped)",
    )
    step_p.add_argument("--model", default=None, help="Claude model")
    step_p.add_argument("--max-turns", type=int, default=30, help="Max turns per cycle")
    step_p.add_argument("--max-parallel", type=int, default=8, help="Limit parallel agents (default 8)")
    step_p.add_argument(
        "--wp-procs", type=int, default=8,
        help="Prover processes per WP run (frama-c -wp-par). Default 8",
    )
    step_p.add_argument(
        "--wp-jobs", type=int, default=None,
        help="Max concurrent WP runs. Default: same as --max-parallel",
    )
    step_p.add_argument(
        "--schedule", choices=["leaf-first", "declared"], default="leaf-first",
        help="Agent launch order: leaf-first (callees before callers) or declared",
    )
    step_p.add_argument("--max-cycles", type=int, default=25, help="Per-agent cycle ceiling")
    step_p.add_argument("--timeout", type=float, default=3600, help="Overall timeout (seconds)")
    step_p.add_argument(
        "--enable-dep-analysis",
        action="store_true",
        help="Enable AUTO callee-support ablation (default off)",
    )
    step_p.add_argument("-v", "--verbose", action="store_true", help="DEBUG-level logging")

    subparsers.add_parser("doctor", help="Check the Verdict environment is ready")

    chat_p = subparsers.add_parser("chat", help="Interactive agent chat")
    chat_p.add_argument("-f", "--file", required=True, help="C source file")
    chat_p.add_argument(
        "-H",
        "--header",
        action="append",
        default=[],
        help="Extra header files (repeatable)",
    )
    chat_p.add_argument(
        "--cpp-extra-args",
        default=None,
        help="Extra Frama-C preprocessor flags",
    )
    chat_p.add_argument(
        "-a",
        "--agent",
        action="append",
        default=[],
        help="Function name to start agent for (repeatable)",
    )
    chat_p.add_argument(
        "--model",
        default=None,
        help="Claude model (default: claude-sonnet-4-20250514)",
    )
    chat_p.add_argument(
        "--max-turns",
        type=int,
        default=30,
        help="Max tool-call turns per agent interaction",
    )
    chat_p.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="DEBUG-level logging",
    )

    args = parser.parse_args()

    if args.command == "info":
        cmd_info(args)
    elif args.command == "solve":
        cmd_solve(args)
    elif args.command == "chat":
        cmd_chat(args)
    elif args.command == "step":
        cmd_step(args)
    elif args.command == "doctor":
        sys.exit(cmd_doctor(args))
    else:
        parser.print_help()
        sys.exit(1)


if __name__ == "__main__":
    main()
