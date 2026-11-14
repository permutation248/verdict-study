"""WP result analyzer — parses Frama-C WP output into structured results."""

from __future__ import annotations

import logging
import re
from typing import Dict, List, Tuple

logger = logging.getLogger(__name__)

# WP verdicts that count as discharged.
_VALID_STATUSES = ("Valid", "Dead", "Partial")


class WPResultAnalyzer:
    """Parse and analyze Frama-C WP plugin output."""

    def __init__(self) -> None:
        self.wp_result: str | None = None

    def set_wp_result(self, wp_result: str) -> None:
        self.wp_result = wp_result

    def get_wp_result(self) -> str | None:
        return self.wp_result

    def is_all_rte_properties_valid(
        self, verification_results: Dict[str, Tuple[bool, str, str]]
    ) -> bool:
        return all(r[0] for r in verification_results.values())

    def analyze_wp_rte_result(self) -> Dict[str, Tuple[bool, str, str]]:
        """Analyze WP result and extract RTE/Eva verification data.

        Returns {assertion_id: (is_valid, status_str, obligation_text)} where
        status_str is the raw WP verdict (Valid/Unknown/Timeout/Invalid) and
        obligation_text is the full goal block.
        """
        if not self.wp_result:
            logger.error("WP result is empty")
            return {}
        if "No verification obligation" in self.wp_result:
            logger.warning("No verification obligations found")
            return {}

        parts = self.wp_result.split("[report] Computing properties status...")
        goals_part = parts[0]
        properties_part = parts[1] if len(parts) > 1 else ""

        rte_obligations = self.extract_detailed_obligations(goals_part)
        rte_statuses = self.extract_verification_statuses(properties_part)

        result_dict: Dict[str, Tuple[bool, str, str]] = {}
        all_ids = set(rte_obligations.keys()) | set(rte_statuses.keys())
        for rte_id in all_ids:
            status_str = rte_statuses.get(rte_id, "Unknown")
            is_valid = status_str in _VALID_STATUSES
            obligation = rte_obligations.get(rte_id, f"Unknown obligation for {rte_id}")
            result_dict[rte_id] = (is_valid, status_str, obligation)

        return result_dict

    def extract_detailed_obligations(self, goals_text: str) -> Dict[str, str]:
        """Extract detailed verification obligations from Goal sections.

        Returns {assertion_id: full_goal_block}.
        """
        rte_obligations: Dict[str, str] = {}
        goal_blocks = re.finditer(
            r"Goal Assertion '((?:rte_\d+|Eva_\d+))(?:,([^']*))?' "
            r"\(file ([^,]+), line (\d+)\):(.*?)(?=\n-{60})",
            goals_text,
            re.DOTALL,
        )
        for match in goal_blocks:
            assertion_id = match.group(1)
            rte_obligations[assertion_id] = match.group(0).strip()
        return rte_obligations

    def extract_verification_statuses(self, properties_text: str) -> Dict[str, str]:
        """Extract the RAW WP verdict for each RTE/Eva assertion.

        Returns {assertion_id: status_str} where status_str is the verbatim WP
        verdict (Valid / Unknown / Timeout / Invalid / Dead / Partial). The
        caller decides validity; keeping the raw verdict preserves the
        Timeout/Unknown (provable, needs hypotheses) vs Invalid (counterexample)
        distinction that a bool would erase.
        """
        rte_statuses: Dict[str, str] = {}

        def _scan(text: str) -> None:
            for match in re.finditer(
                r"\[\s*([^\]]+)\s*\]\s*Assertion\s+"
                r"'((?:rte_\d+|Eva_\d+))(?:,([^']*))?' "
                r"\(file\s+([^,]+),\s+line\s+(\d+)\)",
                text,
                re.MULTILINE,
            ):
                rte_statuses[match.group(2)] = match.group(1).strip()

        if properties_text:
            _scan(properties_text)
        # Fallback: search whole wp_result if nothing found
        if not rte_statuses and self.wp_result:
            _scan(self.wp_result)

        return rte_statuses
