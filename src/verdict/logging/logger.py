"""Verdict logging system — per-run directory with pipeline + conversation logs."""

from __future__ import annotations

import json
import logging
import os
import shutil
from datetime import datetime
from pathlib import Path
from typing import Any


_LOG_DIR = Path("logs")


def create_run_dir(
    source_file: str,
    log_dir: Path | str = _LOG_DIR,
) -> Path:
    """Create a per-run log directory: logs/{stem}_{YYYYMMDD_HHMMSS}/

    Creation is exclusive: two runs of the same source starting within the
    same second must NOT share a directory (interleaved pipeline.log, a
    run.json overwritten by two run stores). On collision a ``-N`` suffix
    disambiguates.

    Returns the created directory path.
    """
    log_dir = Path(log_dir)
    stem = Path(source_file).stem  # e.g. "x509-submodule-2_cleaned_26"
    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    for n in range(100):
        suffix = "" if n == 0 else f"-{n + 1}"
        run_dir = log_dir / f"{stem}_{ts}{suffix}"
        try:
            run_dir.mkdir(parents=True, exist_ok=False)
            return run_dir
        except FileExistsError:
            continue
    raise RuntimeError(f"no unique run dir for {stem}_{ts} after 100 tries")


def setup_logging(run_dir: Path | None = None) -> None:
    """Initialize the verdict root logger.

    If *run_dir* is given, a file handler writes DEBUG+ to
    ``run_dir/pipeline.log``.  A console handler always outputs INFO+.
    """
    root = logging.getLogger("verdict")
    if root.handlers:
        return  # already configured
    root.setLevel(logging.DEBUG)

    # Console handler: INFO+
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    console.setFormatter(logging.Formatter("[%(name)s] %(levelname)s: %(message)s"))
    root.addHandler(console)

    # File handler: DEBUG+ → run_dir/pipeline.log
    if run_dir is not None:
        fh = logging.FileHandler(run_dir / "pipeline.log", encoding="utf-8")
        fh.setLevel(logging.DEBUG)
        fh.setFormatter(
            logging.Formatter("%(asctime)s [%(name)s] %(levelname)s: %(message)s")
        )
        root.addHandler(fh)


def create_agent_logger(agent_id: str) -> logging.Logger:
    """Return a child logger scoped to a specific agent."""
    return logging.getLogger(f"verdict.agent.{agent_id}")


def log_tool_call(
    logger: logging.Logger, tool_name: str, **kwargs: Any
) -> None:
    """Log an MCP tool invocation at DEBUG level."""
    logger.debug("tool_call %s args=%s", tool_name, kwargs)


def log_prompt(logger: logging.Logger, prompt: str) -> None:
    """Log a prompt sent to the LLM."""
    logger.debug("prompt length=%d first_100=%s", len(prompt), prompt[:100])


def log_agent_state_change(
    logger: logging.Logger,
    agent_id: str,
    old_state: str,
    new_state: str,
) -> None:
    """Log an agent state transition."""
    logger.info("agent %s: %s -> %s", agent_id, old_state, new_state)


# ---------------------------------------------------------------------------
# Conversation logger — writes conversation.log inside the run directory
# ---------------------------------------------------------------------------


class ConversationLogger:
    """Log full conversation (user / agent / tool) to conversation.log."""

    def __init__(self, run_dir: Path) -> None:
        self._path = run_dir / "conversation.log"
        self._fh = open(self._path, "w", encoding="utf-8")
        ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self._write(f"=== Verdict Chat Session {ts} ===\n")

    @property
    def path(self) -> str:
        return str(self._path)

    def _write(self, text: str) -> None:
        self._fh.write(text + "\n")
        self._fh.flush()

    def log_system(self, msg: str) -> None:
        self._write(f"[SYSTEM] {msg}")

    def log_user(self, target: str, msg: str) -> None:
        self._write(f"[USER -> {target}] {msg}")

    def log_agent_text(self, agent: str, text: str) -> None:
        self._write(f"[{agent}] {text}")

    def log_tool_call(self, agent: str, tool_name: str, args: dict) -> None:
        args_str = json.dumps(args, ensure_ascii=False)
        self._write(f"[{agent}:tool_call] {tool_name}({args_str})")

    def log_tool_result(
        self,
        agent: str,
        tool_use_id: str,
        content: Any,
        is_error: bool = False,
    ) -> None:
        prefix = "tool_error" if is_error else "tool_result"
        if isinstance(content, str):
            result_str = content
        elif isinstance(content, list):
            parts = []
            for block in content:
                if isinstance(block, dict) and block.get("type") == "text":
                    parts.append(block.get("text", ""))
                else:
                    parts.append(str(block))
            result_str = "\n".join(parts)
        else:
            result_str = str(content)
        self._write(f"[{agent}:{prefix}:{tool_use_id}] {result_str}")

    def log_result(
        self,
        agent: str,
        is_error: bool,
        cost: float | None = None,
        turns: int | None = None,
    ) -> None:
        status = "ERROR" if is_error else "OK"
        extra = ""
        if cost is not None:
            extra += f" cost=${cost:.4f}"
        if turns is not None:
            extra += f" turns={turns}"
        self._write(f"[{agent}:result] {status}{extra}")

    def close(self) -> None:
        self._fh.close()


# ---------------------------------------------------------------------------
# Per-tool debug artifact logging
# ---------------------------------------------------------------------------


class ToolDebugger:
    """Per-tool-invocation debug artifact saver."""

    def __init__(self, run_dir: Path) -> None:
        self._tools_dir = run_dir / "tools"
        self._tools_dir.mkdir(exist_ok=True)
        self._counter = 0

    def begin(self, tool_name: str) -> ToolTrace:
        self._counter += 1
        trace_dir = self._tools_dir / f"{self._counter:03d}_{tool_name}"
        trace_dir.mkdir()
        return ToolTrace(trace_dir)


class ToolTrace:
    """One tool invocation's debug artifacts.

    Uses counter-based naming so that multiple saves (e.g. per-iteration
    in a worklist) do not overwrite each other.
    """

    def __init__(self, trace_dir: Path) -> None:
        self._dir = trace_dir
        self._counter = 0

    def save_input(self, data: dict) -> None:
        (self._dir / "input.json").write_text(
            json.dumps(data, indent=2, ensure_ascii=False)
        )

    def save_annotated_file(self, src_path: str, suffix: str = "") -> None:
        self._counter += 1
        name = f"annotated_{self._counter:03d}{suffix}.c"
        shutil.copy2(src_path, self._dir / name)

    def save_command(self, command: list[str], suffix: str = "") -> None:
        self._counter += 1
        name = f"command_{self._counter:03d}{suffix}.txt"
        (self._dir / name).write_text(" ".join(command))

    def save_frama_c_output(
        self, output: str, success: bool, suffix: str = ""
    ) -> None:
        self._counter += 1
        name = f"frama_c_output_{self._counter:03d}{suffix}.txt"
        header = f"# success={success}\n"
        (self._dir / name).write_text(header + output)

    def save_worklist_log(self, log_text: str) -> None:
        """Save the human-readable worklist iteration log."""
        (self._dir / "worklist.log").write_text(log_text)

    def save_vo_reverification_log(self, log_text: str) -> None:
        """Save the VO re-verification log after weakening."""
        (self._dir / "vo_reverification.log").write_text(log_text)

    def save_result(self, data: Any) -> None:
        (self._dir / "result.json").write_text(
            json.dumps(data, indent=2, ensure_ascii=False, default=str)
        )
