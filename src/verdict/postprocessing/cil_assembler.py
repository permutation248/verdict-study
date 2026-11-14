"""Annotated CIL file assembler.

Combines headers, declarations, and annotated functions into a single
C file for Frama-C verification.
"""

from __future__ import annotations

import logging
from typing import List

logger = logging.getLogger(__name__)


class AnnotatedCILFileAssembler:
    """Assemble the final annotated CIL file."""

    def assemble_annotated_cil(
        self,
        headers: List[str],
        ordered_declarations: List[str],
        annotated_functions: List[str],
        output_file_path: str,
    ) -> bool:
        """Write headers + declarations + annotated functions to *output_file_path*."""
        logger.debug("Assembling annotated CIL file at %s", output_file_path)
        try:
            with open(output_file_path, "w") as f:
                if headers:
                    f.write("\n".join(headers) + "\n\n")
                if ordered_declarations:
                    f.write("\n".join(ordered_declarations) + "\n\n")
                if annotated_functions:
                    f.write("\n\n".join(annotated_functions))
            return True
        except Exception as exc:
            logger.error("Error generating annotated file: %s", exc)
            return False
