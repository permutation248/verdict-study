#!/bin/bash
# Environment setup for Verdict
# Usage: source env_setup.sh
#
# This file is GENERIC across machines: it hard-codes no host paths. All
# machine-specific locations come from build_config.sh (written by build.sh:
# LLVM_ROOT, JSONCPP_ROOT, FRAMAC_LIBC_PATH, FRAMAC_BIN_DIR). This script just
# applies them to PATH / LD_LIBRARY_PATH. Re-run build.sh on a new machine and
# this script needs no edits.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo "Setting up environment for Verdict..."

# Machine-specific paths from the generated build configuration
BUILD_CONFIG="$SCRIPT_DIR/src/verdict/preprocessing/bindings/build_config.sh"
if [[ -f "$BUILD_CONFIG" ]]; then
    source "$BUILD_CONFIG"
    export FRAMAC_LIBC_PATH MACOS_SDK_PATH LLVM_ROOT JSONCPP_ROOT
else
    echo -e "${YELLOW}⚠ build_config.sh not found — run build.sh; falling back to auto-detect${NC}"
    if [[ "$(uname)" == "Darwin" ]]; then
        export MACOS_SDK_PATH="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
        if command -v brew >/dev/null 2>&1; then
            export LLVM_ROOT="$(brew --prefix llvm 2>/dev/null)"
            export JSONCPP_ROOT="$(brew --prefix jsoncpp 2>/dev/null)"
        fi
    fi
fi

# Project paths — authoritative, derived from THIS script's location. Set after
# sourcing build_config.sh so its (possibly stale) build-time copies of
# PROJECT_ROOT/BINDINGS_DIR never win. Verdict reads both at runtime.
export PROJECT_ROOT="$SCRIPT_DIR"
export BINDINGS_DIR="$SCRIPT_DIR/src/verdict/preprocessing/bindings"

# PATH: frama-c and its provers (z3/cvc5/alt-ergo) share one bin directory.
# FRAMAC_BIN_DIR is recorded by build.sh and may be an opam switch that is not
# on the login PATH. Prepend it once if present and not already there.
if [[ -n "$FRAMAC_BIN_DIR" && -d "$FRAMAC_BIN_DIR" ]]; then
    case ":$PATH:" in
        *":$FRAMAC_BIN_DIR:"*) ;;
        *) export PATH="$FRAMAC_BIN_DIR:$PATH" ;;
    esac
fi

# Runtime libs for the clang parser binding (Linux). libclang_parser.so links
# LLVM/clang and libstdc++ from LLVM_ROOT/lib — usually a conda env. macOS
# resolves these via rpath at build time, so this is unnecessary there.
if [[ "$(uname)" == "Linux" && -n "$LLVM_ROOT" && -d "$LLVM_ROOT/lib" ]]; then
    export LD_LIBRARY_PATH="$LLVM_ROOT/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
fi

# Frama-C ACSL-annotated libc: auto-detect if the config left it empty.
if [[ -z "$FRAMAC_LIBC_PATH" ]] && command -v frama-c >/dev/null 2>&1; then
    FRAMAC_SHARE="$(frama-c -print-share-path 2>/dev/null)"
    [[ -d "$FRAMAC_SHARE/libc" ]] && export FRAMAC_LIBC_PATH="$FRAMAC_SHARE/libc"
fi
export FRAMAC_LIBC_PATH

# Report
echo -e "${GREEN}✓ Environment configured${NC}"
echo "  PROJECT_ROOT=$PROJECT_ROOT"
[[ -n "$FRAMAC_BIN_DIR" ]]   && echo "  FRAMAC_BIN_DIR=$FRAMAC_BIN_DIR"
[[ -n "$FRAMAC_LIBC_PATH" ]] && echo "  FRAMAC_LIBC_PATH=$FRAMAC_LIBC_PATH"
[[ -n "$LLVM_ROOT" ]]        && echo "  LLVM_ROOT=$LLVM_ROOT"
[[ "$(uname)" == "Linux" && -n "$LD_LIBRARY_PATH" ]] && echo "  LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

# Compiled clang parser library
LIB_EXT="so"
[[ "$(uname)" == "Darwin" ]] && LIB_EXT="dylib"
LIB_PATH="$BINDINGS_DIR/build/libclang_parser.$LIB_EXT"
if [[ -f "$LIB_PATH" ]]; then
    echo -e "${GREEN}✓ clang parser library: $LIB_PATH${NC}"
else
    echo -e "${RED}✗ clang parser library missing: $LIB_PATH${NC} (run build.sh)"
fi

# Frama-C reachable?
if command -v frama-c >/dev/null 2>&1; then
    echo -e "${GREEN}✓ frama-c: $(command -v frama-c)${NC}"
else
    echo -e "${RED}✗ frama-c not on PATH${NC} (set FRAMAC_BIN_DIR in build_config.sh, or add it to PATH)"
fi

echo -e "${GREEN}✓ Environment setup completed!${NC}"
