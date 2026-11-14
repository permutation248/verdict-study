"""Interactive chat/debug CLI mode."""

from __future__ import annotations

import argparse
import asyncio
import logging
import signal
import sys
from pathlib import Path

from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.agent.runner import FunctionAgent
from verdict.cli.agent_stream import ChatSink, process_agent_stream
from verdict.cli.display import _build_function_info, _interactive_picker
from verdict.cli.output import _C, _ToolOutputBuffer
from verdict.core.project_context import ProjectContext
from verdict.core.spec_tracker import SpecTracker
from verdict.logging.logger import (
    ConversationLogger,
    ToolDebugger,
    create_run_dir,
    setup_logging,
)
from verdict.messaging.bus import AsyncQueueBus
from verdict.preprocessing.pipeline import preprocess


def cmd_chat(args: argparse.Namespace) -> None:
    """Interactive agent chat."""
    run_dir = create_run_dir(args.file)
    setup_logging(run_dir)

    if args.verbose:
        logging.getLogger("verdict").setLevel(logging.DEBUG)

    print(f"Preprocessing {args.file}...")
    project_ctx, _ = preprocess(
        args.file,
        header_files=args.header or None,
        cpp_extra_args=args.cpp_extra_args,
    )

    selected: list[str] = []
    if args.agent:
        for name in args.agent:
            if name not in project_ctx.call_graph_dict:
                print(f"Error: function '{name}' not found in call graph.")
                print(f"Available: {', '.join(sorted(project_ctx.call_graph_dict.keys()))}")
                sys.exit(1)
        selected = args.agent
    else:
        selected = _interactive_picker(project_ctx)
        if not selected:
            print("No functions selected.")
            return

    print(f"\nStarting agents for: {', '.join(selected)}\n")

    try:
        asyncio.run(_run_chat(selected, project_ctx, args, run_dir))
    except KeyboardInterrupt:
        print(f"\n{_C.SYSTEM}Interrupted.{_C.RESET}")


async def _run_chat(
    func_names: list[str],
    project_ctx: ProjectContext,
    args: argparse.Namespace,
    run_dir: Path,
) -> None:
    """Set up agents and run interactive chat loop."""
    from claude_agent_sdk import ClaudeSDKClient

    conv = ConversationLogger(run_dir)
    debugger = ToolDebugger(run_dir)
    output_buffer = _ToolOutputBuffer(verbose=args.verbose)
    output_buffer.install()
    sink = ChatSink(output_buffer=output_buffer, conv_logger=conv)
    conv.log_system(f"Source: {args.file}")
    conv.log_system(f"Agents: {', '.join(func_names)}")
    conv.log_system(f"Model: {args.model or 'default'}")
    spec_tracker = SpecTracker(run_dir, project_ctx)
    print(f"{_C.SYSTEM}Logs: {run_dir}{_C.RESET}")
    print(f"{_C.SYSTEM}Specs: {run_dir}/current_specs.c{_C.RESET}\n")

    registry = AgentRegistry()
    bus = AsyncQueueBus()

    clients: dict[str, ClaudeSDKClient] = {}
    agent_names: list[str] = []

    for func_name in func_names:
        agent_id = f"agent_{func_name}"
        info = _build_function_info(func_name, project_ctx)
        registry.register(AgentEntry(agent_id=agent_id, function_info=info))

        agent = FunctionAgent(
            agent_id,
            info,
            registry,
            bus,
            project_context=project_ctx,
            model=args.model,
            max_turns=args.max_turns,
            spec_tracker=spec_tracker,
            debugger=debugger,
            tool_profile="debug",
        )
        client = ClaudeSDKClient(options=agent.build_sdk_options())
        await client.connect()

        clients[func_name] = client
        agent_names.append(func_name)
        conv.log_system(f"Agent {func_name} connected")

    is_multi = len(agent_names) > 1

    print("=" * 60)
    if is_multi:
        print(f"Multi-agent mode: {', '.join(agent_names)}")
        print("Use @function_name to route messages.")
    else:
        print(f"Agent: {agent_names[0]}")
    print("Type 'quit' or 'exit' to stop. Ctrl-C also works.")
    print("=" * 60)
    print()

    shutdown = False
    loop = asyncio.get_running_loop()
    original_handler = signal.getsignal(signal.SIGINT)

    def _sigint_handler(sig, frame):
        del sig, frame
        nonlocal shutdown
        if shutdown:
            raise KeyboardInterrupt
        shutdown = True
        logging.getLogger("claude_agent_sdk").setLevel(logging.CRITICAL)
        print(f"\n{_C.SYSTEM}Shutting down...{_C.RESET}")

    signal.signal(signal.SIGINT, _sigint_handler)

    try:
        while not shutdown:
            try:
                if is_multi:
                    raw = await loop.run_in_executor(
                        None,
                        lambda: input(f"{_C.USER}[orchestrator] > {_C.RESET}"),
                    )
                else:
                    raw = await loop.run_in_executor(
                        None,
                        lambda: input(f"{_C.USER}[{agent_names[0]}] > {_C.RESET}"),
                    )
            except EOFError:
                break
            except KeyboardInterrupt:
                break

            if shutdown:
                break

            stripped = raw.strip()
            if stripped.lower() in ("quit", "exit"):
                break
            if not stripped:
                continue

            if is_multi:
                target, user_msg = _parse_multi_input(stripped, agent_names)
                if target is None:
                    print(f"  Use @name prefix. Available: {', '.join(agent_names)}")
                    continue
            else:
                target = agent_names[0]
                user_msg = stripped

            conv.log_user(target, user_msg)
            client = clients[target]

            try:
                await client.query(user_msg)
                await process_agent_stream(
                    client, agent_id=target, sink=sink
                )
            except asyncio.CancelledError:
                break
            except Exception:
                if shutdown:
                    break
                raise

            print()

    finally:
        signal.signal(signal.SIGINT, original_handler)
        output_buffer.uninstall()

        print(f"{_C.SYSTEM}Disconnecting agents...{_C.RESET}")
        for name, client in clients.items():
            try:
                await client.disconnect()
                conv.log_system(f"Agent {name} disconnected")
            except Exception:
                pass

        conv.log_system("Session ended")
        conv.close()
        print(f"{_C.SYSTEM}Done. Logs saved to {run_dir}{_C.RESET}")


def _parse_multi_input(raw: str, agent_names: list[str]) -> tuple[str | None, str]:
    """Parse @name prefix from multi-agent input."""
    if raw.startswith("@"):
        parts = raw.split(None, 1)
        name = parts[0][1:]
        if name in agent_names:
            msg = parts[1] if len(parts) > 1 else ""
            return name, msg
    return None, raw
