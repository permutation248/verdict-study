"""Tests for verdict doctor CLI checks."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path
from unittest.mock import patch

import pytest

from verdict.cli.doctor import (
    DoctorCheck,
    FAIL,
    PASS,
    WARN,
    check_anthropic_api_key,
    check_bindings_lib,
    check_claude_agent_sdk,
    check_cpp,
    check_frama_c,
    check_framac_libc_path,
    check_logs_writable,
    check_python_version,
    check_uv,
    check_why3,
    check_wp_alt_ergo,
    cmd_doctor,
    collect_checks,
)


# ---------------------------------------------------------------------------
# Individual check unit tests — mostly stub out shutil.which and subprocess.
# ---------------------------------------------------------------------------


def test_python_version_check_reports_current():
    result = check_python_version()
    assert result.status == PASS
    assert f"{sys.version_info.major}.{sys.version_info.minor}" in result.detail


def test_uv_missing_returns_fail():
    with patch("verdict.cli.doctor.shutil.which", return_value=None):
        result = check_uv()
    assert result.status == FAIL
    assert "astral.sh" in (result.fix_hint or "")


def test_uv_present_returns_pass():
    def _which(cmd):
        return "/usr/local/bin/uv" if cmd == "uv" else None

    proc = subprocess.CompletedProcess(args=[], returncode=0, stdout="uv 0.5.0\n", stderr="")
    with patch("verdict.cli.doctor.shutil.which", side_effect=_which), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_uv()
    assert result.status == PASS
    assert "0.5.0" in result.detail


def test_frama_c_missing_returns_fail():
    with patch("verdict.cli.doctor.shutil.which", return_value=None):
        result = check_frama_c()
    assert result.status == FAIL
    assert "PATH" in result.detail


def test_frama_c_old_version_warns():
    proc = subprocess.CompletedProcess(
        args=[], returncode=0, stdout="Frama-C 25.0 (Manganese)\n", stderr=""
    )
    with patch("verdict.cli.doctor.shutil.which", return_value="/usr/bin/frama-c"), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_frama_c()
    assert result.status == WARN
    assert ">= 27" in result.detail or ">= 27" in (result.fix_hint or "")


def test_frama_c_recent_version_passes():
    proc = subprocess.CompletedProcess(
        args=[], returncode=0, stdout="Frama-C 28.1 (Nickel)\n", stderr=""
    )
    with patch("verdict.cli.doctor.shutil.which", return_value="/usr/bin/frama-c"), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_frama_c()
    assert result.status == PASS
    assert "28.1" in result.detail


def test_wp_alt_ergo_missing_frama_c():
    with patch("verdict.cli.doctor.shutil.which", return_value=None):
        result = check_wp_alt_ergo()
    assert result.status == FAIL


def test_wp_alt_ergo_detects_alt_ergo():
    def _which(cmd):
        return f"/usr/bin/{cmd}"

    proc = subprocess.CompletedProcess(
        args=[],
        returncode=0,
        stdout="Alt-Ergo 2.5.4 found\nZ3 not found\n",
        stderr="",
    )
    with patch("verdict.cli.doctor.shutil.which", side_effect=_which), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_wp_alt_ergo()
    assert result.status == PASS


def test_why3_missing_is_warn_not_fail():
    with patch("verdict.cli.doctor.shutil.which", return_value=None):
        result = check_why3()
    assert result.status == WARN


def test_cpp_resolves_stdint():
    proc = subprocess.CompletedProcess(
        args=[],
        returncode=0,
        stdout="int32_t probe;\n",
        stderr="",
    )
    with patch("verdict.cli.doctor.shutil.which", return_value="/usr/bin/cpp"), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_cpp()
    assert result.status == PASS


def test_cpp_missing_stdint_returns_fail():
    proc = subprocess.CompletedProcess(
        args=[],
        returncode=1,
        stdout="",
        stderr="fatal error: 'stdint.h' file not found\n",
    )
    with patch("verdict.cli.doctor.shutil.which", return_value="/usr/bin/cpp"), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_cpp()
    assert result.status == FAIL
    assert "stdint" in result.detail


def test_anthropic_api_key_from_env(monkeypatch):
    from verdict.core import env as core_env

    monkeypatch.setenv("ANTHROPIC_API_KEY", "sk-ant-abcdefghijkl1234")
    monkeypatch.setattr(
        core_env,
        "get_sdk_env",
        lambda: {"ANTHROPIC_API_KEY": "sk-ant-abcdefghijkl1234"},
    )
    result = check_anthropic_api_key()
    assert result.status == PASS
    assert "sk-a" in result.detail
    assert "1234" in result.detail


def test_anthropic_api_key_missing(monkeypatch, tmp_path):
    from verdict.core import env as core_env

    monkeypatch.delenv("ANTHROPIC_API_KEY", raising=False)
    monkeypatch.delenv("ANTHROPIC_AUTH_TOKEN", raising=False)
    monkeypatch.setattr(core_env, "get_sdk_env", lambda: {})
    monkeypatch.chdir(tmp_path)
    result = check_anthropic_api_key()
    assert result.status == FAIL


def test_anthropic_api_key_in_dotenv_is_pass(monkeypatch, tmp_path):
    """`.env` is loaded automatically by core.env.get_sdk_env — key found
    there must report PASS, not WARN."""
    monkeypatch.delenv("ANTHROPIC_API_KEY", raising=False)
    monkeypatch.delenv("ANTHROPIC_AUTH_TOKEN", raising=False)
    # Stub get_sdk_env so the check sees the same result without relying
    # on the project-root .env file location.
    from verdict.core import env as core_env

    monkeypatch.setattr(
        core_env,
        "get_sdk_env",
        lambda: {"ANTHROPIC_API_KEY": "sk-ant-0123456789abcd"},
    )
    result = check_anthropic_api_key()
    assert result.status == PASS
    assert "sk-a" in result.detail


def test_claude_agent_sdk_import_works():
    """claude_agent_sdk is a required project dep; must import cleanly."""
    result = check_claude_agent_sdk()
    assert result.status == PASS


def test_claude_agent_sdk_import_failure(monkeypatch):
    def _raise(name):
        raise ImportError("boom")

    with patch("verdict.cli.doctor.importlib.import_module", side_effect=_raise):
        result = check_claude_agent_sdk()
    assert result.status == FAIL
    assert "boom" in result.detail


def test_bindings_lib_missing(tmp_path):
    result = check_bindings_lib(tmp_path)
    assert result.status == FAIL
    assert "libclang_parser" in result.detail


def test_bindings_lib_present(tmp_path):
    lib_ext = "dylib" if sys.platform == "darwin" else "so"
    build_dir = tmp_path / "src" / "verdict" / "preprocessing" / "bindings" / "build"
    build_dir.mkdir(parents=True)
    (build_dir / f"libclang_parser.{lib_ext}").write_bytes(b"dummy")
    result = check_bindings_lib(tmp_path)
    assert result.status == PASS


def test_framac_libc_path_env_ok(monkeypatch, tmp_path):
    libc = tmp_path / "libc"
    libc.mkdir()
    monkeypatch.setenv("FRAMAC_LIBC_PATH", str(libc))
    result = check_framac_libc_path()
    assert result.status == PASS


def test_framac_libc_path_auto_detect(monkeypatch, tmp_path):
    monkeypatch.delenv("FRAMAC_LIBC_PATH", raising=False)
    libc = tmp_path / "share" / "libc"
    libc.mkdir(parents=True)

    proc = subprocess.CompletedProcess(
        args=[],
        returncode=0,
        stdout=str(tmp_path / "share") + "\n",
        stderr="",
    )
    with patch("verdict.cli.doctor.shutil.which", return_value="/usr/bin/frama-c"), \
         patch("verdict.cli.doctor.subprocess.run", return_value=proc):
        result = check_framac_libc_path()
    assert result.status == WARN
    assert "auto-detectable" in result.detail


def test_logs_writable_ok(tmp_path):
    result = check_logs_writable(tmp_path)
    assert result.status == PASS
    assert (tmp_path / "logs").is_dir()
    # probe file must be cleaned up
    assert not (tmp_path / "logs" / ".doctor_probe").exists()


# ---------------------------------------------------------------------------
# cmd_doctor orchestration
# ---------------------------------------------------------------------------


def test_cmd_doctor_exit_code_on_fail(monkeypatch, capsys):
    fake_results = [
        DoctorCheck("one", PASS, "ok"),
        DoctorCheck("two", FAIL, "broken", fix_hint="fix it"),
    ]
    with patch("verdict.cli.doctor.collect_checks", return_value=fake_results):
        code = cmd_doctor()
    assert code == 1
    output = capsys.readouterr().out
    assert "PASS" in output
    assert "FAIL" in output
    assert "fix it" in output


def test_cmd_doctor_exit_code_on_all_pass(capsys):
    fake_results = [
        DoctorCheck("one", PASS, "ok"),
        DoctorCheck("two", WARN, "minor"),
    ]
    with patch("verdict.cli.doctor.collect_checks", return_value=fake_results):
        code = cmd_doctor()
    assert code == 0
    output = capsys.readouterr().out
    assert "1 passed" in output
    assert "1 warning" in output
    assert "0 failed" in output


def test_collect_checks_runs_all_11():
    # Smoke test: at least 11 checks run without raising, each returns a DoctorCheck.
    results = collect_checks()
    assert len(results) >= 11
    for r in results:
        assert isinstance(r, DoctorCheck)
        assert r.status in {PASS, WARN, FAIL}
