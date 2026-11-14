"""Extended Call Graph — function-level call graph with rich metadata.

ExtendedCallGraphNode is a pure-Python data class used throughout the
framework.  ExtendedCallGraph uses ClangParserBinding (C++ bindings) to
parse C source files and build the graph.
"""

from __future__ import annotations

import copy
import logging
import os
from typing import Any, Dict, List, Optional, Tuple

logger = logging.getLogger(__name__)


class ExtendedCallGraphNode:
    """Represents a function node in the call graph."""

    def __init__(self, func_name: str) -> None:
        self.func_name = func_name
        self.location: Optional[Tuple] = None
        self.extent: Optional[Tuple[int, int, int, int]] = None
        self.source_code: Optional[str] = None
        self.is_definition: bool = False
        self.loops: List[Dict[str, Any]] = []
        self.children: List[ExtendedCallGraphNode] = []
        self.parents: List[ExtendedCallGraphNode] = []
        self.call_sites: List[Tuple[str, Tuple[int, int, int, int]]] = []
        self.call_sites_orders: Dict[Tuple, int] = {}
        self.call_site_order: Dict[Tuple, int] = {}
        self.rte_asserts: List[Tuple[str, Tuple[int, int, int, int]]] = []
        self.call_site_asserts: List[Tuple[str, Tuple[int, int, int, int]]] = []
        self.post_conditions_asserts: List[Tuple[str, Tuple[int, int, int, int]]] = []
        self.loop_contracts_asserts: List[Tuple[str, Tuple[int, int, int, int]]] = []
        self.global_vars_used: List[Dict] = []

        self.pre_conditions: List[str] = []
        self.post_conditions: List[str] = []
        self.loop_contracts: Dict[str, List[str]] = {}
        self.assigns: List[str] = []

    # ------------------------------------------------------------------
    # Deserialization
    # ------------------------------------------------------------------

    @classmethod
    def from_json(cls, json_data: dict) -> ExtendedCallGraphNode:
        """Create node from JSON data (as produced by ClangParserBinding)."""
        node = cls(json_data["func_name"])

        if "location" in json_data:
            loc = json_data["location"]
            node.location = (loc["file"], loc["line"], loc["column"], loc["offset"])

        if "extent" in json_data:
            ext = json_data["extent"]
            node.extent = (ext["start_line"], ext["start_column"], ext["end_line"], ext["end_column"])

        node.source_code = json_data.get("source_code", "")
        node.is_definition = json_data.get("is_definition", False)

        for loop_data in json_data.get("loops", []):
            node.loops.append({
                "loop_name": loop_data["loop_name"],
                "loop_location": (
                    loop_data["location"]["file"],
                    loop_data["location"]["line"],
                    loop_data["location"]["column"],
                    loop_data["location"]["offset"],
                ),
                "loop_extent": (
                    loop_data["extent"]["start_line"],
                    loop_data["extent"]["start_column"],
                    loop_data["extent"]["end_line"],
                    loop_data["extent"]["end_column"],
                ),
                "loop_source_code": loop_data["source_code"],
            })

        for ad in json_data.get("rte_asserts", []):
            ext = ad["extent"]
            node.rte_asserts.append((
                ad["content"],
                (ext["start_line"], ext["start_column"], ext["end_line"], ext["end_column"]),
            ))

        for cs in json_data.get("call_sites", []):
            ext = cs["extent"]
            node.call_sites.append((
                cs["callee_func_name"],
                (ext["start_line"], ext["start_column"], ext["end_line"], ext["end_column"]),
            ))

        node.global_vars_used = json_data.get("global_vars_used", [])
        return node

    # ------------------------------------------------------------------
    # Mutation helper — the only annotation mutator still used (by _cil_builder
    # when applying loop contracts into a copied node).
    # ------------------------------------------------------------------

    def update_loop_contracts(self, loop_contracts: Dict[str, List[str]]) -> None:
        def _sort(contracts: List[str]) -> List[str]:
            invs = [c for c in contracts if "loop invariant" in c]
            asn = [c for c in contracts if "loop assigns" in c]
            var = [c for c in contracts if "loop variant" in c]
            return invs + asn + var

        for loop_name, contracts in loop_contracts.items():
            if loop_name not in [lp["loop_name"] for lp in self.loops]:
                logger.error("Loop name %s not found", loop_name)
                continue
            if loop_name not in self.loop_contracts:
                self.loop_contracts[loop_name] = []
            for c in contracts:
                if c not in self.loop_contracts[loop_name]:
                    self.loop_contracts[loop_name].append(c)
            self.loop_contracts[loop_name] = _sort(self.loop_contracts[loop_name])


    # ------------------------------------------------------------------
    # Deep copy (handles circular parent/child refs)
    # ------------------------------------------------------------------

    def __deepcopy__(self, memo: dict) -> ExtendedCallGraphNode:
        if id(self) in memo:
            return memo[id(self)]
        cls = self.__class__
        result = cls.__new__(cls)
        memo[id(self)] = result
        for k, v in self.__dict__.items():
            if k in ("children", "parents"):
                continue
            setattr(result, k, copy.deepcopy(v, memo))
        result.children = [copy.deepcopy(c, memo) for c in self.children]
        result.parents = [copy.deepcopy(p, memo) for p in self.parents]
        return result

    def __repr__(self) -> str:
        return f"Function: {self.func_name}"


# ---------------------------------------------------------------------------
# Helper: locate bindings path
# ---------------------------------------------------------------------------


def _get_bindings_dir() -> str:
    """Return the path to preprocessing/bindings."""
    env = os.environ.get("BINDINGS_DIR")
    if env and os.path.exists(env):
        return env
    # Default: relative to this package
    return os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        "..", "preprocessing", "bindings",
    )


class ExtendedCallGraph:
    """Build an extended call graph from a C source file using clang bindings."""

    def __init__(self, file_path: str) -> None:
        self.file_path = file_path
        self.call_graph: Dict[str, ExtendedCallGraphNode] = {}
        self.source_content: Optional[List[str]] = None

        self.call_site_orders: Dict[Tuple, int] = {}
        self.call_site_order: Dict[Tuple, int] = {}
        self.ordered_global_definitions: List[Dict] = []
        self.ordered_function_declarations: List[Dict] = []
        self.ordered_declarations: List[Dict] = []
        self.function_declarations_map: Dict[str, str] = {}

        bindings_dir = _get_bindings_dir()
        clang_args = self._build_clang_args(bindings_dir)

        from verdict.preprocessing.clang_parser_binding import ClangParserBinding
        self.clang_binding = ClangParserBinding(file_path, clang_args)

    @staticmethod
    def _build_clang_args(bindings_dir: str) -> List[str]:
        import platform as _platform

        args: List[str] = []
        if _platform.system() == "Darwin":
            sdk = os.environ.get("MACOS_SDK_PATH")
            if sdk:
                args.extend(["-isysroot", sdk])

        libc = os.environ.get("FRAMAC_LIBC_PATH")
        if libc:
            args.extend(["-I", libc])

        if os.path.exists(bindings_dir):
            args.extend(["-I", bindings_dir])

        project_root = os.environ.get("PROJECT_ROOT")
        if project_root:
            src_dir = os.path.join(project_root, "src")
            if os.path.exists(src_dir):
                args.extend(["-I", src_dir])

        return args

    def get_call_graph_dict(self) -> Dict[str, ExtendedCallGraphNode]:
        return self.call_graph

    def _load_source_code(self) -> bool:
        try:
            if os.path.exists(self.file_path):
                with open(self.file_path, "r") as f:
                    self.source_content = f.readlines()
                return True
            return False
        except Exception:
            self.source_content = None
            return False

    def generate_extended_call_graph(self) -> Dict[str, ExtendedCallGraphNode]:
        """Generate the extended call graph using C++ ClangParser."""
        logger.info("Generating extended call graph for %s", self.file_path)
        self._load_source_code()

        complete_data = self.clang_binding.parse_and_generate_call_graph()

        call_graph_data = complete_data.get("call_graph", {})
        self.ordered_global_definitions = complete_data.get("ordered_global_definitions", [])
        self.ordered_function_declarations = complete_data.get("ordered_function_declarations", [])

        self._format_ordered_global_definitions()
        self._format_ordered_function_declarations()

        combined = self.ordered_global_definitions + self.ordered_function_declarations
        combined.sort(key=lambda x: x.get("location", {}).get("line", 0))
        self.ordered_declarations = combined

        self._create_function_declarations_map()

        self.call_graph = {
            name: ExtendedCallGraphNode.from_json(data)
            for name, data in call_graph_data.items()
        }

        self._parse_call_site_orders(complete_data)
        self._establish_relationships()

        return self.call_graph

    def _format_ordered_global_definitions(self) -> None:
        for d in self.ordered_global_definitions:
            d["source_code"] = d["source_code"].strip() + ";"

    def _format_ordered_function_declarations(self) -> None:
        for d in self.ordered_function_declarations:
            src = d["source_code"].strip()
            if src and not src.endswith(";"):
                src += ";"
            d["source_code"] = src

    def _create_function_declarations_map(self) -> None:
        self.function_declarations_map = {
            d["name"]: d.get("source_code", "")
            for d in self.ordered_function_declarations
            if "name" in d
        }

    def _parse_call_site_orders(self, complete_data: Dict) -> None:
        self.call_site_orders = {}
        self.call_site_order = {}
        for key, order in complete_data.get("call_site_orders", {}).items():
            if "->" in key:
                caller, callee = key.split("->", 1)
                self.call_site_orders[(caller.strip(), callee.strip())] = order
        for key, order in complete_data.get("call_site_order", {}).items():
            if "->" not in key or "@" not in key:
                continue
            caller_callee, loc_str = key.rsplit("@", 1)
            caller, callee = caller_callee.split("->", 1)
            try:
                if ":" in loc_str:
                    line, col = loc_str.split(":", 1)
                    self.call_site_order[
                        (caller.strip(), callee.strip(), int(line), int(col))
                    ] = order
                else:
                    self.call_site_order[
                        (caller.strip(), callee.strip(), int(loc_str.strip()))
                    ] = order
            except ValueError:
                continue

    def _establish_relationships(self) -> None:
        for node in self.call_graph.values():
            for callee_name, _ in node.call_sites:
                callee_node = self.call_graph.get(callee_name)
                if callee_node is None:
                    continue
                if callee_node not in node.children:
                    node.children.append(callee_node)
                if node not in callee_node.parents:
                    callee_node.parents.append(node)

