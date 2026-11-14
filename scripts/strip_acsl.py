"""Strip all ACSL annotations from a C file.

Removes:
- Block annotations: /*@ ... */ (single and multi-line)
- Single-line annotations: //@ ...
- Ghost code: /*@ ghost ... */
- Standalone annotation lines that become empty after stripping

Usage:
    python scripts/strip_acsl.py input.c output.c
"""

import re
import sys


def strip_acsl(source: str) -> str:
    """Remove all ACSL annotations from C source code."""
    # Remove multi-line /*@ ... */ blocks
    result = re.sub(r'/\*@.*?\*/', '', source, flags=re.DOTALL)
    # Remove single-line //@ ...
    result = re.sub(r'///@.*$', '', result, flags=re.MULTILINE)
    result = re.sub(r'//@.*$', '', result, flags=re.MULTILINE)
    # Clean up blank lines left behind (collapse 3+ newlines to 2)
    result = re.sub(r'\n{3,}', '\n\n', result)
    # Remove lines that are only whitespace + semicolon (leftover from /*@ assert ...; */ ;)
    result = re.sub(r'^\s*;\s*$', '', result, flags=re.MULTILINE)
    result = re.sub(r'\n{3,}', '\n\n', result)
    return result


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.c output.c")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'r') as f:
        source = f.read()

    stripped = strip_acsl(source)

    with open(output_path, 'w') as f:
        f.write(stripped)

    # Stats
    orig_lines = source.count('\n')
    new_lines = stripped.count('\n')
    acsl_blocks = len(re.findall(r'/\*@', source))
    print(f"Stripped {acsl_blocks} ACSL blocks: {orig_lines} → {new_lines} lines")


if __name__ == "__main__":
    main()
