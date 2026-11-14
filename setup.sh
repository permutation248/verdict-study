#!/bin/bash
# =============================================================================
# Unified setup for Verdict
# =============================================================================
# One entry point per machine. It:
#   1. installs the C++ binding's system deps (LLVM/clang, jsoncpp, cmake)
#   2. sets up the Python env with uv (`uv sync`)
#   3. builds the clang parser binding via build.sh — this GENERATES the
#      per-machine build_config.sh (LLVM_ROOT, FRAMAC_BIN_DIR, ...)
#   4. verifies the whole chain with `verdict doctor`
#
# env_setup.sh is deliberately NOT generated here. It is generic, committed
# code; every machine-specific path lives in the generated build_config.sh.
# So once this script has run, you just `source env_setup.sh` on any machine.
#
# Frama-C + WP + provers are NOT installed here: Verdict needs a custom
# frama-c built with the global-deps plugin, which is too bespoke to automate.
# This script detects them and prints guidance when missing.

set -e

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
BLUE='\033[0;34m'; PURPLE='\033[0;35m'; NC='\033[0m'
log_info()    { echo -e "${BLUE}[INFO]${NC} $1"; }
log_success() { echo -e "${GREEN}[OK]${NC} $1"; }
log_warning() { echo -e "${YELLOW}[WARN]${NC} $1"; }
log_error()   { echo -e "${RED}[ERROR]${NC} $1"; }
log_step()    { echo -e "${PURPLE}[STEP]${NC} $1"; }

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BINDINGS_DIR="$SCRIPT_DIR/src/verdict/preprocessing/bindings"

command_exists() { command -v "$1" >/dev/null 2>&1; }

detect_platform() {
    case "$(uname -s)" in
        Darwin*) echo "macos" ;;
        Linux*)  echo "linux" ;;
        *)       echo "unknown" ;;
    esac
}

detect_package_manager() {
    if command_exists brew;     then echo "brew"
    elif command_exists apt-get; then echo "apt"
    elif command_exists yum;     then echo "yum"
    elif command_exists pacman;  then echo "pacman"
    else echo "unknown"; fi
}

# --- system deps for the C++ clang parser binding -----------------------------
install_system_deps() {
    log_step "Installing system dependencies for the clang parser binding..."
    local pkg; pkg=$(detect_package_manager)
    case "$pkg" in
        brew)
            brew install llvm jsoncpp cmake
            ;;
        apt)
            sudo apt-get update
            sudo apt-get install -y clang libclang-dev llvm-dev libjsoncpp-dev cmake build-essential
            ;;
        yum)
            sudo yum install -y clang clang-devel llvm-devel jsoncpp-devel cmake gcc-c++
            ;;
        pacman)
            sudo pacman -Syu --noconfirm clang llvm jsoncpp cmake base-devel
            ;;
        *)
            log_warning "No supported package manager found."
            echo "  Install manually: LLVM/clang dev libs, jsoncpp dev libs, cmake, a C++17 compiler."
            echo "  No sudo (e.g. a shared server)? Provide them via conda and re-run with --skip-system-deps:"
            echo "    conda install -c conda-forge clangdev llvmdev jsoncpp cmake"
            return 1
            ;;
    esac
    log_success "System dependencies installed"
}

# --- Python env via uv --------------------------------------------------------
setup_python() {
    log_step "Setting up the Python environment with uv..."
    if ! command_exists uv; then
        log_error "uv not found. Install it, then re-run (or use --skip-python):"
        echo "  curl -LsSf https://astral.sh/uv/install.sh | sh"
        echo "  (then ensure ~/.local/bin is on PATH)"
        return 1
    fi
    ( cd "$PROJECT_ROOT" && uv sync )
    log_success "Python environment ready (uv sync)"
}

# --- C++ binding + build_config.sh (delegated to build.sh) --------------------
build_binding() {
    local force="$1"
    log_step "Building the clang parser binding (generates build_config.sh)..."
    chmod +x "$BINDINGS_DIR/build.sh"
    if [[ "$force" == "true" ]]; then
        ( cd "$BINDINGS_DIR" && ./build.sh -f )
    else
        ( cd "$BINDINGS_DIR" && ./build.sh )
    fi
    log_success "Binding built; build_config.sh generated"
}

# --- Frama-C + provers: detect only, guide if missing -------------------------
check_framac() {
    log_step "Checking Frama-C and provers (not installed by this script)..."
    if command_exists frama-c; then
        log_success "frama-c: $(command -v frama-c)"
        local missing=()
        for p in z3 cvc5 alt-ergo; do
            command_exists "$p" || missing+=("$p")
        done
        if [[ ${#missing[@]} -gt 0 ]]; then
            log_warning "provers not on PATH: ${missing[*]}"
        else
            log_success "provers on PATH: z3, cvc5, alt-ergo"
        fi
    else
        log_warning "frama-c not on PATH."
        echo "  Verdict needs frama-c with the global-deps plugin."
        echo "  Install frama-c (e.g. via an opam switch), put it on PATH, then re-run"
        echo "  setup.sh — it will auto-install the bundled global-deps plugin."
    fi
}

# --- global-deps plugin: dune build + install into the frama-c switch ---------
install_framac_plugin() {
    local plugin_dir="$PROJECT_ROOT/frama-c-plugins/global-deps"
    if [[ ! -d "$plugin_dir" ]]; then
        log_warning "Plugin source not found at $plugin_dir — skipping"
        return 0
    fi
    if ! command_exists frama-c; then
        log_info "Skipping global-deps plugin install (frama-c not on PATH)"
        return 0
    fi
    if ! command_exists dune; then
        log_warning "dune not found — cannot build global-deps plugin"
        echo "  Install dune (opam install dune), then re-run setup.sh."
        return 0
    fi
    log_step "Installing the global-deps Frama-C plugin..."
    ( cd "$plugin_dir" && dune build && dune install ) || {
        log_warning "global-deps plugin install failed (non-fatal)"
        echo "  You can install it manually: cd $plugin_dir && dune build && dune install"
        return 0
    }
    log_success "global-deps plugin installed into $(dirname "$(command -v frama-c)")"
}

# --- verify the whole chain ---------------------------------------------------
verify() {
    log_step "Verifying the installation (verdict doctor)..."
    # shellcheck disable=SC1091
    source "$PROJECT_ROOT/env_setup.sh" >/dev/null 2>&1 || true
    set +e
    ( cd "$PROJECT_ROOT" && uv run verdict doctor )
    local rc=$?
    set -e
    return $rc
}

print_usage() {
    cat << EOF
Usage: $0 [options]

Options:
  -y, --yes             Non-interactive (skip the confirmation prompt)
  --skip-system-deps    Don't install C++ deps (already present, e.g. via conda)
  --skip-python         Don't run 'uv sync'
  --skip-build          Don't build the binding / regenerate build_config.sh
  --force-config        Pass -f to build.sh to regenerate build_config.sh
  --only-verify         Only run 'verdict doctor'
  -h, --help            Show this help

After setup, on ANY machine:
  source env_setup.sh && uv run verdict doctor
EOF
}

main() {
    local non_interactive=false skip_system=false skip_python=false
    local skip_build=false force_config=false only_verify=false

    while [[ $# -gt 0 ]]; do
        case "$1" in
            -y|--yes)            non_interactive=true ;;
            --skip-system-deps)  skip_system=true ;;
            --skip-python)       skip_python=true ;;
            --skip-build)        skip_build=true ;;
            --force-config)      force_config=true ;;
            --only-verify)       only_verify=true ;;
            -h|--help)           print_usage; exit 0 ;;
            *) log_error "Unknown option: $1"; print_usage; exit 1 ;;
        esac
        shift
    done

    echo
    echo "============================================"
    echo "Verdict setup — platform: $(detect_platform), pkg: $(detect_package_manager)"
    echo "============================================"
    echo

    if [[ "$only_verify" == "true" ]]; then
        verify; exit $?
    fi

    if [[ "$non_interactive" != "true" ]]; then
        echo "This will: install C++ deps, run 'uv sync', build the binding"
        echo "(generates build_config.sh), then run 'verdict doctor'."
        echo "Frama-C itself is not installed (you bring it), but the bundled"
        echo "global-deps plugin is auto-installed into the frama-c switch."
        read -r -p "Continue? (Y/n): " reply
        [[ "$reply" =~ ^[Nn]$ ]] && { log_info "Cancelled."; exit 0; }
    fi

    [[ "$skip_system" == "true" ]] && log_info "Skipping system deps" || install_system_deps
    [[ "$skip_python" == "true" ]] && log_info "Skipping python (uv sync)" || setup_python
    [[ "$skip_build"  == "true" ]] && log_info "Skipping binding build" || build_binding "$force_config"
    check_framac
    install_framac_plugin

    echo
    set +e; verify; local rc=$?; set -e

    echo
    echo "============================================"
    if [[ $rc -eq 0 ]]; then
        log_success "Setup complete."
    else
        log_warning "Setup finished, but 'verdict doctor' reported issues (see above)."
    fi
    echo
    echo "Next, on this machine:"
    echo -e "  ${GREEN}source env_setup.sh${NC}        # frama-c + provers on PATH, lib paths set"
    echo    "  # uv (python launcher) and any API proxy are separate concerns:"
    echo    "  #   export PATH=\"\$HOME/.local/bin:\$PATH\"   # if uv isn't already on PATH"
    echo    "  #   source <your-proxy>.sh                  # if API access needs a proxy"
    echo -e "  ${GREEN}uv run verdict solve <file.c>${NC}"
    echo "============================================"
}

main "$@"
