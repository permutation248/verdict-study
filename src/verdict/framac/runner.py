"""Frama-C runner — subprocess wrapper for invoking frama-c CLI."""

from __future__ import annotations

import asyncio
import logging
import os
import subprocess
from typing import List, Optional, Tuple

logger = logging.getLogger(__name__)


class FramaCRunner:
    """Wrapper for executing Frama-C commands."""

    def __init__(
        self,
        source_file_path: str,
        include_paths: Optional[List[str]] = None,
    ) -> None:
        self.source_file_path = source_file_path
        self.include_paths = include_paths or []
        self.cpp_extra_args: Optional[str] = None
        self._auto_detect_include_paths()
        logger.debug("FramaCRunner initialized for %s", source_file_path)

    def _auto_detect_include_paths(self) -> None:
        if not self.source_file_path:
            return
        source_dir = os.path.dirname(os.path.abspath(self.source_file_path))
        if source_dir not in self.include_paths:
            self.include_paths.append(source_dir)

        for rel in ("lib", "include", "src", "..", "../include", "../lib"):
            full = os.path.join(source_dir, rel)
            if os.path.exists(full) and full not in self.include_paths:
                self.include_paths.append(full)

    def add_include_path(self, path: str) -> None:
        if path not in self.include_paths:
            self.include_paths.append(path)

    def set_include_paths(self, paths: List[str]) -> None:
        self.include_paths = paths

    def _build_cpp_extra_args(self) -> str:
        if self.cpp_extra_args:
            return self.cpp_extra_args
        if not self.include_paths:
            return ""
        return " ".join(f"-I{p}" for p in self.include_paths)

    def update_file_paths(self, source_file_path: str) -> bool:
        self.source_file_path = source_file_path
        self.include_paths = []
        self._auto_detect_include_paths()
        return True

    def _build_full_command(
        self,
        command: List[str],
        additional_cpp_args: Optional[str] = None,
    ) -> Optional[List[str]]:
        """Build the full frama-c command list. Returns None on invalid input."""
        if not command or not all(isinstance(c, str) for c in command):
            return None

        full_command = ["frama-c"]

        cpp_args = self._build_cpp_extra_args()
        if additional_cpp_args:
            cpp_args = f"{cpp_args} {additional_cpp_args}" if cpp_args else additional_cpp_args

        if cpp_args and cpp_args.strip():
            full_command.append(f"-cpp-extra-args={cpp_args}")

        full_command.append(self.source_file_path)
        full_command.extend(command)
        return full_command

    def run_FramaC(
        self,
        command: List[str],
        additional_cpp_args: Optional[str] = None,
    ) -> Tuple[bool, str]:
        """Run Frama-C synchronously. Prefer run_FramaC_async in async contexts."""
        logger.debug("Running Frama-C")
        full_command = self._build_full_command(command, additional_cpp_args)
        if full_command is None:
            return False, "Invalid command"

        logger.debug("Running command: %s", " ".join(full_command))

        try:
            result = subprocess.run(full_command, capture_output=True, text=True)
            if result.returncode != 0:
                error_msg = result.stderr or result.stdout
                logger.error("Frama-C returned error code %d", result.returncode)
                return False, error_msg
            return True, result.stdout
        except Exception as exc:
            logger.error("Error running Frama-C: %s", exc)
            return False, str(exc)

    async def run_FramaC_async(
        self,
        command: List[str],
        additional_cpp_args: Optional[str] = None,
    ) -> Tuple[bool, str]:
        """Run Frama-C without blocking the event loop.

        Acquires the shared WP semaphore (``wp_resources``) so that at most
        ``wp_resources.jobs()`` frama-c processes run concurrently,
        regardless of how many agents are active.  While an agent awaits this
        semaphore, other agents can continue their LLM work.
        """
        from verdict.framac import wp_resources

        full_command = self._build_full_command(command, additional_cpp_args)
        if full_command is None:
            return False, "Invalid command"

        import time as _time

        sem = wp_resources.semaphore()
        t_queue = _time.monotonic()
        async with sem:
            t_start = _time.monotonic()
            wait_s = t_start - t_queue
            logger.debug("Running command: %s", " ".join(full_command))
            if wait_s > 0.5:
                logger.info("WP queue wait: %.1fs", wait_s)

            try:
                proc = await asyncio.create_subprocess_exec(
                    *full_command,
                    stdout=asyncio.subprocess.PIPE,
                    stderr=asyncio.subprocess.PIPE,
                )
                stdout, stderr = await proc.communicate()
                t_end = _time.monotonic()
                wp_s = t_end - t_start
                logger.info("WP finished: %.1fs (wait=%.1fs)", wp_s, wait_s)

                stdout_str = stdout.decode() if stdout else ""
                stderr_str = stderr.decode() if stderr else ""

                if proc.returncode != 0:
                    error_msg = stderr_str or stdout_str
                    logger.error("Frama-C returned error code %d", proc.returncode)
                    return False, error_msg
                return True, stdout_str
            except Exception as exc:
                logger.error("Error running Frama-C: %s", exc)
                return False, str(exc)
