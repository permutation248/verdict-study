"""Shared test fixtures for Verdict tests."""

import os
import shutil
import subprocess

import pytest

from verdict.core.types import FunctionInfo, FunctionSpec, SpecType
from verdict.agent.registry import AgentEntry, AgentRegistry
from verdict.messaging.bus import AsyncQueueBus


# ---------------------------------------------------------------------------
# Environment setup for integration tests (ClangParserBinding + Frama-C)
# ---------------------------------------------------------------------------

def _setup_env() -> None:
    """Source build_config.sh values into os.environ so ECG can find the dylib."""
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    bindings_dir = os.path.join(project_root, "src", "verdict", "preprocessing", "bindings")
    build_config = os.path.join(bindings_dir, "build_config.sh")

    os.environ.setdefault("PROJECT_ROOT", project_root)
    os.environ.setdefault("BINDINGS_DIR", bindings_dir)

    if os.path.isfile(build_config):
        # Parse shell variables from build_config.sh
        try:
            result = subprocess.run(
                ["bash", "-c", f"source {build_config} && env"],
                capture_output=True, text=True, timeout=5,
            )
            for line in result.stdout.splitlines():
                if "=" in line:
                    key, _, val = line.partition("=")
                    if key in ("FRAMAC_LIBC_PATH", "MACOS_SDK_PATH", "LLVM_ROOT", "JSONCPP_ROOT"):
                        os.environ.setdefault(key, val)
        except Exception:
            pass

    # Auto-detect Frama-C libc path if not set
    if "FRAMAC_LIBC_PATH" not in os.environ and shutil.which("frama-c"):
        try:
            result = subprocess.run(
                ["frama-c", "-print-share-path"],
                capture_output=True, text=True, timeout=5,
            )
            share = result.stdout.strip()
            libc = os.path.join(share, "libc")
            if os.path.isdir(libc):
                os.environ["FRAMAC_LIBC_PATH"] = libc
        except Exception:
            pass


_setup_env()


# ---------------------------------------------------------------------------
# Availability checks for integration test skip markers
# ---------------------------------------------------------------------------

FRAMAC_AVAILABLE = shutil.which("frama-c") is not None

_BINDINGS_DIR = os.environ.get("BINDINGS_DIR", "")
_LIB_EXT = "dylib" if os.uname().sysname == "Darwin" else "so"
BINDINGS_AVAILABLE = os.path.isfile(
    os.path.join(_BINDINGS_DIR, "build", f"libclang_parser.{_LIB_EXT}")
)

_BENCHMARKS_DIR = os.path.join(os.path.dirname(__file__), "..", "benchmarks", "x509")
CIL_SOURCE_EXISTS = os.path.isfile(
    os.path.join(_BENCHMARKS_DIR, "x509-submodule-2_cleaned_26.c")
)

requires_framac = pytest.mark.skipif(
    not FRAMAC_AVAILABLE, reason="frama-c not installed"
)
requires_bindings = pytest.mark.skipif(
    not BINDINGS_AVAILABLE, reason="libclang_parser not compiled"
)
requires_cil_source = pytest.mark.skipif(
    not CIL_SOURCE_EXISTS, reason="x509 test fixture not found"
)
requires_full_pipeline = pytest.mark.skipif(
    not (FRAMAC_AVAILABLE and BINDINGS_AVAILABLE and CIL_SOURCE_EXISTS),
    reason="Full pipeline requires frama-c + bindings + test fixture",
)


@pytest.fixture
def sample_function_info() -> FunctionInfo:
    return FunctionInfo(
        name="add",
        source_file="math.c",
        source_code="int add(int a, int b) { return a + b; }",
        callees=[],
        callers=["main"],
    )


@pytest.fixture
def sample_function_info_callee() -> FunctionInfo:
    return FunctionInfo(
        name="helper",
        source_file="math.c",
        source_code="int helper(int x) { return x * 2; }",
        callees=[],
        callers=["add"],
    )


@pytest.fixture
def registry(sample_function_info: FunctionInfo) -> AgentRegistry:
    reg = AgentRegistry()
    entry = AgentEntry(agent_id="agent_add", function_info=sample_function_info)
    reg.register(entry)
    return reg


@pytest.fixture
def bus() -> AsyncQueueBus:
    return AsyncQueueBus()
