"""MessageBus: async message transport between agents."""

from __future__ import annotations

import asyncio
from collections import deque
from typing import Protocol, runtime_checkable

from verdict.core.types import AgentMessage


@runtime_checkable
class MessageBus(Protocol):
    """Protocol for inter-agent message delivery."""

    async def send(self, recipient_id: str, message: AgentMessage) -> None: ...
    async def receive(self, agent_id: str) -> AgentMessage: ...
    def has_messages(self, agent_id: str) -> bool: ...
    def message_count(self, agent_id: str) -> int: ...
    def peek_messages(self, agent_id: str) -> list[AgentMessage]: ...


class AsyncQueueBus:
    """MessageBus implementation backed by per-agent asyncio.Queues."""

    def __init__(self) -> None:
        self._queues: dict[str, asyncio.Queue[AgentMessage]] = {}
        self._shadow_queues: dict[str, deque[AgentMessage]] = {}

    def _ensure_queue(self, agent_id: str) -> asyncio.Queue[AgentMessage]:
        if agent_id not in self._queues:
            self._queues[agent_id] = asyncio.Queue()
        return self._queues[agent_id]

    def _ensure_shadow_queue(self, agent_id: str) -> deque[AgentMessage]:
        if agent_id not in self._shadow_queues:
            self._shadow_queues[agent_id] = deque()
        return self._shadow_queues[agent_id]

    async def send(self, recipient_id: str, message: AgentMessage) -> None:
        q = self._ensure_queue(recipient_id)
        shadow = self._ensure_shadow_queue(recipient_id)
        await q.put(message)
        shadow.append(message)

    async def receive(self, agent_id: str) -> AgentMessage:
        q = self._ensure_queue(agent_id)
        shadow = self._ensure_shadow_queue(agent_id)
        message = await q.get()
        if shadow:
            shadow.popleft()
        return message

    def has_messages(self, agent_id: str) -> bool:
        q = self._queues.get(agent_id)
        return q is not None and not q.empty()

    def message_count(self, agent_id: str) -> int:
        q = self._queues.get(agent_id)
        return q.qsize() if q is not None else 0

    def peek_messages(self, agent_id: str) -> list[AgentMessage]:
        shadow = self._shadow_queues.get(agent_id)
        if shadow is None:
            return []
        return list(shadow)
