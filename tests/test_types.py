"""Tests for core data types."""

from verdict.core.types import (
    DemandResolved,
    DemandStatus,
    FunctionSpec,
    ProofState,
    SpecStatus,
    SpecType,
    ProofDemand,
    SpecificationRequest,
    SpecificationResponse,
    SpecificationUpdate,
    CascadeNotification,
    AgentResult,
    FunctionInfo,
)


def test_proof_state_values():
    assert ProofState.EMPTY.value == "empty"
    assert ProofState.LOCAL_OK.value == "local_ok"
    assert ProofState.GLOBAL_OK.value == "global_ok"
    assert ProofState.FAILED.value == "failed"


def test_spec_type_loop_contract():
    assert SpecType.LOOP_CONTRACT.value == "loop_contract"
    assert SpecType.POSTCONDITION.value == "postcondition"


def test_function_spec_defaults():
    spec = FunctionSpec(function_name="foo")
    assert spec.preconditions == []
    assert spec.postconditions == []
    assert spec.loop_contracts == {}
    assert spec.assigns == []
    assert spec.status == SpecStatus.DRAFT
    assert spec.version == 0


def test_function_spec_loop_contracts():
    spec = FunctionSpec(
        function_name="foo",
        loop_contracts={"while_1": ["loop invariant i >= 0;", "loop assigns i;"]},
    )
    assert "while_1" in spec.loop_contracts
    assert len(spec.loop_contracts["while_1"]) == 2


def test_specification_request():
    req = SpecificationRequest(
        requester_id="agent_a",
        target_function="func_b",
        request_type=SpecType.POSTCONDITION,
        required_property="\\result >= 0",
    )
    assert req.requester_id == "agent_a"
    assert req.context == {}


def test_agent_result():
    result = AgentResult(
        agent_id="agent_x",
        function_name="x",
        proof_state=ProofState.LOCAL_OK,
    )
    assert result.error is None
    assert result.spec is None


def test_demand_status_values():
    assert DemandStatus.OPEN.value == "open"
    assert DemandStatus.SATISFIED.value == "satisfied"
    assert DemandStatus.REJECTED.value == "rejected"


def test_proof_demand_defaults():
    demand = ProofDemand(
        requester_id="agent_main",
        target_function="callee",
        property_kind="postcondition",
        property_text="ensures \\result >= 0;",
    )
    assert demand.blocking_vo_ids == []
    assert demand.requester_dependency_snapshot == {}
    assert demand.demand_id == ""
    assert demand.status == DemandStatus.OPEN
    assert demand.resolution_message is None
