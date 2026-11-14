"""RunStore: persistence + in-process subscriptions for solve runs.

Responsibilities:
- Persist `run.json` (RunRecord snapshot) and `events.jsonl` (append-only
  event stream) under a run's log directory.
- Notify in-process subscribers whenever a new event is appended. The CLI
  progress printer and the annotated-source renderer subscribe.
- Provide a cooperative cancel mechanism via a `cancel.requested` marker
  file that the orchestrator polls between loop iterations.

Thread-safety: every mutation is guarded by a single threading.Lock, so the
store stays safe even when accessed from more than one thread.
"""

from __future__ import annotations

import json
import logging
import threading
from dataclasses import asdict
from pathlib import Path
from typing import TYPE_CHECKING, Any, Callable

from verdict.core.run_record import RunEvent, RunRecord, RunStatus, _now_iso

if TYPE_CHECKING:
    from verdict.core.types import AgentResult

logger = logging.getLogger(__name__)


class RunStore:
    """Owns one run's persistence + subscriber fan-out."""

    def __init__(self, run_dir: Path, record: RunRecord) -> None:
        self.run_dir = Path(run_dir)
        self.record = record
        self._record_path = self.run_dir / "run.json"
        self._events_path = self.run_dir / "events.jsonl"
        self._cancel_path = self.run_dir / "cancel.requested"
        self._lock = threading.Lock()
        self._subscribers: list[Callable[[RunEvent], None]] = []

        self.run_dir.mkdir(parents=True, exist_ok=True)
        self.save_record()

    # ------------------------------------------------------------------
    # Record persistence
    # ------------------------------------------------------------------

    def save_record(self) -> None:
        """Write the current RunRecord snapshot to run.json atomically."""
        with self._lock:
            try:
                payload = json.dumps(
                    self.record.to_dict(), indent=2, ensure_ascii=False
                )
                self._record_path.write_text(payload + "\n", encoding="utf-8")
            except Exception:
                logger.exception("Failed to persist run record")

    def set_status(self, status: RunStatus) -> None:
        self.record.status = status
        self.save_record()

    def finalize(self, results: list["AgentResult"]) -> None:
        """Called at end-of-run: snapshot final AgentResults + summary."""
        self.record.finished_at = _now_iso()
        self.record.agents = {result.function_name: result for result in results}

        global_ok = sum(
            1
            for r in results
            if r.proof_state is not None and r.proof_state.value == "global_ok"
        )
        failed = sum(1 for r in results if r.error)
        self.record.summary = {
            "total": len(results),
            "global_ok": global_ok,
            "failed": failed,
            "cycle_limited": sum(1 for r in results if r.cycle_limit_reached),
        }
        self.save_record()

    # ------------------------------------------------------------------
    # Event stream
    # ------------------------------------------------------------------

    def append_event(
        self,
        type: str,
        data: dict[str, Any] | None = None,
        agent_id: str | None = None,
    ) -> RunEvent:
        """Append an event to events.jsonl and notify subscribers."""
        event = RunEvent(
            run_id=self.record.id,
            type=type,
            agent_id=agent_id,
            data=data or {},
        )

        with self._lock:
            try:
                with self._events_path.open("a", encoding="utf-8") as handle:
                    handle.write(
                        json.dumps(event.to_dict(), ensure_ascii=False) + "\n"
                    )
            except Exception:
                logger.exception("Failed to append run event")

        # Fire subscribers outside the lock so slow consumers don't block
        # event production. Copy the list to be safe if a subscriber
        # triggers another subscribe/unsubscribe.
        for subscriber in list(self._subscribers):
            try:
                subscriber(event)
            except Exception:
                logger.exception("run store subscriber failed")

        return event

    def subscribe(self, callback: Callable[[RunEvent], None]) -> None:
        """Register a subscriber that receives every new event."""
        with self._lock:
            self._subscribers.append(callback)

    def unsubscribe(self, callback: Callable[[RunEvent], None]) -> None:
        with self._lock:
            try:
                self._subscribers.remove(callback)
            except ValueError:
                pass

    def read_events(self) -> list[RunEvent]:
        """Load every persisted event for this run, in order."""
        if not self._events_path.exists():
            return []
        events: list[RunEvent] = []
        with self._events_path.open("r", encoding="utf-8") as handle:
            for line in handle:
                line = line.strip()
                if not line:
                    continue
                try:
                    payload = json.loads(line)
                    events.append(
                        RunEvent(
                            run_id=payload["run_id"],
                            type=payload["type"],
                            timestamp=payload.get("timestamp", ""),
                            agent_id=payload.get("agent_id"),
                            data=payload.get("data", {}),
                        )
                    )
                except (json.JSONDecodeError, KeyError):
                    logger.warning("Skipping malformed event line")
        return events

    # ------------------------------------------------------------------
    # Cooperative cancel
    # ------------------------------------------------------------------

    def cancel_requested(self) -> bool:
        return self._cancel_path.exists()

    def request_cancel(self) -> None:
        try:
            self._cancel_path.write_text("cancel\n", encoding="utf-8")
        except Exception:
            logger.exception("Failed to write cancel marker")

    def clear_cancel_request(self) -> None:
        try:
            self._cancel_path.unlink(missing_ok=True)
        except Exception:
            logger.exception("Failed to clear cancel marker")
