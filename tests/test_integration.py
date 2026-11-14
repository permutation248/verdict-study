"""Integration tests using real ExtendedCallGraphNode objects (no frama-c)."""

from verdict.analysis.extended_call_graph import ExtendedCallGraphNode
from verdict.analysis.verification_obligation_graph import (
    VerificationObligationGraph,
    AssertionType,
)
from verdict.tools._mechanical import extract_context, compute_weakening_effects


def _node_json(name, source, rte_asserts=None, call_sites=None, loops=None):
    """Minimal JSON for ExtendedCallGraphNode.from_json."""
    return {
        "func_name": name,
        "source_code": source,
        "is_definition": True,
        "extent": {"start_line": 1, "start_column": 1, "end_line": 5, "end_column": 1},
        "loops": loops or [],
        "rte_asserts": rte_asserts or [],
        "call_sites": call_sites or [],
        "global_vars_used": [],
    }


def _build_graph():
    """Build a small 2-function call graph."""
    helper = ExtendedCallGraphNode.from_json(_node_json(
        "helper", "int helper(int x){ return x+1; }",
        rte_asserts=[{
            # Post-assigner CIL content: carries the rte_N label the VO graph
            # now reads (the assigner is the single numbering authority).
            "content": "/*@ assert rte_1: signed_overflow: x+1 <= 2147483647; */",
            "extent": {"start_line": 1, "start_column": 1, "end_line": 1, "end_column": 10},
        }],
    ))
    main_fn = ExtendedCallGraphNode.from_json(_node_json(
        "main_fn", "int main_fn(){ return helper(1); }",
        call_sites=[{
            "callee_func_name": "helper",
            "extent": {"start_line": 1, "start_column": 20, "end_line": 1, "end_column": 30},
        }],
    ))
    # Establish relationships
    main_fn.children.append(helper)
    helper.parents.append(main_fn)
    return {"main_fn": main_fn, "helper": helper}


def test_extract_context_with_real_nodes():
    graph = _build_graph()
    result = extract_context(graph, "main_fn", depth=1)
    assert result["function_name"] == "main_fn"
    assert len(result["callee_layers"]) >= 1


def test_extract_context_include_callers():
    graph = _build_graph()
    result = extract_context(graph, "helper", depth=1, include_callers=True)
    assert result["function_name"] == "helper"
    assert result["callers"] is not None
    assert any(c["name"] == "main_fn" for c in result["callers"])


def test_vo_graph_generation():
    graph = _build_graph()
    vog = VerificationObligationGraph()
    nodes = vog.generate_from_extended_call_graph(graph)
    assert len(nodes) > 0

    # Should have at least one RTE and one call-site VO
    rte_nodes = [n for n in nodes.values() if n.assertion_type == AssertionType.RTE]
    cs_nodes = [n for n in nodes.values() if n.assertion_type == AssertionType.CALL_SITE]
    assert len(rte_nodes) >= 1
    assert len(cs_nodes) >= 1

    # RTE node should be for helper
    assert rte_nodes[0].func_name == "helper"
    # Call-site node should be for main_fn
    assert cs_nodes[0].func_name == "main_fn"


def test_compute_weakening_with_real_nodes():
    graph = _build_graph()
    result = compute_weakening_effects(graph, "helper")
    assert "main_fn" in result["affected_callers"]
