"""Code annotator — inserts ACSL annotations into C source code."""

from __future__ import annotations

import logging
from typing import Dict, Tuple

logger = logging.getLogger(__name__)


class CodeAnnotator:
    """Insert annotations into source code at the correct positions."""

    def insert_function_contract(
        self, source_code: str, contract: str
    ) -> Tuple[bool, str]:
        """Insert *contract* right before the function declaration."""
        if not source_code or not contract:
            return False, None
        return True, contract + source_code

    def insert_loop_contracts(
        self, source_code: str, contracts_dict: Dict[int, str]
    ) -> Tuple[bool, str]:
        """Insert loop contracts at relative line positions (bottom-to-top)."""
        if not source_code or not contracts_dict:
            return False, None

        lines = source_code.split("\n")
        for rel_line in sorted(contracts_dict.keys(), reverse=True):
            lines.insert(rel_line, contracts_dict[rel_line])

        return True, "\n".join(lines)
