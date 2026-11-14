#!/bin/bash

# =============================================================================
# Universal Build Script - Multi-platform Support with Flexible Configuration
# =============================================================================

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Default configuration
DEFAULT_CONFIG_FILE="build_config.sh"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Correctly define BINDINGS_DIR and PROJECT_ROOT
BINDINGS_DIR="$SCRIPT_DIR"
PROJECT_ROOT="$(cd "$BINDINGS_DIR/../../.." && pwd)" # Go up three levels to project root
BUILD_DIR="$BINDINGS_DIR/build"

# Platform detection
detect_platform() {
    local platform=$(uname -s)
    case $platform in
        Darwin*)
            echo "macos"
            ;;
        Linux*)
            echo "linux"
            ;;
        MINGW* | MSYS* | CYGWIN*)
            echo "windows"
            ;;
        *)
            echo "unknown"
            ;;
    esac
}

# Package manager detection
detect_package_manager() {
    if command -v brew >/dev/null 2>&1; then
        echo "brew"
    elif command -v apt-get >/dev/null 2>&1; then
        echo "apt"
    elif command -v yum >/dev/null 2>&1; then
        echo "yum"
    elif command -v pacman >/dev/null 2>&1; then
        echo "pacman"
    elif command -v zypper >/dev/null 2>&1; then
        echo "zypper"
    else
        echo "unknown"
    fi
}

# Find LLVM installation path
find_llvm_path() {
    local platform=$1
    local pkg_manager=$2
    
    # Common LLVM installation paths
    local common_paths=(
        "/opt/homebrew/opt/llvm"
        "/usr/local/opt/llvm"
        "/usr/lib/llvm-*"
        "/usr/local/llvm"
        "/usr/llvm"
        "/opt/llvm"
    )
    
    # Add package manager specific paths
    case $pkg_manager in
        brew)
            common_paths+=("/opt/homebrew/opt/llvm" "/usr/local/opt/llvm")
            ;;
        apt)
            common_paths+=("/usr/lib/llvm-18" "/usr/lib/llvm-17" "/usr/lib/llvm-16" "/usr/lib/llvm-15")
            ;;
        yum|pacman|zypper)
            common_paths+=("/usr" "/usr/local")
            ;;
    esac
    
    # Search for LLVM
    for path in "${common_paths[@]}"; do
        # Handle wildcard paths
        if [[ "$path" == *"*"* ]]; then
            for expanded_path in $path; do
                if [[ -d "$expanded_path" && -f "$expanded_path/bin/llvm-config" ]]; then
                    echo "$expanded_path"
                    return 0
                fi
            done
        else
            if [[ -d "$path" && -f "$path/bin/llvm-config" ]]; then
                echo "$path"
                return 0
            fi
        fi
    done
    
    # Try environment variable
    if [[ -n "$LLVM_ROOT" && -d "$LLVM_ROOT" ]]; then
        echo "$LLVM_ROOT"
        return 0
    fi
    
    # Try which command
    if command -v llvm-config >/dev/null 2>&1; then
        local llvm_config_path=$(which llvm-config)
        local llvm_root=$(dirname $(dirname $llvm_config_path))
        echo "$llvm_root"
        return 0
    fi
    
    return 1
}

# Find JsonCPP path
find_jsoncpp_path() {
    local platform=$1
    local pkg_manager=$2
    
    local common_paths=(
        "/opt/homebrew/opt/jsoncpp"
        "/usr/local/opt/jsoncpp"
        "/usr"
        "/usr/local"
    )
    
    # Add Ubuntu/Debian specific multiarch paths
    if [[ "$pkg_manager" == "apt" ]]; then
        # Get the multiarch triplet for current system
        local multiarch_triplet=""
        if command -v dpkg-architecture >/dev/null 2>&1; then
            multiarch_triplet=$(dpkg-architecture -qDEB_HOST_MULTIARCH 2>/dev/null)
        fi
        
        # Add multiarch paths
        if [[ -n "$multiarch_triplet" ]]; then
            common_paths+=("/usr/lib/$multiarch_triplet")
        fi
        
        # Add common Ubuntu multiarch paths as fallback
        common_paths+=(
            "/usr/lib/x86_64-linux-gnu"
            "/usr/lib/i386-linux-gnu"
            "/usr/lib/aarch64-linux-gnu"
            "/usr/lib/arm-linux-gnueabihf"
        )
    fi
    
    # Search for JsonCPP in common paths
    for path in "${common_paths[@]}"; do
        # Check for include directory
        if [[ -d "$path/include/json" || -d "$path/include/jsoncpp" ]]; then
            echo "$path"
            return 0
        fi
        
        # For Ubuntu multiarch, check if library exists even without include
        if [[ "$pkg_manager" == "apt" && -f "$path/libjsoncpp.so" ]]; then
            # For multiarch paths, return /usr as the root since includes are in /usr/include
            if [[ "$path" == "/usr/lib/"* ]]; then
                echo "/usr"
                return 0
            else
                echo "$path"
                return 0
            fi
        fi
    done
    
    # Try to find JsonCPP library using pkg-config
    if command -v pkg-config >/dev/null 2>&1; then
        if pkg-config --exists jsoncpp 2>/dev/null; then
            local jsoncpp_prefix=$(pkg-config --variable=prefix jsoncpp 2>/dev/null)
            if [[ -n "$jsoncpp_prefix" && -d "$jsoncpp_prefix" ]]; then
                echo "$jsoncpp_prefix"
                return 0
            fi
        fi
    fi
    
    # Try to find using ldconfig (Linux only)
    if [[ "$platform" == "linux" ]] && command -v ldconfig >/dev/null 2>&1; then
        local jsoncpp_lib=$(ldconfig -p 2>/dev/null | grep -m1 "libjsoncpp.so" | awk '{print $NF}')
        if [[ -n "$jsoncpp_lib" && -f "$jsoncpp_lib" ]]; then
            # Extract the directory and go up to find the root
            local lib_dir=$(dirname "$jsoncpp_lib")
            if [[ "$lib_dir" == "/usr/lib/"* ]]; then
                echo "/usr"
                return 0
            else
                echo "$(dirname "$lib_dir")"
                return 0
            fi
        fi
    fi
    
    return 1
}

# Find Frama-C path
find_framac_path() {
    # Try opam first
    if command -v opam >/dev/null 2>&1; then
        local opam_share=$(opam var share 2>/dev/null)
        if [[ -n "$opam_share" && -d "$opam_share/frama-c/share/libc" ]]; then
            echo "$opam_share/frama-c/share/libc"
            return 0
        fi
    fi
    
    # Try system paths
    local common_paths=(
        "/usr/share/frama-c/libc"
        "/usr/local/share/frama-c/libc"
        "/opt/frama-c/share/libc"
    )
    
    for path in "${common_paths[@]}"; do
        if [[ -d "$path" ]]; then
            echo "$path"
            return 0
        fi
    done
    
    return 1
}

# Get system SDK path (macOS)
get_system_sdk_path() {
    if [[ "$1" == "macos" ]]; then
        if command -v xcrun >/dev/null 2>&1; then
            xcrun --show-sdk-path 2>/dev/null || echo "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
        else
            echo "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
        fi
    fi
}

# Create configuration file in script directory
create_config_file() {
    local config_file="$1"
    local platform=$(detect_platform)
    local pkg_manager=$(detect_package_manager)
    
    log_info "Creating configuration file: $config_file"
    
    cat > "$config_file" << EOF
#!/bin/bash
# Build configuration file - Auto-generated
# Platform: $platform
# Package Manager: $pkg_manager

# =============================================================================
# Dependency Path Configuration
# =============================================================================

EOF

    # Find LLVM
    log_info "Searching for LLVM installation..."
    local llvm_path
    if llvm_path=$(find_llvm_path "$platform" "$pkg_manager"); then
        log_success "Found LLVM: $llvm_path"
        echo "LLVM_ROOT=\"$llvm_path\"" >> "$config_file"
    else
        log_warning "LLVM not found, using default path"
        echo "LLVM_ROOT=\"/usr/local\"" >> "$config_file"
    fi
    
    # Find JsonCPP
    log_info "Searching for JsonCPP installation..."
    local jsoncpp_path
    if jsoncpp_path=$(find_jsoncpp_path "$platform" "$pkg_manager"); then
        log_success "Found JsonCPP: $jsoncpp_path"
        echo "JSONCPP_ROOT=\"$jsoncpp_path\"" >> "$config_file"
    else
        log_warning "JsonCPP not found, using default path"
        echo "JSONCPP_ROOT=\"/usr/local\"" >> "$config_file"
    fi
    
    # Find Frama-C
    log_info "Searching for Frama-C library path..."
    local framac_path
    if framac_path=$(find_framac_path); then
        log_success "Found Frama-C: $framac_path"
        echo "FRAMAC_LIBC_PATH=\"$framac_path\"" >> "$config_file"
    else
        log_warning "Frama-C not found, please set FRAMAC_LIBC_PATH manually"
        echo "FRAMAC_LIBC_PATH=\"\"" >> "$config_file"
    fi

    # Find Frama-C bin directory. frama-c and its provers (z3/cvc5/alt-ergo)
    # share one bin (e.g. an opam switch not on the login PATH). Recorded so
    # env_setup.sh can prepend it to PATH generically, on any machine.
    log_info "Searching for Frama-C bin directory..."
    if command -v frama-c >/dev/null 2>&1; then
        local framac_bin_dir=$(cd "$(dirname "$(command -v frama-c)")" && pwd)
        log_success "Found Frama-C bin: $framac_bin_dir"
        echo "FRAMAC_BIN_DIR=\"$framac_bin_dir\"" >> "$config_file"
    else
        log_warning "frama-c not on PATH; leaving FRAMAC_BIN_DIR empty"
        echo "FRAMAC_BIN_DIR=\"\"" >> "$config_file"
    fi

    # Add other configuration
    cat >> "$config_file" << EOF

# =============================================================================
# System Configuration
# =============================================================================

# Platform
PLATFORM="$platform"
PACKAGE_MANAGER="$pkg_manager"

# Project paths
# Write the correct PROJECT_ROOT to the config file
PROJECT_ROOT="$PROJECT_ROOT"
BINDINGS_DIR="$BINDINGS_DIR"

EOF

    # macOS specific configuration
    if [[ "$platform" == "macos" ]]; then
        local sdk_path=$(get_system_sdk_path "$platform")
        cat >> "$config_file" << EOF
# macOS SDK path
MACOS_SDK_PATH="$sdk_path"

EOF
    fi
    
    # Add library extension configuration
    cat >> "$config_file" << EOF
# Library file extensions
case "\$PLATFORM" in
    macos)
        LIB_EXTENSION="dylib"
        ;;
    linux)
        LIB_EXTENSION="so"
        ;;
    windows)
        LIB_EXTENSION="dll"
        ;;
    *)
        LIB_EXTENSION="so"
        ;;
esac

# =============================================================================
# Advanced Configuration
# =============================================================================

# Build options
CMAKE_BUILD_TYPE="Release"
CMAKE_CXX_STANDARD="17"

# Parallel compilation
MAKE_JOBS=\$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Verbose output
VERBOSE=false

EOF
    
    log_success "Configuration file created: $config_file"
}

# Check dependencies
check_dependencies() {
    log_info "Checking build dependencies..."
    
    local missing_deps=()
    
    # Check cmake
    if ! command -v cmake >/dev/null 2>&1; then
        missing_deps+=("cmake")
    fi
    
    # Check make
    if ! command -v make >/dev/null 2>&1; then
        missing_deps+=("make")
    fi
    
    # Check compiler
    if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
        missing_deps+=("g++/clang++")
    fi
    
    if [[ ${#missing_deps[@]} -gt 0 ]]; then
        log_error "Missing dependencies: ${missing_deps[*]}"
        log_info "Please install missing dependencies and try again"
        return 1
    fi
    
    log_success "All basic dependencies are installed"
    return 0
}

# Validate configuration
validate_config() {
    local config_file="$1"
    
    if [[ ! -f "$config_file" ]]; then
        log_error "Configuration file not found: $config_file"
        return 1
    fi
    
    source "$config_file"
    
    log_info "Validating configuration..."
    
    local errors=0
    
    # Check LLVM
    if [[ ! -d "$LLVM_ROOT" ]]; then
        log_error "LLVM path does not exist: $LLVM_ROOT"
        errors=$((errors + 1))
    elif [[ ! -f "$LLVM_ROOT/lib/libclang.$LIB_EXTENSION" ]]; then
        log_warning "libclang library not found: $LLVM_ROOT/lib/libclang.$LIB_EXTENSION"
    fi
    
    # Check JsonCPP
    if [[ ! -d "$JSONCPP_ROOT" ]]; then
        log_error "JsonCPP path does not exist: $JSONCPP_ROOT"
        errors=$((errors + 1))
    fi
    
    # Check Frama-C (optional)
    if [[ -n "$FRAMAC_LIBC_PATH" && ! -d "$FRAMAC_LIBC_PATH" ]]; then
        log_warning "Frama-C path does not exist: $FRAMAC_LIBC_PATH"
    fi
    
    if [[ $errors -gt 0 ]]; then
        log_error "Configuration validation failed, please check and fix configuration file"
        return 1
    fi
    
    log_success "Configuration validation passed"
    return 0
}

# Generate CMake configuration
generate_cmake_config() {
    local config_file="$1"
    
    # Convert to absolute path if relative
    if [[ ! "$config_file" = /* ]]; then
        config_file="$PROJECT_DIR/$config_file"
    fi
    
    source "$config_file"
    
    log_info "Generating CMake configuration..."
    
    # Create CMake configuration arguments
    local cmake_args=(
        "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
        "-DCMAKE_CXX_STANDARD=$CMAKE_CXX_STANDARD"
        "-DLLVM_ROOT=$LLVM_ROOT"
        "-DJSONCPP_ROOT=$JSONCPP_ROOT"
        "-DPROJECT_ROOT=$PROJECT_ROOT"
        "-DLIB_EXTENSION=$LIB_EXTENSION"
    )
    
    # Add platform-specific configuration
    if [[ "$PLATFORM" == "macos" && -n "$MACOS_SDK_PATH" ]]; then
        cmake_args+=("-DMACOS_SDK_PATH=$MACOS_SDK_PATH")
    fi
    
    if [[ -n "$FRAMAC_LIBC_PATH" ]]; then
        cmake_args+=("-DFRAMAC_LIBC_PATH=$FRAMAC_LIBC_PATH")
    fi
    
    echo "${cmake_args[@]}"
}

# Build project
build_project() {
    local config_file="$1"
    
    log_info "Starting project build..."
    
    # Convert to absolute path if relative
    if [[ ! "$config_file" = /* ]]; then
        config_file="$BINDINGS_DIR/$config_file"
    fi
    
    # Load configuration
    source "$config_file"
    
    # Remove old build directory
    if [[ -d "$BUILD_DIR" ]]; then
        log_info "Removing old build directory..."
        rm -rf "$BUILD_DIR"
    fi
    
    # Create build directory
    log_info "Creating build directory..."
    mkdir -p "$BUILD_DIR"
    
    # Generate CMake configuration BEFORE changing directory
    local cmake_args=$(generate_cmake_config "$config_file")
    
    # Change to build directory
    cd "$BUILD_DIR"
    
    # Run CMake
    log_info "Running CMake configuration..."
    if [[ "$VERBOSE" == "true" ]]; then
        cmake $cmake_args ..
    else
        cmake $cmake_args .. > cmake_output.log 2>&1
    fi
    
    if [[ $? -ne 0 ]]; then
        log_error "CMake configuration failed!"
        if [[ "$VERBOSE" == "false" ]]; then
            log_info "Detailed output: $BUILD_DIR/cmake_output.log"
        fi
        return 1
    fi
    
    # Compile project
    log_info "Starting compilation..."
    if [[ "$VERBOSE" == "true" ]]; then
        make -j"$MAKE_JOBS"
    else
        make -j"$MAKE_JOBS" > make_output.log 2>&1
    fi
    
    if [[ $? -eq 0 ]]; then
        log_success "Build successful!"
        log_info "Executable location: $BUILD_DIR/advanced_parser"
        log_info "Shared library location: $BUILD_DIR/libclang_parser.$LIB_EXTENSION"
        return 0
    else
        log_error "Compilation failed!"
        if [[ "$VERBOSE" == "false" ]]; then
            log_info "Detailed output: $BUILD_DIR/make_output.log"
        fi
        return 1
    fi
}

# Show installation help
show_install_help() {
    local platform=$(detect_platform)
    local pkg_manager=$(detect_package_manager)
    
    log_info "Dependency Installation Guide:"
    echo
    
    case "$pkg_manager" in
        brew)
            echo "Using Homebrew to install dependencies:"
            echo "  brew install llvm jsoncpp cmake"
            echo "  # Install Frama-C (optional):"
            echo "  opam install frama-c"
            ;;
        apt)
            echo "Using APT to install dependencies:"
            echo "  sudo apt update"
            echo "  sudo apt install llvm-dev libclang-dev libjsoncpp-dev cmake build-essential"
            echo "  # Install Frama-C (optional):"
            echo "  opam install frama-c"
            ;;
        yum)
            echo "Using YUM to install dependencies:"
            echo "  sudo yum install llvm-devel clang-devel jsoncpp-devel cmake gcc-c++"
            ;;
        pacman)
            echo "Using Pacman to install dependencies:"
            echo "  sudo pacman -S llvm clang jsoncpp cmake base-devel"
            ;;
        *)
            echo "Please manually install the following dependencies:"
            echo "  - LLVM/Clang development libraries"
            echo "  - JsonCPP library"
            echo "  - CMake"
            echo "  - C++ compiler"
            ;;
    esac
    
    echo
    log_info "After installation, please re-run the build script"
}

# Main function
main() {
    local config_file="$DEFAULT_CONFIG_FILE"
    local force_config=false
    local show_help=false
    local install_help=false
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -c|--config)
                config_file="$2"
                shift 2
                ;;
            -f|--force)
                force_config=true
                shift
                ;;
            -h|--help)
                show_help=true
                shift
                ;;
            --install-help)
                install_help=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            *)
                log_error "Unknown argument: $1"
                show_help=true
                shift
                ;;
        esac
    done
    
    # Show help
    if [[ "$show_help" == "true" ]]; then
        cat << EOF
Usage: $0 [options]

Options:
  -c, --config FILE    Specify configuration file (default: $DEFAULT_CONFIG_FILE)
  -f, --force         Force regenerate configuration file
  -v, --verbose       Verbose output
  -h, --help          Show this help message
  --install-help      Show dependency installation guide

Examples:
  $0                          # Build with default configuration
  $0 -f                       # Force regenerate config and build
  $0 -c my_config.sh          # Use custom configuration file
  $0 --install-help           # Show dependency installation guide

EOF
        return 0
    fi
    
    # Show installation help
    if [[ "$install_help" == "true" ]]; then
        show_install_help
        return 0
    fi
    
    log_info "Universal Build Script - Platform: $(detect_platform)"
    
    # Check basic dependencies
    if ! check_dependencies; then
        show_install_help
        return 1
    fi
    
    # Create or update configuration file
    if [[ ! -f "$config_file" || "$force_config" == "true" ]]; then
        create_config_file "$config_file"
    else
        log_info "Using existing configuration file: $config_file"
    fi
    
    # Validate configuration
    if ! validate_config "$config_file"; then
        log_error "Please fix configuration file and try again, or use -f to regenerate"
        return 1
    fi
    
    # Build project
    if build_project "$config_file"; then
        log_success "Project build completed!"
        return 0
    else
        log_error "Project build failed!"
        return 1
    fi
}

# Run main function
main "$@"