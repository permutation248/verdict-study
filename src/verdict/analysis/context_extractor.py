"""Context extractor — BFS-based extraction of callee/caller context."""

from __future__ import annotations

import logging
from collections import deque
from dataclasses import dataclass, field
from typing import Any, Dict, List, Set

from verdict.analysis.extended_call_graph import ExtendedCallGraphNode

logger = logging.getLogger(__name__)


@dataclass
class CalleeLayer:
    """A layer of callees at a given BFS depth."""
    depth: int
    functions: List[ExtendedCallGraphNode] = field(default_factory=list)


@dataclass
class KContextResult:
    """Result of k-layer callee extraction."""
    root: ExtendedCallGraphNode
    layers: List[CalleeLayer] = field(default_factory=list)


class ContextExtractor:
    """Extract k-layer callee context via BFS."""

    def __init__(self, max_callee_depth: int = 1) -> None:
        self.max_callee_depth = max_callee_depth

    def extract_k_layer_callees(
        self,
        root_node: ExtendedCallGraphNode,
        call_graph_dict: Dict[str, Any],
    ) -> KContextResult:
        """BFS extraction of callees up to k layers deep."""
        result = KContextResult(root=root_node)
        visited: Set[str] = {root_node.func_name}
        queue: deque[tuple[ExtendedCallGraphNode, int]] = deque()

        # Seed with direct callees
        for child in root_node.children:
            if child.func_name not in visited:
                queue.append((child, 1))
                visited.add(child.func_name)

        current_depth = 0
        current_layer: List[ExtendedCallGraphNode] = []

        while queue:
            node, depth = queue.popleft()
            if depth > self.max_callee_depth:
                continue

            if depth != current_depth:
                if current_layer:
                    result.layers.append(
                        CalleeLayer(depth=current_depth, functions=current_layer)
                    )
                current_layer = []
                current_depth = depth

            current_layer.append(node)

            # Enqueue next level
            if depth < self.max_callee_depth:
                for child in node.children:
                    if child.func_name not in visited:
                        queue.append((child, depth + 1))
                        visited.add(child.func_name)

        if current_layer:
            result.layers.append(
                CalleeLayer(depth=current_depth, functions=current_layer)
            )

        return result
