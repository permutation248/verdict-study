"""Build real ProjectContext + ToolContext from raw C source via full pipeline.

No mocking — runs the real pipeline via verdict.preprocessing.pipeline.
"""

from __future__ import annotations

import logging

from verdict.analysis.extended_call_graph import ExtendedCallGraph
from verdict.core.project_context import ProjectContext
from verdict.core.types import FunctionInfo
from verdict.core.vo_tracker import VOEntry, VOTracker
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus
from verdict.tools.context import ToolContext

logger = logging.getLogger(__name__)


def build_real_project_context(
    source_file: str,
    header_files: list[str] | None = None,
) -> tuple[ProjectContext, ExtendedCallGraph]:
    """Run the full pipeline on a raw C source and build a real ProjectContext.

    Delegates to verdict.preprocessing.pipeline.preprocess().

    Returns (ProjectContext, ExtendedCallGraph) for further inspection.
    """
    from verdict.preprocessing.pipeline import preprocess

    return preprocess(source_file, header_files=header_files)


def build_tool_context(
    func_name: str,
    project_ctx: ProjectContext,
    vo_tracker: VOTracker | None = None,
) -> ToolContext:
    """Build a ToolContext for the named function using real ProjectContext."""
    node = project_ctx.call_graph_dict.get(func_name)
    if node is None:
        raise ValueError(
            f"Function '{func_name}' not found in call graph. "
            f"Available: {list(project_ctx.call_graph_dict.keys())}"
        )

    info = FunctionInfo(
        name=func_name,
        source_file=project_ctx.source_file,
        source_code=node.source_code or "",
    )

    agent_id = f"agent_{func_name}"
    registry = AgentRegistry()
    entry = AgentEntry(agent_id=agent_id, function_info=info)
    registry.register(entry)

    bus = AsyncQueueBus()

    return ToolContext(
        agent_id=agent_id,
        function_info=info,
        registry=registry,
        message_bus=bus,
        logger=logging.getLogger(f"verdict.test.{func_name}"),
        project_context=project_ctx,
        vo_tracker=vo_tracker,
    )


def build_vo_tracker_for_function(
    func_name: str,
    project_ctx: ProjectContext,
) -> VOTracker:
    """Build a VOTracker pre-populated with RTE VOs for the given function."""
    tracker = VOTracker()
    rte_vos = project_ctx.rte_vos_by_function.get(func_name, [])
    for vo_id in rte_vos:
        tracker.add_rte_vo(VOEntry(
            vo_id=vo_id,
            vo_type="rte",
            description=f"RTE assertion {vo_id} in {func_name}",
            related_function=func_name,
        ))
    return tracker
