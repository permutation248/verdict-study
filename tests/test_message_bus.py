"""Tests for AsyncQueueBus."""

import pytest

from verdict.core.types import SpecType, SpecificationRequest
from verdict.messaging.bus import AsyncQueueBus


@pytest.mark.asyncio
async def test_send_and_receive(bus):
    msg = SpecificationRequest(
        requester_id="a",
        target_function="f",
        request_type=SpecType.POSTCONDITION,
        required_property="\\result >= 0",
    )
    await bus.send("agent_b", msg)
    assert bus.has_messages("agent_b")
    got = await bus.receive("agent_b")
    assert got == msg


@pytest.mark.asyncio
async def test_has_messages_empty(bus):
    assert not bus.has_messages("nobody")


@pytest.mark.asyncio
async def test_multiple_messages_fifo(bus):
    for i in range(3):
        msg = SpecificationRequest(
            requester_id="a",
            target_function=f"f{i}",
            request_type=SpecType.POSTCONDITION,
            required_property=f"prop_{i}",
        )
        await bus.send("agent_x", msg)

    for i in range(3):
        got = await bus.receive("agent_x")
        assert got.target_function == f"f{i}"

    assert not bus.has_messages("agent_x")


@pytest.mark.asyncio
async def test_peek_messages_non_destructive(bus):
    msg = SpecificationRequest(
        requester_id="a",
        target_function="f",
        request_type=SpecType.POSTCONDITION,
        required_property="prop",
    )
    await bus.send("agent_y", msg)

    peeked = bus.peek_messages("agent_y")
    assert peeked == [msg]
    assert bus.has_messages("agent_y")

    got = await bus.receive("agent_y")
    assert got == msg
