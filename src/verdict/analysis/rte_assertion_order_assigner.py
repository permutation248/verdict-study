"""RTE assertion order assigner.

Assigns sequential numbers (rte_1, rte_2, ...) to RTE assertions in an
annotated CIL file using DFS post-order traversal of the extended call graph.

Adapted from reference_implementation/analysis/rte_assertion_order_assigner.py.
"""

from __future__ import annotations

import logging
import re
from enum import Enum
from typing import Any, Dict, List, Tuple

from verdict.analysis.extended_call_graph import ExtendedCallGraphNode

logger = logging.getLogger(__name__)

# Canonical parser for the labels this module writes (``assert rte_N:``). The
# assigner is the single numbering authority — it writes these labels into the
# CIL, and every downstream reader (pipeline._extract_rte_vos, the VO graph)
# parses the number back from here rather than re-deriving its own counter.
RTE_LABEL_RE = re.compile(r"assert\s+(rte_\d+)\s*:")


class RTEAssertionType(Enum):
    RTE = "rte"
    EVA = "Eva"


class AssertionPatternMatcher:
    """Pattern matching and replacement for RTE/Eva assertion labels."""

    def __init__(self, rte_assertion_type: RTEAssertionType) -> None:
        self.rte_assertion_type = rte_assertion_type
        prefix = rte_assertion_type.value
        self.single_line_pattern = re.compile(rf"assert\s+{prefix}[^:]*:")
        self.multi_line_pattern = re.compile(rf"\s*{prefix}[^:]*:")
        self.replacement_prefix = prefix

    def find_and_replace_single_line(
        self, line: str, counter: int
    ) -> Tuple[bool, str]:
        match = self.single_line_pattern.search(line)
        if match:
            new_line = (
                line[: match.start()]
                + f"assert {self.replacement_prefix}_{counter}:"
                + line[match.end() :]
            )
            return True, new_line
        return False, line

    def find_and_replace_multi_line(
        self, line: str, counter: int
    ) -> Tuple[bool, str]:
        match = self.multi_line_pattern.search(line)
        if match:
            new_line = (
                line[: match.start()]
                + f"        {self.replacement_prefix}_{counter}:"
                + line[match.end() :]
            )
            return True, new_line
        return False, line


class RTEAssertionOrderAssigner:
    """Assign sequential numbers to RTE assertions via DFS post-order."""

    def __init__(
        self,
        annotated_cil_file_path: str,
        extended_call_graph: Dict[str, Any],
        rte_assertion_type: str = "rte",
    ) -> None:
        self.annotated_cil_file_path = annotated_cil_file_path
        self.extended_call_graph = extended_call_graph
        self.rte_assertion_type = RTEAssertionType(rte_assertion_type)
        self.pattern_matcher = AssertionPatternMatcher(self.rte_assertion_type)
        self.assertion_counter = 0

        try:
            with open(annotated_cil_file_path, "r") as f:
                self.file_content = f.readlines()
        except Exception as exc:
            logger.error("Failed to read file: %s", exc)
            self.file_content = []

    def assign_assertion_orders(self) -> Tuple[bool, int]:
        """Assign sequential numbers to assertions. Rewrites the CIL file.

        Returns (success, total_assertions_processed).
        """
        if not self.file_content:
            return False, 0

        logger.info(
            "Assigning orders to %s assertions in %s",
            self.rte_assertion_type.value,
            self.annotated_cil_file_path,
        )

        # Find root functions (no parents)
        root_functions = [
            node
            for node in self.extended_call_graph.values()
            if not node.parents
        ]

        visited: set = set()
        self.assertion_counter = 0
        modified_lines = self.file_content.copy()

        def dfs_post_order(node: ExtendedCallGraphNode) -> None:
            if node in visited:
                return
            visited.add(node)

            for child in node.children:
                if child in self.extended_call_graph.values() and child not in visited:
                    dfs_post_order(child)

            for _content, extent in node.rte_asserts:
                self.assertion_counter += 1
                start_line, _sc, end_line, _ec = extent
                start_idx = start_line - 1

                line = modified_lines[start_idx]
                found, new_line = self.pattern_matcher.find_and_replace_single_line(
                    line, self.assertion_counter
                )
                if found:
                    modified_lines[start_idx] = new_line
                else:
                    for idx in range(start_idx + 1, min(end_line, len(modified_lines))):
                        found, new_line = self.pattern_matcher.find_and_replace_multi_line(
                            modified_lines[idx], self.assertion_counter
                        )
                        if found:
                            modified_lines[idx] = new_line
                            break
                    else:
                        logger.warning(
                            "Could not find assertion pattern in lines %d-%d for %s",
                            start_line, end_line, node.func_name,
                        )

        for root in root_functions:
            dfs_post_order(root)

        try:
            with open(self.annotated_cil_file_path, "w") as f:
                f.writelines(modified_lines)
            logger.info(
                "Assigned orders to %d %s assertions",
                self.assertion_counter,
                self.rte_assertion_type.value,
            )
            return True, self.assertion_counter
        except Exception as exc:
            logger.error("Failed to write modified content: %s", exc)
            return False, 0
