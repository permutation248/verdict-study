"""ACSL annotation semantic filter.

Catches semantic errors in ACSL annotations before they reach Frama-C WP.
The filter operates on annotation TEXT (no AST); it detects patterns that
are syntactically legal ACSL but semantically wrong in the WP proof context.

Current checks:
  - ``&param`` in memory predicates (``\\separated``, ``\\valid``,
    ``\\valid_read``): for ANY function parameter ``p``, ``&p`` refers to
    the stack slot holding the parameter, not the memory it points to.
    For pointer params this is trivially true and useless; for non-pointer
    params WP cannot take the address of a logic value and aborts.
    Both cases are errors — the intended expression is ``p`` (for pointers)
    or the annotation should be restructured (for non-pointers).
"""

from __future__ import annotations

import logging
import re
from typing import Any

logger = logging.getLogger(__name__)

_MEMORY_PREDICATE_RE = re.compile(
    r"\\(?:separated|valid|valid_read)\s*\("
)


def _extract_memory_predicate_args(annotation: str) -> list[str]:
    """Extract the argument strings of memory predicates in *annotation*.

    Returns one string per top-level predicate occurrence, containing
    everything inside the outermost parentheses.
    """
    args: list[str] = []
    for m in _MEMORY_PREDICATE_RE.finditer(annotation):
        start = m.end() - 1  # the opening '('
        depth = 0
        for i in range(start, len(annotation)):
            if annotation[i] == "(":
                depth += 1
            elif annotation[i] == ")":
                depth -= 1
                if depth == 0:
                    args.append(annotation[start + 1 : i])
                    break
    return args


def check_annotation(
    annotation: str,
    param_names: set[str],
) -> list[str]:
    """Check a single ACSL clause for semantic errors.

    Returns a list of error messages (empty = no errors).
    ``param_names`` is the set of ALL function parameter names (both
    pointer and non-pointer).
    """
    errors: list[str] = []
    for arg_text in _extract_memory_predicate_args(annotation):
        for m in re.finditer(r"&(\w+)", arg_text):
            name = m.group(1)
            if name not in param_names:
                continue
            rest = arg_text[m.end():]
            if rest.lstrip().startswith("->"):
                continue  # &param->field is fine (struct field address)
            errors.append(
                f"'&{name}' takes the address of parameter '{name}' "
                f"itself (its stack slot), not the memory it points to. "
                f"Use '{name}' directly in \\separated/\\valid/\\valid_read."
            )
    return errors


def check_annotations(
    clauses: list[str],
    param_names: set[str],
    label: str = "Annotation",
) -> tuple[bool, list[str]]:
    """Check a list of ACSL clauses. Returns (valid, error_messages)."""
    errors: list[str] = []
    for i, clause in enumerate(clauses):
        for err in check_annotation(clause, param_names):
            errors.append(f"{label} {i + 1}: {err}")
    return (not errors, errors)


def param_names_from_params(
    function_params: list[dict[str, Any]],
) -> set[str]:
    """Extract parameter names from the ``[{name, type}]`` list."""
    return {p["name"] for p in function_params if p.get("name")}


class AnnotationSemanticFilter:
    """Stateless filter — kept for backward compatibility with call sites."""

    def filter_preconditions(
        self,
        preconditions: list[str],
        function_params: list[dict[str, Any]],
    ) -> tuple[bool, list[str]]:
        return check_annotations(
            preconditions,
            param_names_from_params(function_params),
            label="Precondition",
        )

    def filter_single_function_annotations(
        self,
        preconditions: list[str],
        function_params: list[dict[str, Any]],
    ) -> tuple[bool, list[str]]:
        if not preconditions:
            return True, []
        return self.filter_preconditions(preconditions, function_params)
