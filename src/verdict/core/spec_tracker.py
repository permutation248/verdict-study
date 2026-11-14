"""SpecTracker: live annotated C source rendering.

Subscribes to `spec_published` events from `RunStore` and regenerates
`current_specs.c` in the run directory each time a new authoritative
version lands. This file is intended for humans skimming the current
verified contracts alongside the source code.

State-level history (what each function published, when) now lives in
`events.jsonl` via the RunStore; we no longer maintain
`spec_history.jsonl` or `state_cache.json`.
"""

from __future__ import annotations

import copy
import logging
from datetime import datetime, timezone
from pathlib import Path
from typing import TYPE_CHECKING

from verdict.core.types import FunctionSpec, SpecStatus

if TYPE_CHECKING:
    from verdict.core.project_context import ProjectContext
    from verdict.core.run_record import RunEvent
    from verdict.core.run_store import RunStore

logger = logging.getLogger(__name__)


class SpecTracker:
    """Renders `current_specs.c` in response to spec-publication events."""

    def __init__(
        self,
        run_dir: Path,
        project_ctx: ProjectContext,
        run_store: RunStore | None = None,
    ) -> None:
        self._run_dir = Path(run_dir)
        self._project_ctx = project_ctx
        self._specs: dict[str, FunctionSpec] = {}
        self._version = 0
        self._annotated_path = self._run_dir / "current_specs.c"

        # Directly-written updates from `update_my_spec` are still supported
        # for unit tests that don't go through the event stream.
        self._run_store = run_store
        if run_store is not None:
            run_store.subscribe(self._on_event)

    def record_update(self, agent_id: str, spec: FunctionSpec) -> None:
        """Accept a spec update directly (used when no event stream is wired)."""
        self._version += 1
        self._specs[spec.function_name] = spec
        self._regenerate_annotated_file()

    # ------------------------------------------------------------------
    # Event subscription path
    # ------------------------------------------------------------------

    def _on_event(self, event: RunEvent) -> None:
        if event.type != "spec_published":
            return
        data = event.data
        function_name = data.get("function_name")
        if not function_name:
            return
        spec_blob = data.get("spec") or {}
        self._version += 1
        self._specs[function_name] = FunctionSpec(
            function_name=function_name,
            preconditions=list(spec_blob.get("preconditions", []) or []),
            postconditions=list(spec_blob.get("postconditions", []) or []),
            loop_contracts=dict(spec_blob.get("loop_contracts", {}) or {}),
            assigns=list(spec_blob.get("assigns", []) or []),
            version=int(data.get("version", 0) or 0),
            status=SpecStatus.PUBLISHED,
            based_on_versions=dict(spec_blob.get("based_on_versions", {}) or {}),
        )
        self._regenerate_annotated_file()

    # ------------------------------------------------------------------
    # Annotated source rendering
    # ------------------------------------------------------------------

    def _regenerate_annotated_file(self) -> None:
        from verdict.annotation.function_annotator import FunctionAnnotator
        from verdict.tools._cil_builder import _apply_annotations_to_node

        call_graph = self._project_ctx.call_graph_dict
        annotator = FunctionAnnotator(self._project_ctx.function_declarations_map)

        ts = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%S")
        lines = [
            f"/* Verdict Specification Snapshot",
            f" * Source: {self._project_ctx.source_file}",
            f" * Version: {self._version} | Updated: {ts}",
            f" */",
            "",
        ]

        for func_name, node in call_graph.items():
            spec = self._specs.get(func_name)
            status = "unverified"

            if spec is not None:
                if spec.status == SpecStatus.PUBLISHED:
                    status = "VERIFIED"
                annotations = {
                    "preconditions": spec.preconditions,
                    "postconditions": spec.postconditions,
                    "loop_contracts": spec.loop_contracts,
                    "assigns": spec.assigns,
                }
                node_copy = copy.deepcopy(node)
                _apply_annotations_to_node(node_copy, annotations)
                ok, annotated = annotator.get_function_with_annotations(node_copy)
                if ok and annotated:
                    source = annotated
                else:
                    source = getattr(node, "source_code", "") or ""
            else:
                source = getattr(node, "source_code", "") or ""

            lines.append(f"/* ── {func_name} [{status}] {'─' * max(1, 40 - len(func_name))} */")
            lines.append(source)
            lines.append("")

        try:
            self._annotated_path.write_text("\n".join(lines))
        except Exception:
            logger.exception("Failed to write annotated specs file")
