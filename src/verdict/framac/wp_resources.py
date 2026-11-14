"""Process-wide WP resource budget — the single home for WP CPU policy.

One WP run spawns up to ``procs`` prover PROCESSES (≈ one core each, per the
Frama-C WP manual: ``-wp-par`` "limits the number of parallel process runs for
decision procedures"); ``jobs`` of those runs proceed at once, gated by a
shared semaphore. So peak cores ≈ ``jobs × procs``.

This is deliberately DECOUPLED from agent parallelism (``--max-parallel``):
agents are bounded by the LLM API, not the CPU, so hundreds can be in flight
while only ``jobs`` frama-c processes actually run. The command builder
reads :func:`procs`, the runner's concurrency gate reads :func:`semaphore`, and
the orchestrator calls :func:`configure` once before the solve loop.
"""

from __future__ import annotations

import asyncio
import logging
import os

logger = logging.getLogger(__name__)


def _cpu_count() -> int:
    return os.cpu_count() or 8


_procs: int = 8
_jobs: int = max(1, _cpu_count() // 8)
_semaphore: asyncio.Semaphore | None = None
_model: str | None = None


def configure(
    procs: int = 8,
    jobs: int | None = None,
    model: str | None = None,
) -> tuple[int, int]:
    """Set the WP budget; returns the resolved ``(procs, jobs)``.

    ``jobs`` defaults to ``ncpu // procs`` — fills the machine without peak
    oversubscription. An explicit value is honoured (e.g. to leave headroom on
    a shared host, or to mildly oversubscribe since WP rarely uses all ``procs``
    slots per run), with a warning when ``procs × jobs`` exceeds the cores.

    ``model`` sets ``-wp-model`` (e.g. ``Typed+var+int+real`` for the
    SpecSyn-evaluation semantics ablation: unbounded integers + real
    arithmetic instead of IEEE floats — unsound w.r.t. machine semantics,
    use only for evaluation-model comparisons). ``None`` (default) omits
    the flag → Frama-C's default sound ``Typed`` model.
    """
    global _procs, _jobs, _semaphore, _model
    ncpu = _cpu_count()
    _procs = max(1, procs)
    _jobs = max(1, jobs if jobs is not None else ncpu // _procs)
    _model = model or None
    if _procs * _jobs > ncpu:
        logger.warning(
            "WP oversubscribe: procs(%d) × jobs(%d) = %d > %d cores",
            _procs, _jobs, _procs * _jobs, ncpu,
        )
    if _model:
        logger.warning(
            "WP model override: -wp-model %s (non-default proving semantics)",
            _model,
        )
    _semaphore = None  # recreated lazily at the new size in the running loop
    return _procs, _jobs


def procs() -> int:
    """Prover processes per WP run (frama-c ``-wp-par``)."""
    return _procs


def model() -> str | None:
    """The ``-wp-model`` override, or None for Frama-C's default."""
    return _model


def jobs() -> int:
    """Max concurrent WP runs."""
    return _jobs


def semaphore() -> asyncio.Semaphore:
    """The shared gate bounding concurrent frama-c processes to ``jobs``."""
    global _semaphore
    if _semaphore is None:
        _semaphore = asyncio.Semaphore(_jobs)
    return _semaphore
