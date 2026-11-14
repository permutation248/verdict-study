"""CLI package for Verdict."""

__all__ = ["main"]


def main() -> None:
    """Lazy entry point to avoid import cycles during library use."""
    from verdict.cli.app import main as _main

    _main()
