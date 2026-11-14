"""Entry point: python -m verdict.orchestrator"""

import asyncio

from verdict.core.types import FunctionInfo
from verdict.orchestrator.orchestrator import Orchestrator


def main() -> None:
    """Smoke-test: run orchestrator with a dummy function."""
    dummy = FunctionInfo(
        name="example_func",
        source_file="example.c",
        source_code="int example_func(int x) { return x + 1; }",
    )
    orch = Orchestrator([dummy], timeout=30)
    results = asyncio.run(orch.run())
    for r in results:
        proof = r.proof_state.value if r.proof_state is not None else "unknown"
        print(f"{r.function_name}: {proof} (error={r.error})")


if __name__ == "__main__":
    main()
