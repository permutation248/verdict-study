"""Python binding for the C++ ClangParser shared library.

Adapted from reference_implementation/analysis/clang_parser_binding.py
to use verdict's own logging and path conventions.
"""

from __future__ import annotations

import json
import logging
import os
import platform
from ctypes import CDLL, POINTER, c_char_p, c_int, c_void_p
from typing import Any, Dict, List

logger = logging.getLogger(__name__)


class ClangParserBinding:
    """Python ctypes binding for libclang_parser (.dylib/.so/.dll)."""

    def __init__(self, file_path: str, clang_args: List[str]) -> None:
        self.file_path = file_path
        self.clang_args = clang_args

        lib_path = self._find_library_path()
        self.lib = CDLL(lib_path)
        self._setup_function_signatures()

    def _find_library_path(self) -> str:
        """Locate the compiled shared library."""
        # 1. Check BINDINGS_DIR env var (set by env_setup.sh)
        bindings_dir = os.environ.get("BINDINGS_DIR")
        if not bindings_dir:
            # 2. Fallback: look relative to this file
            bindings_dir = os.path.join(
                os.path.dirname(os.path.abspath(__file__)), "bindings"
            )

        system = platform.system().lower()
        if system == "darwin":
            lib_ext = "dylib"
        elif system == "windows":
            lib_ext = "dll"
        else:
            lib_ext = "so"

        lib_name = f"libclang_parser.{lib_ext}"

        # Primary: bindings_dir/build/
        lib_path = os.path.join(bindings_dir, "build", lib_name)
        if os.path.exists(lib_path):
            logger.info("Using clang_parser library: %s", lib_path)
            return lib_path

        # Fallback: directly in bindings_dir
        fallback = os.path.join(bindings_dir, lib_name)
        if os.path.exists(fallback):
            logger.info("Using clang_parser library (fallback): %s", fallback)
            return fallback

        raise FileNotFoundError(
            f"Could not find clang_parser library.\n"
            f"  Searched: {lib_path}\n"
            f"  Searched: {fallback}\n"
            "Run build.sh in src/verdict/preprocessing/bindings/ first."
        )

    def _setup_function_signatures(self) -> None:
        """Configure ctypes function signatures for the C API."""
        self.lib.createParser.argtypes = [c_char_p, POINTER(c_char_p), c_int]
        self.lib.createParser.restype = c_void_p

        self.lib.destroyParser.argtypes = [c_void_p]
        self.lib.destroyParser.restype = None

        self.lib.parseFile.argtypes = [c_void_p]
        self.lib.parseFile.restype = c_int

        self.lib.generateExtendedCallGraph.argtypes = [c_void_p]
        self.lib.generateExtendedCallGraph.restype = None

        self.lib.getCallGraphJSON.argtypes = [c_void_p]
        self.lib.getCallGraphJSON.restype = c_char_p

    def parse_and_generate_call_graph(self) -> Dict[str, Any]:
        """Parse file and generate extended call graph with call site orders.

        Returns a dict with keys: call_graph, ordered_global_definitions,
        ordered_function_declarations, call_site_orders, call_site_order.
        """
        args_array = (c_char_p * len(self.clang_args))(
            *[arg.encode("utf-8") for arg in self.clang_args]
        )
        parser = self.lib.createParser(
            self.file_path.encode("utf-8"), args_array, len(self.clang_args)
        )
        if not parser:
            raise RuntimeError("Failed to create parser")

        try:
            if not self.lib.parseFile(parser):
                raise RuntimeError("Failed to parse file")

            self.lib.generateExtendedCallGraph(parser)

            json_result = self.lib.getCallGraphJSON(parser)
            if not json_result:
                raise RuntimeError("Failed to get call graph JSON")

            return json.loads(json_result.decode("utf-8"))
        finally:
            self.lib.destroyParser(parser)
