#!/bin/bash
# Environment setup script for RTE-Guided-Specification-Generation
# Usage: source preprocessing/bindings/setup_env.sh

# Get the directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_CONFIG="$SCRIPT_DIR/build_config.sh"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Setting up environment for RTE-Guided-Specification-Generation..."

# Check if build_config.sh exists
if [[ ! -f "$BUILD_CONFIG" ]]; then
    echo -e "${RED}Error: build_config.sh not found${NC}"
    echo -e "${YELLOW}Please run './build.sh' first${NC}"
    return 1
fi

# Source the build configuration
source "$BUILD_CONFIG"

# Fix PROJECT_ROOT to point to actual project root
ACTUAL_PROJECT_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd)"
export PROJECT_ROOT="$ACTUAL_PROJECT_ROOT"
export BINDINGS_DIR="$SCRIPT_DIR"

# Export other variables
export FRAMAC_LIBC_PATH
export MACOS_SDK_PATH
export LLVM_ROOT
export JSONCPP_ROOT

# Verify setup
echo -e "${GREEN}✓ Environment variables exported${NC}"
echo "  FRAMAC_LIBC_PATH=$FRAMAC_LIBC_PATH"
echo "  MACOS_SDK_PATH=$MACOS_SDK_PATH"
echo "  PROJECT_ROOT=$PROJECT_ROOT (corrected to project root)"
echo "  BINDINGS_DIR=$BINDINGS_DIR"

# Check if compiled library exists
LIB_PATH="$SCRIPT_DIR/build/libclang_parser.$LIB_EXTENSION"
if [[ -f "$LIB_PATH" ]]; then
    echo -e "${GREEN}✓ Compiled library found: $LIB_PATH${NC}"
else
    echo -e "${RED}✗ Compiled library not found, please run './build.sh'${NC}"
fi

echo -e "${GREEN}✓ Environment setup completed!${NC}"
echo ""
echo "Now you can run: python cli.py -f [your_file.c]" 