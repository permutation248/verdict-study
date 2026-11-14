"""ACSL annotation generator.

Generates properly formatted ACSL contract strings (requires, ensures,
assigns, loop invariants) for insertion into C source code.
"""

from __future__ import annotations

import logging
from typing import Any, Dict, List, Tuple

logger = logging.getLogger(__name__)


class AnnotationGenerator:
    """Generate ACSL annotation strings in Frama-C format."""

    def generate_function_preconditions(
        self, preconditions: List[str], function_extent: Tuple[int, int, int, int]
    ) -> Tuple[bool, str]:
        return self.generate_function_contract(preconditions, [], [], function_extent)

    def generate_function_assigns(
        self, assigns: List[str], function_extent: Tuple[int, int, int, int]
    ) -> Tuple[bool, str]:
        return self.generate_function_contract([], [], assigns, function_extent)

    def generate_function_postconditions(
        self, postconditions: List[str], function_extent: Tuple[int, int, int, int]
    ) -> Tuple[bool, str]:
        return self.generate_function_contract([], postconditions, [], function_extent)

    def generate_function_contract(
        self,
        preconditions: List[str],
        postconditions: List[str],
        assigns: List[str],
        function_extent: Tuple[int, int, int, int],
    ) -> Tuple[bool, str]:
        """Generate a function contract. ACSL order: requires -> assigns -> ensures."""
        if not preconditions and not postconditions and not assigns:
            return False, None

        contract_parts: List[str] = []

        # requires (no dedup — must match pre_conditions list for WP obligation count)
        contract_parts.extend(preconditions)

        # assigns (dedup)
        seen_assigns: List[str] = []
        for a in assigns:
            if a not in seen_assigns:
                seen_assigns.append(a)
        contract_parts.extend(seen_assigns)

        # ensures (dedup)
        seen_post: List[str] = []
        for p in postconditions:
            if p not in seen_post:
                seen_post.append(p)
        contract_parts.extend(seen_post)

        contract = "/*@\n" + "\n".join(contract_parts) + "\n*/\n"

        _, start_col, _, _ = function_extent
        return True, self._align_annotation(contract, start_col)

    def generate_each_loop_contract(
        self,
        loop_contracts: List[str],
        function_extent: Tuple[int, int, int, int],
        loop_extent: Tuple[int, int, int, int],
    ) -> Tuple[bool, Dict[int, str]]:
        function_start_line = function_extent[0]
        loop_start_line, loop_start_col, _, _ = loop_extent
        relative = loop_start_line - function_start_line

        if loop_contracts:
            text = "/*@\n" + "\n".join(loop_contracts) + "\n*/"
        else:
            text = ""

        aligned = self._align_annotation(text, loop_start_col)
        return True, {relative: aligned}

    def generate_all_loop_contracts(
        self,
        loop_contracts_dict: Dict[str, List[str]],
        function_extent: Tuple[int, int, int, int],
        loop_info_list: List[Dict[str, Any]],
    ) -> Tuple[bool, Dict[int, str]]:
        if not loop_contracts_dict or not loop_info_list:
            return False, None

        all_contracts: Dict[int, str] = {}
        for loop_info in loop_info_list:
            loop_name = loop_info["loop_name"]
            loop_extent = loop_info["loop_extent"]
            if loop_name not in loop_contracts_dict:
                continue
            ok, d = self.generate_each_loop_contract(
                loop_contracts_dict[loop_name], function_extent, loop_extent
            )
            if not ok:
                return False, None
            all_contracts.update(d)

        return True, all_contracts

    # ------------------------------------------------------------------

    @staticmethod
    def _align_annotation(annotation: str, start_col: int) -> str:
        indent = " " * (start_col - 1)
        return "\n".join(indent + line for line in annotation.split("\n"))
