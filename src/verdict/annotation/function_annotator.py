"""Function annotator — orchestrates annotation of functions with ACSL contracts."""

from __future__ import annotations

import logging
from typing import Any, Dict, List, Tuple

from verdict.analysis.extended_call_graph import ExtendedCallGraphNode
from verdict.annotation.annotation_generator import AnnotationGenerator
from verdict.annotation.code_annotator import CodeAnnotator

logger = logging.getLogger(__name__)


class FunctionAnnotator:
    """Annotate functions with ACSL contracts (pre/post/assigns/loops)."""

    def __init__(self, function_declaration_map: Dict[str, str] | None = None) -> None:
        self.annotation_generator = AnnotationGenerator()
        self.code_annotator = CodeAnnotator()
        self.function_declaration_map = function_declaration_map or {}

    def get_function_with_annotations(
        self,
        node: ExtendedCallGraphNode,
        assign_loops_names: bool = False,
    ) -> Tuple[bool, str]:
        """Get function source enriched with all ACSL contracts."""
        if not node.source_code:
            return False, None

        annotated = node.source_code

        preconditions = node.pre_conditions
        assigns = node.assigns
        postconditions = node.post_conditions
        loop_contracts = node.loop_contracts
        extent = node.extent

        # Generate function contract
        func_contract = None
        if preconditions or assigns or postconditions:
            ok, func_contract = self.annotation_generator.generate_function_contract(
                preconditions, postconditions, assigns, extent
            )
            if not ok:
                return False, None

        # Generate loop contracts
        loop_annots = None
        if loop_contracts and node.loops:
            ok, loop_annots = self.annotation_generator.generate_all_loop_contracts(
                loop_contracts, extent, node.loops
            )
            if not ok:
                return False, None

        # Insert loop contracts first (bottom-to-top)
        if loop_annots:
            ok, annotated = self.code_annotator.insert_loop_contracts(annotated, loop_annots)
            if not ok:
                return False, None

        # Insert function contract second (prepend)
        if func_contract:
            ok, annotated = self.code_annotator.insert_function_contract(annotated, func_contract)
            if not ok:
                return False, None

        return True, annotated

    def get_callee_declarations_with_contracts(
        self,
        callee_node: ExtendedCallGraphNode,
    ) -> Tuple[bool, str]:
        """Get callee function definition with empty body and contracts."""
        func_decl = self.function_declaration_map.get(callee_node.func_name)
        if not func_decl:
            logger.warning("No declaration found for callee: %s", callee_node.func_name)
            return True, None

        # Convert declaration to definition with empty body
        func_def = func_decl.strip()
        if func_def.endswith(";"):
            func_def = func_def[:-1] + " { };"
        else:
            func_def = func_def + " { };"

        # Generate contract if annotations exist
        contract = None
        if callee_node.pre_conditions or callee_node.post_conditions or callee_node.assigns:
            ok, contract = self.annotation_generator.generate_function_contract(
                callee_node.pre_conditions,
                callee_node.post_conditions,
                callee_node.assigns,
                callee_node.extent,
            )
            if not ok:
                return False, None

        # Insert contract if present
        if contract:
            ok, result = self.code_annotator.insert_function_contract(func_def, contract)
            if not ok:
                return False, None
            return True, result

        return True, func_def
