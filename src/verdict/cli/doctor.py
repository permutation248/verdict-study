"""Environment health check for Verdict.

Runs a focused set of checks covering the tools and secrets that a fresh
clone needs before `verdict solve` can work end-to-end. Each check prints
a single status line; the first FAIL makes the command exit with code 1
so CI integrations and wrapper scripts can rely on it.
"""

from __future__ import annotations

import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Callable


PASS = "PASS"
WARN = "WARN"
FAIL = "FAIL"


@dataclass
class DoctorCheck:
    """One health check result."""

    name: str
    status: str  # PASS | WARN | FAIL
    detail: str
    fix_hint: str | None = None


def _status_color(status: str) -> str:
    # Use bracket-padded labels; the CLI already calls _init_colors in app.py.
    if status == PASS:
        return "\033[32m[PASS]\033[0m"
    if status == WARN:
        return "\033[33m[WARN]\033[0m"
    if status == FAIL:
        return "\033[31m[FAIL]\033[0m"
    return f"[{status}]"


def _render(check: DoctorCheck) -> str:
    head = f"{_status_color(check.status)} {check.name}: {check.detail}"
    if check.fix_hint:
        head += f"\n       → {check.fix_hint}"
    return head


def _run_subprocess(argv: list[str], timeout: float = 5.0) -> subprocess.CompletedProcess | None:
    """Run a command, swallow OSError/timeout, return None on failure."""
    try:
        return subprocess.run(
            argv,
            capture_output=True,
            text=True,
            timeout=timeout,
            check=False,
        )
    except (OSError, subprocess.TimeoutExpired):
        return None


# ---------------------------------------------------------------------------
# Individual checks
# ---------------------------------------------------------------------------


def check_python_version() -> DoctorCheck:
    """Python >= 3.11 is required for `from __future__ import annotations` semantics,
    `X | None` typing, and the typing generics we use across core/types.py."""
    v = sys.version_info
    version_str = f"{v.major}.{v.minor}.{v.micro}"
    if v >= (3, 11):
        return DoctorCheck("python", PASS, version_str)
    return DoctorCheck(
        "python",
        FAIL,
        f"{version_str} (need >= 3.11)",
        fix_hint="Use pyenv or uv to install Python 3.11+",
    )


def check_uv() -> DoctorCheck:
    path = shutil.which("uv")
    if path is None:
        return DoctorCheck(
            "uv",
            FAIL,
            "not found in PATH",
            fix_hint="curl -LsSf https://astral.sh/uv/install.sh | sh",
        )
    result = _run_subprocess([path, "--version"])
    version = result.stdout.strip() if result and result.returncode == 0 else "unknown"
    return DoctorCheck("uv", PASS, f"{path} ({version})")


def check_frama_c() -> DoctorCheck:
    path = shutil.which("frama-c")
    if path is None:
        return DoctorCheck(
            "frama-c",
            FAIL,
            "not found in PATH",
            fix_hint="brew install frama-c  # or see https://frama-c.com/install.html",
        )
    result = _run_subprocess([path, "-version"])
    if result is None or result.returncode != 0:
        return DoctorCheck(
            "frama-c",
            FAIL,
            f"{path} (version probe failed)",
            fix_hint="Run `frama-c -version` manually to see the error",
        )
    version_line = (result.stdout or result.stderr or "").strip().splitlines()
    version = version_line[0] if version_line else "unknown"
    match = re.search(r"(\d+)\.(\d+)", version)
    if match and int(match.group(1)) < 27:
        return DoctorCheck(
            "frama-c",
            WARN,
            f"{path} ({version}) — Verdict is tested against Frama-C >= 27",
            fix_hint="Upgrade: brew upgrade frama-c",
        )
    return DoctorCheck("frama-c", PASS, f"{path} ({version})")


def check_wp_alt_ergo() -> DoctorCheck:
    if shutil.which("frama-c") is None:
        return DoctorCheck(
            "wp + alt-ergo",
            FAIL,
            "frama-c missing (see above)",
            fix_hint=None,
        )
    if shutil.which("alt-ergo") is None:
        return DoctorCheck(
            "wp + alt-ergo",
            FAIL,
            "alt-ergo not found in PATH",
            fix_hint="opam install alt-ergo  # or brew install alt-ergo",
        )
    # Ask WP to list provers; Alt-Ergo must be in the list.
    result = _run_subprocess(
        ["frama-c", "-wp-detect"],
        timeout=15.0,
    )
    if result is None or result.returncode != 0:
        return DoctorCheck(
            "wp + alt-ergo",
            FAIL,
            "frama-c -wp-detect failed",
            fix_hint="Run `frama-c -wp-detect` manually to see the error",
        )
    output = (result.stdout or "") + (result.stderr or "")
    if "alt-ergo" in output.lower():
        return DoctorCheck("wp + alt-ergo", PASS, "WP sees Alt-Ergo")
    return DoctorCheck(
        "wp + alt-ergo",
        WARN,
        "WP did not report Alt-Ergo among detected provers",
        fix_hint="Check `why3 config detect` output",
    )


def check_why3() -> DoctorCheck:
    path = shutil.which("why3")
    if path is None:
        return DoctorCheck(
            "why3",
            WARN,
            "not found (optional, only needed for multi-prover setups)",
            fix_hint="opam install why3",
        )
    result = _run_subprocess([path, "config", "list-provers"])
    if result is None or result.returncode != 0:
        return DoctorCheck(
            "why3",
            WARN,
            f"{path} (list-provers failed)",
            fix_hint="Run `why3 config detect` once",
        )
    provers = [line.strip() for line in result.stdout.splitlines() if line.strip()]
    if not provers:
        return DoctorCheck(
            "why3",
            WARN,
            "no provers registered",
            fix_hint="why3 config detect",
        )
    return DoctorCheck("why3", PASS, f"{len(provers)} prover(s) registered")


def check_cpp() -> DoctorCheck:
    """cpp must be available and must be able to locate standard headers
    like <stdint.h> — this is the single most common new-user pitfall."""
    path = shutil.which("cpp") or shutil.which("clang")
    if path is None:
        return DoctorCheck(
            "cpp",
            FAIL,
            "neither cpp nor clang found in PATH",
            fix_hint="brew install llvm  (then export CPATH accordingly)",
        )

    # Feed a tiny program that requires stdint.h through the preprocessor.
    test_src = "#include <stdint.h>\nint32_t probe;\n"
    try:
        proc = subprocess.run(
            [path, "-E", "-x", "c", "-"],
            input=test_src,
            capture_output=True,
            text=True,
            timeout=5.0,
            check=False,
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        return DoctorCheck(
            "cpp",
            FAIL,
            f"{path} invocation failed: {exc}",
            fix_hint=None,
        )

    if proc.returncode != 0 or "int32_t" not in proc.stdout:
        return DoctorCheck(
            "cpp",
            FAIL,
            f"{path} cannot resolve <stdint.h>",
            fix_hint=(
                "brew install llvm && export CPATH=$(brew --prefix llvm)/include"
                " (macOS) or apt install libc6-dev (Linux)"
            ),
        )
    return DoctorCheck("cpp", PASS, f"{path} resolves <stdint.h>")


def check_anthropic_api_key() -> DoctorCheck:
    # Primary path: verdict.core.env.get_sdk_env reads .env automatically
    # via python-dotenv and forwards the key to the Claude SDK subprocess.
    # That path does NOT mutate os.environ, so checking .env directly is
    # the honest thing to do here.
    try:
        from verdict.core.env import get_sdk_env
    except ImportError:
        get_sdk_env = None

    if get_sdk_env is not None:
        sdk_env = get_sdk_env()
        token = sdk_env.get("ANTHROPIC_API_KEY") or sdk_env.get("ANTHROPIC_AUTH_TOKEN")
        if token:
            masked = token[:4] + "…" + token[-4:] if len(token) >= 10 else "set"
            source = ".env (auto-loaded)" if Path(".env").exists() and token not in {
                os.environ.get("ANTHROPIC_API_KEY"),
                os.environ.get("ANTHROPIC_AUTH_TOKEN"),
            } else "env"
            return DoctorCheck(
                "anthropic api key",
                PASS,
                f"{masked} [{source}]",
            )

    # Fallback: plain os.environ check (for tests or non-standard setups).
    for key in ("ANTHROPIC_API_KEY", "ANTHROPIC_AUTH_TOKEN"):
        value = os.environ.get(key)
        if value:
            masked = value[:4] + "…" + value[-4:] if len(value) >= 10 else "set"
            return DoctorCheck(
                "anthropic api key",
                PASS,
                f"{key}={masked}",
            )

    return DoctorCheck(
        "anthropic api key",
        FAIL,
        "ANTHROPIC_API_KEY not set in env and .env has no entry",
        fix_hint="echo 'ANTHROPIC_API_KEY=sk-ant-...' >> .env",
    )


def check_claude_agent_sdk() -> DoctorCheck:
    try:
        module = importlib.import_module("claude_agent_sdk")
    except ImportError as exc:
        return DoctorCheck(
            "claude_agent_sdk",
            FAIL,
            f"import failed: {exc}",
            fix_hint="uv sync  (installs project dependencies)",
        )
    version = getattr(module, "__version__", "unknown")
    return DoctorCheck("claude_agent_sdk", PASS, f"version {version}")


def check_bindings_lib(project_root: Path) -> DoctorCheck:
    bindings_dir = project_root / "src" / "verdict" / "preprocessing" / "bindings"
    lib_ext = "dylib" if sys.platform == "darwin" else "so"
    lib_path = bindings_dir / "build" / f"libclang_parser.{lib_ext}"
    if lib_path.exists():
        return DoctorCheck("clang parser bindings", PASS, str(lib_path))
    build_config = bindings_dir / "build_config.sh"
    hint = None
    if build_config.exists():
        hint = f"cd {bindings_dir} && ./build.sh  (or consult build_config.sh)"
    return DoctorCheck(
        "clang parser bindings",
        FAIL,
        f"libclang_parser.{lib_ext} not built",
        fix_hint=hint,
    )


def check_framac_libc_path() -> DoctorCheck:
    value = os.environ.get("FRAMAC_LIBC_PATH")
    if value and Path(value).is_dir():
        return DoctorCheck("FRAMAC_LIBC_PATH", PASS, value)

    # Try to detect via frama-c -print-share-path
    if shutil.which("frama-c") is None:
        return DoctorCheck(
            "FRAMAC_LIBC_PATH",
            FAIL,
            "not set and frama-c missing",
            fix_hint="export FRAMAC_LIBC_PATH=$(frama-c -print-share-path)/libc",
        )
    result = _run_subprocess(["frama-c", "-print-share-path"])
    if result is None or result.returncode != 0:
        return DoctorCheck(
            "FRAMAC_LIBC_PATH",
            WARN,
            "not set and `frama-c -print-share-path` failed",
            fix_hint="export FRAMAC_LIBC_PATH=... (path to Frama-C's libc headers)",
        )
    share = result.stdout.strip()
    libc = Path(share) / "libc"
    if libc.is_dir():
        return DoctorCheck(
            "FRAMAC_LIBC_PATH",
            WARN,
            f"not set but auto-detectable at {libc}",
            fix_hint=f"export FRAMAC_LIBC_PATH={libc}",
        )
    return DoctorCheck(
        "FRAMAC_LIBC_PATH",
        FAIL,
        "not set and could not auto-detect",
        fix_hint="See https://frama-c.com/ for libc include path",
    )


def check_logs_writable(project_root: Path) -> DoctorCheck:
    logs = project_root / "logs"
    try:
        logs.mkdir(exist_ok=True)
    except OSError as exc:
        return DoctorCheck(
            "logs writable",
            FAIL,
            f"cannot create {logs}: {exc}",
            fix_hint=f"chmod u+w {project_root}",
        )
    probe = logs / ".doctor_probe"
    try:
        probe.write_text("ok", encoding="utf-8")
        probe.unlink()
    except OSError as exc:
        return DoctorCheck(
            "logs writable",
            FAIL,
            f"cannot write to {logs}: {exc}",
            fix_hint=f"chmod u+w {logs}",
        )
    return DoctorCheck("logs writable", PASS, str(logs))


# ---------------------------------------------------------------------------
# Orchestration
# ---------------------------------------------------------------------------


CheckFn = Callable[[], DoctorCheck]


def collect_checks(project_root: Path | None = None) -> list[DoctorCheck]:
    """Run every check in order and return their results."""
    root = project_root or Path(__file__).resolve().parents[3]

    checks: list[DoctorCheck] = [
        check_python_version(),
        check_uv(),
        check_frama_c(),
        check_wp_alt_ergo(),
        check_why3(),
        check_cpp(),
        check_anthropic_api_key(),
        check_claude_agent_sdk(),
        check_bindings_lib(root),
        check_framac_libc_path(),
        check_logs_writable(root),
    ]
    return checks


def cmd_doctor(args: argparse.Namespace | None = None) -> int:
    """Run the doctor checks and print a report.

    Returns the exit code: 0 if no FAILs, 1 otherwise. WARNs don't fail.
    """
    results = collect_checks()
    pass_count = sum(1 for r in results if r.status == PASS)
    warn_count = sum(1 for r in results if r.status == WARN)
    fail_count = sum(1 for r in results if r.status == FAIL)

    print("Verdict Environment Doctor")
    print("=" * 60)
    for check in results:
        print(_render(check))
    print()
    print(
        f"{pass_count} passed, {warn_count} warning(s), {fail_count} failed"
    )

    return 1 if fail_count > 0 else 0
