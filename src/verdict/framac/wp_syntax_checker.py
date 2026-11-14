"""WP syntax checker — validates ACSL annotation syntax via Frama-C."""

from __future__ import annotations

import logging
import re
from typing import List, Tuple

from verdict.framac.runner import FramaCRunner

logger = logging.getLogger(__name__)


class WPSyntaxChecker:
    """Validate ACSL annotation syntax using Frama-C kernel."""

    def __init__(self) -> None:
        self.source_file_path: str | None = None
        self.frama_c_runner: FramaCRunner | None = None
        self.commands = ["-kernel-warn-key", "annot-error"]

    def set_source_file_path(self, source_file_path: str) -> None:
        self.source_file_path = source_file_path
        self.frama_c_runner = FramaCRunner(source_file_path)

    def check_syntax(self) -> Tuple[bool, List[str]]:
        """Check ACSL syntax. Returns (has_errors, error_list)."""
        try:
            success, output = self.frama_c_runner.run_FramaC(self.commands)
            if success:
                has_errors, errors = self._check_annotation_errors(output)
                return has_errors, errors
            else:
                return True, [f"Frama-C execution failed: {output}"]
        except Exception as exc:
            return True, [f"Exception during syntax check: {exc}"]

    @staticmethod
    def _check_annotation_errors(result: str) -> Tuple[bool, List[str]]:
        pattern = r"\[kernel:annot-error\][^\n]*\n\s+[^\n]*"
        errors = re.findall(pattern, result)
        return (True, errors) if errors else (False, [])
