"""Verification Obligation Graph — VO nodes derived from ExtendedCallGraph.

VO nodes derived from the extended call graph for the multi-agent architecture.
The orchestrator uses this to pre-compute RTE and call-site VOs per function.
"""

from __future__ import annotations

import enum
import logging
from typing import Any, Dict, List, Optional, Tuple

from verdict.analysis.extended_call_graph import ExtendedCallGraphNode
from verdict.analysis.rte_assertion_order_assigner import RTE_LABEL_RE

logger = logging.getLogger(__name__)


# ---------------------------------------------------------------------------
# Enums
# ---------------------------------------------------------------------------


class VerificationStatus(enum.Enum):
    PENDING = "pending"
    VERIFIED = "verified"
    UNVERIFIABLE = "failed"
    UNKNOWN = "unknown"


class RTEType(enum.Enum):
    MEM_ACCESS = "mem_access"
    SIGNED_OVERFLOW = "signed_overflow"
    UNSIGNED_OVERFLOW = "unsigned_overflow"
    PTR_COMPARISON = "ptr_comparison"
    DIVISION_BY_ZERO = "division_by_zero"
    INDEX_BOUND = "index_bound"
    INITIALIZATION = "initialization"
    FLOAT_TO_INT = "float_to_int"
    IS_NAN_OR_INFINITE = "is_nan_or_infinite"
    UNKNOWN = "unknown"


class AssertionType(enum.Enum):
    RTE = "rte"
    CALL_SITE = "call_site"
    LOOP_INVARIANT = "loop_invariant"
    POSTCONDITION = "postcondition"
    UNKNOWN = "unknown"


# ---------------------------------------------------------------------------
# RTE type detection
# ---------------------------------------------------------------------------

_RTE_KEYWORDS: list[tuple[str, RTEType]] = [
    ("mem_access", RTEType.MEM_ACCESS),
    ("signed_overflow", RTEType.SIGNED_OVERFLOW),
    ("unsigned_overflow", RTEType.UNSIGNED_OVERFLOW),
    ("ptr_comparison", RTEType.PTR_COMPARISON),
    ("division_by_zero", RTEType.DIVISION_BY_ZERO),
    ("index_bound", RTEType.INDEX_BOUND),
    ("initialization", RTEType.INITIALIZATION),
    ("float_to_int", RTEType.FLOAT_TO_INT),
    ("is_nan_or_infinite", RTEType.IS_NAN_OR_INFINITE),
]


def _determine_rte_type(content: str) -> RTEType:
    for keyword, rte_type in _RTE_KEYWORDS:
        if keyword in content:
            return rte_type
    return RTEType.UNKNOWN


# ---------------------------------------------------------------------------
# VO Node
# ---------------------------------------------------------------------------


class VerificationObligationNode:
    """A single verification obligation derived from the call graph."""

    def __init__(
        self,
        vo_id: str,
        extended_function_node: ExtendedCallGraphNode,
        assertion_type: AssertionType,
        status: VerificationStatus = VerificationStatus.PENDING,
    ) -> None:
        self.vo_id = vo_id
        self.extended_function_node = extended_function_node
        self.assertion_type = assertion_type
        self.status = status
        self.func_name: str = extended_function_node.func_name
        self.extent: Optional[Tuple] = None
        self.assertion_content: Optional[str] = None
        self.rte_assertion_type: Optional[RTEType] = None

    def update_status(self, status: VerificationStatus) -> None:
        self.status = status

    def __repr__(self) -> str:
        return f"VONode({self.vo_id}, {self.assertion_type.name})"


class CallSiteVerificationObligationNode(VerificationObligationNode):
    """VO node for a call-site precondition check."""

    def __init__(
        self,
        vo_id: str,
        extended_function_node: ExtendedCallGraphNode,
        caller_node: ExtendedCallGraphNode,
        callee_node: ExtendedCallGraphNode,
        call_site_order: int = 0,
    ) -> None:
        super().__init__(vo_id, extended_function_node, AssertionType.CALL_SITE)
        self.caller_extended_function_node = caller_node
        self.callee_extended_function_node = callee_node
        self.call_site_order = call_site_order


# ---------------------------------------------------------------------------
# VO Graph
# ---------------------------------------------------------------------------


class VerificationObligationGraph:
    """Build VO nodes from an ExtendedCallGraph dict.

    Usage (by Orchestrator):
        vog = VerificationObligationGraph()
        vog.generate_from_extended_call_graph(call_graph_dict)
        nodes = vog.nodes_dict  # {vo_id: VerificationObligationNode}
    """

    def __init__(self) -> None:
        self.nodes_dict: Dict[str, VerificationObligationNode] = {}
        self.nodes_list: List[VerificationObligationNode] = []

    def generate_from_extended_call_graph(
        self,
        call_graph_dict: Dict[str, Any],
    ) -> Dict[str, VerificationObligationNode]:
        """Generate VO nodes via post-order DFS over the call graph."""
        # Find root functions (no parents)
        roots = [
            n for n in call_graph_dict.values()
            if not getattr(n, "parents", [])
        ]

        visited: set = set()
        cs_counter = 0

        def dfs(node: Any) -> None:
            nonlocal cs_counter
            if id(node) in visited:
                return
            visited.add(id(node))

            # Children first (post-order)
            for child in getattr(node, "children", []):
                if child in call_graph_dict.values():
                    dfs(child)

            # Merge call-sites and RTE asserts, sort by start line
            items: list[tuple[int, str, Any]] = []
            for callee_name, extent in getattr(node, "call_sites", []):
                items.append((extent[0], "cs", (callee_name, extent)))
            for content, extent in getattr(node, "rte_asserts", []):
                items.append((extent[0], "rte", (content, extent)))
            items.sort(key=lambda x: x[0])

            for _, kind, data in items:
                if kind == "rte":
                    content, extent = data
                    # Read the rte_N label the assigner wrote into the CIL — the
                    # single numbering authority. We do NOT re-number here: an
                    # independent counter would silently diverge from the CIL
                    # labels (and thus the -wp-prop targets) if rte_asserts order
                    # ever changed. Skip an unlabeled assert, matching
                    # pipeline._extract_rte_vos which also keeps only labeled ones.
                    label = RTE_LABEL_RE.search(content)
                    if label is None:
                        logger.warning(
                            "rte assert without an assigned label, skipping: %s",
                            content[:60],
                        )
                        continue
                    vo = VerificationObligationNode(
                        vo_id=label.group(1),
                        extended_function_node=node,
                        assertion_type=AssertionType.RTE,
                    )
                    vo.extent = extent
                    vo.assertion_content = content
                    vo.rte_assertion_type = _determine_rte_type(content)
                elif kind == "cs":
                    cs_counter += 1
                    callee_name, extent = data
                    callee_node = call_graph_dict.get(callee_name)
                    if callee_node is None:
                        continue
                    vo = CallSiteVerificationObligationNode(
                        vo_id=f"call_site_{cs_counter}",
                        extended_function_node=node,
                        caller_node=node,
                        callee_node=callee_node,
                    )
                    vo.extent = extent
                else:
                    continue

                self.nodes_list.append(vo)
                self.nodes_dict[vo.vo_id] = vo

        for root in roots:
            dfs(root)

        logger.info("Generated %d VO nodes", len(self.nodes_list))
        return self.nodes_dict
