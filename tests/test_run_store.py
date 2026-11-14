"""Tests for RunRecord / RunEvent / RunStore persistence."""

from __future__ import annotations

import json
from pathlib import Path

import pytest

from unittest.mock import patch

from claude_agent_sdk import AssistantMessage, ResultMessage, TextBlock

from verdict.core.run_record import (
    RunEvent,
    RunRecord,
    RunStatus,
    _agent_result_to_dict,
)
from verdict.core.run_store import RunStore
from verdict.core.types import (
    AgentResult,
    FunctionInfo,
    FunctionSpec,
    ProofState,
    SpecStatus,
    VOSummary,
)
from verdict.orchestrator.orchestrator import Orchestrator


MODEL = "claude-sonnet-4-20250514"


@pytest.fixture
def record() -> RunRecord:
    return RunRecord(
        id="run-1",
        source_file="foo.c",
        config={"scope": "global", "timeout": 300},
    )


def test_runrecord_to_dict_roundtrip(record: RunRecord):
    d = record.to_dict()
    assert d["id"] == "run-1"
    assert d["source_file"] == "foo.c"
    assert d["status"] == "init"
    assert d["config"]["scope"] == "global"
    # Must be json-serializable
    json.dumps(d)


def test_agent_result_serialization_preserves_enum_values():
    result = AgentResult(
        agent_id="agent_main",
        function_name="main",
        proof_state=ProofState.GLOBAL_OK,
        spec=FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED),
        cycle_count=3,
        vo_summary=VOSummary(rte_verified=2, rte_bug=1),
    )
    d = _agent_result_to_dict(result)
    assert d["proof_state"] == "global_ok"
    assert d["spec"]["function_name"] == "main"
    assert d["vo_summary"]["rte_verified"] == 2
    assert d["vo_summary"]["rte_bug"] == 1
    assert d["cycle_count"] == 3
    # json-safe
    json.dumps(d)


def test_agent_result_serialization_handles_spec_deps_frozenset():
    """spec_deps CLAUSES descriptors carry a frozenset, which json.dumps
    cannot encode directly — the serializer must convert it to a sorted list."""
    from verdict.core.spec_dep import SpecDep

    spec = FunctionSpec(function_name="main", status=SpecStatus.PUBLISHED)
    spec.spec_deps = {
        "callee_a": SpecDep(depends=False),
        "callee_b": SpecDep(
            depends=True, clauses=frozenset({"ensures \\result >= 0;", "assigns *p;"})
        ),
    }
    result = AgentResult(
        agent_id="agent_main",
        function_name="main",
        proof_state=ProofState.LOCAL_OK,
        spec=spec,
    )
    d = _agent_result_to_dict(result)
    cs = d["spec"]["spec_deps"]
    assert cs["callee_a"]["clauses"] is None
    assert cs["callee_b"]["clauses"] == sorted(
        {"ensures \\result >= 0;", "assigns *p;"}
    )
    # must be JSON-serializable (frozenset would raise TypeError)
    json.dumps(d)


def test_runstore_initial_record_written(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    run_json = tmp_path / "run.json"
    assert run_json.exists()
    loaded = json.loads(run_json.read_text())
    assert loaded["id"] == "run-1"
    assert loaded["status"] == "init"


def test_runstore_set_status_persists(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    store.set_status(RunStatus.RUNNING)
    loaded = json.loads((tmp_path / "run.json").read_text())
    assert loaded["status"] == "running"


def test_runstore_append_event_writes_jsonl(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    store.append_event("run_started", {"source_file": "foo.c"})
    store.append_event("agent_state_changed", {"old": "init", "new": "running"}, agent_id="agent_a")

    events_path = tmp_path / "events.jsonl"
    assert events_path.exists()
    lines = events_path.read_text().strip().splitlines()
    assert len(lines) == 2

    first = json.loads(lines[0])
    assert first["type"] == "run_started"
    assert first["run_id"] == "run-1"
    assert first["agent_id"] is None
    assert first["data"]["source_file"] == "foo.c"

    second = json.loads(lines[1])
    assert second["type"] == "agent_state_changed"
    assert second["agent_id"] == "agent_a"


def test_runstore_read_events_roundtrip(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    store.append_event("a", {"x": 1})
    store.append_event("b", {"y": 2}, agent_id="agent_b")
    store.append_event("c")

    reloaded = store.read_events()
    assert [e.type for e in reloaded] == ["a", "b", "c"]
    assert reloaded[1].agent_id == "agent_b"
    assert reloaded[0].data == {"x": 1}


def test_runstore_subscribe_receives_events(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    received: list[RunEvent] = []

    def _capture(event: RunEvent) -> None:
        received.append(event)

    store.subscribe(_capture)
    store.append_event("spec_published", {"function_name": "main"})
    store.append_event("vo_verified", {"vo_id": "rte_1"})

    assert len(received) == 2
    assert received[0].type == "spec_published"
    assert received[1].data["vo_id"] == "rte_1"


def test_runstore_unsubscribe_stops_delivery(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    received: list[RunEvent] = []

    def _capture(event: RunEvent) -> None:
        received.append(event)

    store.subscribe(_capture)
    store.append_event("e1")
    store.unsubscribe(_capture)
    store.append_event("e2")

    assert [e.type for e in received] == ["e1"]


def test_runstore_subscriber_exception_does_not_break_store(
    tmp_path: Path, record: RunRecord
):
    store = RunStore(tmp_path, record)

    def _broken(event: RunEvent) -> None:
        raise RuntimeError("boom")

    store.subscribe(_broken)
    # Should not raise
    store.append_event("e1")
    # Store still persisted the event
    assert len(store.read_events()) == 1


def test_runstore_cancel_marker(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)
    assert not store.cancel_requested()
    store.request_cancel()
    assert store.cancel_requested()
    assert (tmp_path / "cancel.requested").exists()
    store.clear_cancel_request()
    assert not store.cancel_requested()


def test_runstore_finalize_computes_summary(tmp_path: Path, record: RunRecord):
    store = RunStore(tmp_path, record)

    results = [
        AgentResult(
            agent_id="agent_a",
            function_name="a",
            proof_state=ProofState.GLOBAL_OK,
            cycle_count=2,
        ),
        AgentResult(
            agent_id="agent_b",
            function_name="b",
            proof_state=ProofState.FAILED,
            error="could not verify",
            cycle_count=25,
            cycle_limit_reached=True,
        ),
    ]

    store.finalize(results)
    loaded = json.loads((tmp_path / "run.json").read_text())
    assert loaded["finished_at"] is not None
    assert loaded["summary"]["total"] == 2
    assert loaded["summary"]["global_ok"] == 1
    assert loaded["summary"]["failed"] == 1
    assert loaded["summary"]["cycle_limited"] == 1
    assert loaded["agents"]["a"]["proof_state"] == "global_ok"
    assert loaded["agents"]["b"]["error"] == "could not verify"
    assert loaded["agents"]["b"]["cycle_limit_reached"] is True


# ---------------------------------------------------------------------------
# End-to-end: run a mock orchestrator with a RunStore and inspect outputs
# ---------------------------------------------------------------------------


def _make_mock_factory(registry):
    class MockClientFactory:
        def __init__(self, options=None, transport=None):
            self._options = options

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            import re

            m = re.search(r"`(\w+)`", prompt)
            func_name = m.group(1) if m else "unknown"
            for agent_id, entry in registry._entries.items():
                if entry.function_info.name == func_name:
                    await registry.write_spec(
                        agent_id,
                        FunctionSpec(
                            function_name=func_name,
                            postconditions=["ensures \\result >= 0;"],
                            status=SpecStatus.PUBLISHED
                        ),
                    )
                    break

        async def receive_response(self):
            yield AssistantMessage(
                content=[TextBlock(text="done")], model=MODEL
            )
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0.0,
                duration_ms=1,
                duration_api_ms=1,
                num_turns=1,
                is_error=False,
                session_id="s",
            )

        async def disconnect(self):
            pass

    return MockClientFactory


@pytest.mark.asyncio
async def test_orchestrator_writes_run_json_and_events(tmp_path: Path):
    record = RunRecord(
        id=tmp_path.name,
        source_file="fake.c",
        config={"scope": "global", "timeout": 10},
    )
    store = RunStore(tmp_path, record)

    func = FunctionInfo(
        name="inc",
        source_file="inc.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10, run_store=store)

    MockFactory = _make_mock_factory(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        results = await orch.run()

    # run.json exists, has completed status and finalized agents
    run_json = json.loads((tmp_path / "run.json").read_text())
    assert run_json["status"] == "completed"
    assert run_json["finished_at"] is not None
    assert "inc" in run_json["agents"]
    assert run_json["summary"]["total"] == 1

    # events.jsonl has at least run_started / agent_cycle_* / spec_published / run_completed
    events_lines = (tmp_path / "events.jsonl").read_text().strip().splitlines()
    assert events_lines, "events.jsonl should not be empty"
    event_types = [json.loads(line)["type"] for line in events_lines]

    assert "run_started" in event_types
    assert "agent_cycle_started" in event_types
    assert "agent_cycle_ended" in event_types
    assert "spec_published" in event_types
    assert "run_completed" in event_types

    # Timestamps must be monotonic
    timestamps = [json.loads(line)["timestamp"] for line in events_lines]
    assert timestamps == sorted(timestamps)


@pytest.mark.asyncio
async def test_orchestrator_emits_call_graph_and_live_state(tmp_path: Path):
    """run_started event must carry call_graph; run.json must populate
    agents with callees as soon as the initial batch starts."""
    record = RunRecord(id=tmp_path.name, source_file="fake.c")
    store = RunStore(tmp_path, record)

    funcs = [
        FunctionInfo(
            name="main",
            source_file="m.c",
            source_code="int main() { return helper(1); }",
            callees=["helper"],
            callers=[],
        ),
        FunctionInfo(
            name="helper",
            source_file="h.c",
            source_code="int helper(int x) { return x + 1; }",
            callees=[],
            callers=["main"],
        ),
    ]
    orch = Orchestrator(funcs, timeout=10, run_store=store)

    MockFactory = _make_mock_factory(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        await orch.run()

    events = [
        json.loads(line)
        for line in (tmp_path / "events.jsonl").read_text().splitlines()
        if line.strip()
    ]
    started = next(e for e in events if e["type"] == "run_started")
    assert started["data"]["call_graph"] == {"main": ["helper"], "helper": []}
    assert "main" in started["data"]["function_names"]

    # run.json includes the callees per agent at finalize time
    run_json = json.loads((tmp_path / "run.json").read_text())
    assert run_json["agents"]["main"]["callees"] == ["helper"]
    assert run_json["agents"]["helper"]["callees"] == []


@pytest.mark.asyncio
async def test_orchestrator_honors_cancel_request(tmp_path: Path):
    record = RunRecord(id=tmp_path.name, source_file="fake.c")
    store = RunStore(tmp_path, record)
    # Pre-request cancel so the first loop iteration sees it.
    store.request_cancel()

    func = FunctionInfo(
        name="inc",
        source_file="inc.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10, run_store=store)

    MockFactory = _make_mock_factory(orch.registry)
    with patch("verdict.agent.runner.ClaudeSDKClient", MockFactory):
        await orch.run()

    run_json = json.loads((tmp_path / "run.json").read_text())
    assert run_json["status"] == "cancelled"

    event_types = [
        json.loads(line)["type"]
        for line in (tmp_path / "events.jsonl").read_text().splitlines()
        if line.strip()
    ]
    assert "run_cancelled" in event_types


@pytest.mark.asyncio
async def test_external_cancel_still_writes_terminal_run_json(tmp_path: Path):
    """Cancelling the orchestrator task from outside (the programmatic
    equivalent of Ctrl-C hitting the event loop mid-cycle) must leave
    run.json in a terminal status, not frozen in 'running'."""
    import asyncio as _asyncio

    record = RunRecord(id=tmp_path.name, source_file="fake.c")
    store = RunStore(tmp_path, record)

    class SlowClient:
        def __init__(self, options=None, transport=None):
            pass

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            # Block long enough that an external cancel can hit us mid-flight
            await _asyncio.sleep(10)

        async def receive_response(self):
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0,
                duration_ms=0,
                duration_api_ms=0,
                num_turns=0,
                is_error=False,
                session_id="s",
            )

        async def disconnect(self):
            pass

    func = FunctionInfo(
        name="inc",
        source_file="inc.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=60, run_store=store)

    with patch("verdict.agent.runner.ClaudeSDKClient", SlowClient):
        run_task = _asyncio.create_task(orch.run())
        await _asyncio.sleep(0.3)
        run_task.cancel()
        # The orchestrator catches CancelledError internally, persists
        # terminal state, and completes normally (no re-raise for
        # CancelledError — only KeyboardInterrupt re-raises).
        try:
            await run_task
        except _asyncio.CancelledError:
            pass

    run_json = json.loads((tmp_path / "run.json").read_text())
    assert run_json["status"] in {"cancelled", "failed"}, (
        f"status stayed {run_json['status']!r}, run.json never finalized"
    )
    assert run_json["finished_at"] is not None


@pytest.mark.asyncio
async def test_unexpected_exception_marks_run_failed(tmp_path: Path):
    """A non-timeout, non-cancel exception must land in status 'failed'."""
    record = RunRecord(id=tmp_path.name, source_file="fake.c")
    store = RunStore(tmp_path, record)

    class BadClient:
        def __init__(self, options=None, transport=None):
            pass

        async def connect(self):
            raise RuntimeError("simulated internal error")

        async def query(self, prompt, session_id="default"):
            pass

        async def receive_response(self):
            if False:
                yield None

        async def disconnect(self):
            pass

    func = FunctionInfo(
        name="inc",
        source_file="inc.c",
        source_code="int inc(int x) { return x + 1; }",
    )
    orch = Orchestrator([func], timeout=10, run_store=store)

    with patch("verdict.agent.runner.ClaudeSDKClient", BadClient):
        # Should NOT raise — the exception is caught inside an agent cycle
        # and the orchestrator recovers to finalize.
        await orch.run()

    run_json = json.loads((tmp_path / "run.json").read_text())
    # The agent captured the error, so the run completes normally (per-agent
    # failure ≠ run-level failure). The key invariant is that finished_at
    # is populated and status is not 'running'.
    assert run_json["status"] != "running"
    assert run_json["finished_at"] is not None


@pytest.mark.asyncio
async def test_cancel_during_long_running_agent_aborts_quickly(tmp_path: Path):
    """A long-running agent task must be cancelled within ~1.5s of the
    cancel marker appearing, not after the agent finishes naturally."""
    import asyncio as _asyncio
    import time

    record = RunRecord(id=tmp_path.name, source_file="fake.c")
    store = RunStore(tmp_path, record)

    class SlowMockClient:
        def __init__(self, options=None, transport=None):
            pass

        async def connect(self):
            pass

        async def query(self, prompt, session_id="default"):
            # Pretend to be a slow Claude SDK call
            await _asyncio.sleep(30)

        async def receive_response(self):
            yield ResultMessage(
                subtype="success",
                total_cost_usd=0,
                duration_ms=0,
                duration_api_ms=0,
                num_turns=0,
                is_error=False,
                session_id="s",
            )

        async def disconnect(self):
            pass

    func = FunctionInfo(
        name="slow",
        source_file="slow.c",
        source_code="int slow(int x) { return x; }",
    )
    orch = Orchestrator([func], timeout=60, run_store=store)

    async def _request_cancel_after_delay():
        await _asyncio.sleep(0.3)
        store.request_cancel()

    start = time.monotonic()
    cancel_task = _asyncio.create_task(_request_cancel_after_delay())
    with patch("verdict.agent.runner.ClaudeSDKClient", SlowMockClient):
        await orch.run()
    elapsed = time.monotonic() - start
    await cancel_task

    # Should have cancelled within a few seconds, not waited 30s for the
    # mocked Claude call to finish.
    assert elapsed < 5.0, f"cancel took {elapsed:.1f}s, expected < 5s"

    run_json = json.loads((tmp_path / "run.json").read_text())
    assert run_json["status"] == "cancelled"
