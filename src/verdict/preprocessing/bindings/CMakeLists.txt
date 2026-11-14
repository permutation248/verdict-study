cmake_minimum_required(VERSION 3.10)
project(advanced_parser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
# Platform and Configuration Detection
# =============================================================================

# Detect platform
if(APPLE)
    set(PLATFORM_NAME "macos")
    set(DEFAULT_LIB_EXTENSION "dylib")
elseif(UNIX)
    set(PLATFORM_NAME "linux") 
    set(DEFAULT_LIB_EXTENSION "so")
elseif(WIN32)
    set(PLATFORM_NAME "windows")
    set(DEFAULT_LIB_EXTENSION "dll")
else()
    set(PLATFORM_NAME "unknown")
    set(DEFAULT_LIB_EXTENSION "so")
endif()

# Set library file extension
if(NOT DEFINED LIB_EXTENSION)
    set(LIB_EXTENSION ${DEFAULT_LIB_EXTENSION})
endif()

message(STATUS "Build Platform: ${PLATFORM_NAME}")
message(STATUS "Library Extension: ${LIB_EXTENSION}")

# =============================================================================
# Dependency Path Configuration (Auto-detection and Manual Override)
# =============================================================================

# LLVM/Clang Configuration
if(NOT DEFINED LLVM_ROOT)
    # Try to find LLVM automatically
    find_package(LLVM QUIET)
    if(LLVM_FOUND)
        set(LLVM_ROOT ${LLVM_INSTALL_PREFIX})
        message(STATUS "Auto-discovered LLVM: ${LLVM_ROOT}")
    else()
        # Default paths
        if(APPLE)
            set(LLVM_ROOT "/opt/homebrew/opt/llvm")
        else()
            set(LLVM_ROOT "/usr")
        endif()
        message(WARNING "LLVM not auto-discovered, using default path: ${LLVM_ROOT}")
    endif()
endif()

# JsonCPP Configuration
if(NOT DEFINED JSONCPP_ROOT)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(JSONCPP QUIET jsoncpp)
        if(JSONCPP_FOUND)
            set(JSONCPP_ROOT ${JSONCPP_PREFIX})
            message(STATUS "Auto-discovered JsonCPP: ${JSONCPP_ROOT}")
        endif()
    endif()
    
    if(NOT DEFINED JSONCPP_ROOT)
        if(APPLE)
            set(JSONCPP_ROOT "/opt/homebrew/opt/jsoncpp")
        else()
            set(JSONCPP_ROOT "/usr")
        endif()
        message(WARNING "JsonCPP not auto-discovered, using default path: ${JSONCPP_ROOT}")
    endif()
endif()

# Project root directory
if(NOT DEFINED PROJECT_ROOT)
    set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# =============================================================================
# Path and Library File Definitions
# =============================================================================

# Define include directories
set(CLANG_INCLUDE_DIRS "${LLVM_ROOT}/include")

# 修改：Handle JsonCPP include directory for Ubuntu
if(PLATFORM_NAME STREQUAL "linux" AND JSONCPP_ROOT STREQUAL "/usr")
    # For Ubuntu, JsonCPP headers are typically in /usr/include/jsoncpp
    if(EXISTS "/usr/include/jsoncpp/json/json.h")
        set(JSONCPP_INCLUDE_DIRS "/usr/include/jsoncpp")
        message(STATUS "Using Ubuntu JsonCPP include path: /usr/include/jsoncpp")
    elseif(EXISTS "/usr/include/json/json.h")
        set(JSONCPP_INCLUDE_DIRS "/usr/include")
        message(STATUS "Using standard JsonCPP include path: /usr/include")
    else()
        set(JSONCPP_INCLUDE_DIRS "${JSONCPP_ROOT}/include")
        message(WARNING "JsonCPP headers not found in standard Ubuntu locations")
    endif()
else()
    # 保持原有逻辑不变
    set(JSONCPP_INCLUDE_DIRS "${JSONCPP_ROOT}/include")
endif()

# Define library file paths
set(CLANG_LIBRARIES "${LLVM_ROOT}/lib/libclang.${LIB_EXTENSION}")

# 新增：Handle JsonCPP library path for Ubuntu multiarch
if(PLATFORM_NAME STREQUAL "linux" AND JSONCPP_ROOT STREQUAL "/usr")
    # For Ubuntu multiarch, try to find the actual library
    find_library(JSONCPP_LIBRARIES 
        NAMES jsoncpp 
        PATHS 
            /usr/lib/x86_64-linux-gnu
            /usr/lib/i386-linux-gnu
            /usr/lib/aarch64-linux-gnu
            /usr/lib/arm-linux-gnueabihf
            /usr/lib
    )
    
    # If find_library didn't work, use the default path
    if(NOT JSONCPP_LIBRARIES)
        set(JSONCPP_LIBRARIES "${JSONCPP_ROOT}/lib/libjsoncpp.${LIB_EXTENSION}")
    endif()
else()
    # 保持原有逻辑不变
    set(JSONCPP_LIBRARIES "${JSONCPP_ROOT}/lib/libjsoncpp.${LIB_EXTENSION}")
endif()

# Find LLVM version
if(EXISTS "${LLVM_ROOT}/bin/llvm-config")
    execute_process(
        COMMAND ${LLVM_ROOT}/bin/llvm-config --version
        OUTPUT_VARIABLE LLVM_VERSION_STRING
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX MATCH "^[0-9]+" LLVM_VERSION_MAJOR ${LLVM_VERSION_STRING})
    set(LLVM_LIB_NAME "LLVM-${LLVM_VERSION_MAJOR}")
    message(STATUS "Detected LLVM version: ${LLVM_VERSION_STRING}, using library: ${LLVM_LIB_NAME}")
else()
    set(LLVM_LIB_NAME "LLVM")
    message(WARNING "Cannot detect LLVM version, using default library name: ${LLVM_LIB_NAME}")
endif()

# =============================================================================
# Compilation Settings
# =============================================================================

# Add compile definitions
add_definitions(
    -D__STDC_CONSTANT_MACROS
    -D__STDC_FORMAT_MACROS
    -D__STDC_LIMIT_MACROS
)

# Platform-specific settings
if(APPLE)
    # macOS specific settings
    if(DEFINED MACOS_SDK_PATH AND EXISTS ${MACOS_SDK_PATH})
        set(CMAKE_OSX_SYSROOT ${MACOS_SDK_PATH})
        message(STATUS "Using macOS SDK: ${MACOS_SDK_PATH}")
    endif()
endif()

# =============================================================================
# Include Directory Settings
# =============================================================================

include_directories(
    ${CLANG_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# =============================================================================
# Source File Definitions
# =============================================================================

set(SOURCES
    main.cpp
    clang_parser.cpp
)

# =============================================================================
# Executable Build
# =============================================================================

# Create executable
add_executable(advanced_parser ${SOURCES})

# Add link directories
target_link_directories(advanced_parser PRIVATE 
    ${LLVM_ROOT}/lib
    ${JSONCPP_ROOT}/lib
)

# 新增：For Ubuntu multiarch, add additional link directories
if(PLATFORM_NAME STREQUAL "linux" AND JSONCPP_ROOT STREQUAL "/usr")
    target_link_directories(advanced_parser PRIVATE 
        /usr/lib/x86_64-linux-gnu
        /usr/lib/i386-linux-gnu
        /usr/lib/aarch64-linux-gnu
        /usr/lib/arm-linux-gnueabihf
    )
endif()

# Link libraries
target_link_libraries(advanced_parser
    ${LLVM_LIB_NAME}
    clang
)

# Set linker flags
if(APPLE)
    set_target_properties(advanced_parser PROPERTIES
        LINK_FLAGS "-Wl,-search_paths_first -Wl,-headerpad_max_install_names"
    )
endif()

# =============================================================================
# Shared Library Build
# =============================================================================

# Add C API shared library
add_library(clang_parser_shared SHARED
    clang_parser.cpp
    clang_parser_c_api.cpp
)

# Set shared library output name
set_target_properties(clang_parser_shared PROPERTIES 
    OUTPUT_NAME clang_parser
)

# Add link directories for shared library
target_link_directories(clang_parser_shared PRIVATE 
    ${LLVM_ROOT}/lib
    ${JSONCPP_ROOT}/lib
)

# 新增：For Ubuntu multiarch, add additional link directories
if(PLATFORM_NAME STREQUAL "linux" AND JSONCPP_ROOT STREQUAL "/usr")
    target_link_directories(clang_parser_shared PRIVATE 
        /usr/lib/x86_64-linux-gnu
        /usr/lib/i386-linux-gnu
        /usr/lib/aarch64-linux-gnu
        /usr/lib/arm-linux-gnueabihf
    )
endif()

# Link shared library
target_link_libraries(clang_parser_shared 
    ${CLANG_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${LLVM_LIB_NAME}
)

# Set shared library include directories
target_include_directories(clang_parser_shared PRIVATE 
    ${CLANG_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# =============================================================================
# Installation Rules
# =============================================================================

# Install executable
install(TARGETS advanced_parser
    RUNTIME DESTINATION bin
)

# Install shared library
install(TARGETS clang_parser_shared
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install header files
install(FILES clang_parser.h
    DESTINATION include
)

# =============================================================================
# Debug Information Output
# =============================================================================

message(STATUS "=== Build Configuration Information ===")
message(STATUS "Platform: ${PLATFORM_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "LLVM Root: ${LLVM_ROOT}")
message(STATUS "LLVM Include: ${CLANG_INCLUDE_DIRS}")
message(STATUS "LLVM Libraries: ${CLANG_LIBRARIES}")
message(STATUS "LLVM Library Name: ${LLVM_LIB_NAME}")
message(STATUS "JsonCPP Root: ${JSONCPP_ROOT}")
message(STATUS "JsonCPP Include: ${JSONCPP_INCLUDE_DIRS}")
message(STATUS "JsonCPP Libraries: ${JSONCPP_LIBRARIES}")
message(STATUS "Project Root: ${PROJECT_ROOT}")
if(DEFINED FRAMAC_LIBC_PATH)
    message(STATUS "Frama-C Libc Path: ${FRAMAC_LIBC_PATH}")
endif()
if(APPLE AND DEFINED MACOS_SDK_PATH)
    message(STATUS "macOS SDK Path: ${MACOS_SDK_PATH}")
endif()
message(STATUS "==========================================")

# =============================================================================
# Dependency Validation
# =============================================================================

# Verify critical files exist
if(NOT EXISTS ${CLANG_INCLUDE_DIRS})
    message(FATAL_ERROR "Clang include directory does not exist: ${CLANG_INCLUDE_DIRS}")
endif()

if(NOT EXISTS ${JSONCPP_INCLUDE_DIRS})
    message(FATAL_ERROR "JsonCPP include directory does not exist: ${JSONCPP_INCLUDE_DIRS}")
endif()

if(NOT EXISTS ${CLANG_LIBRARIES})
    message(WARNING "Clang library file does not exist: ${CLANG_LIBRARIES}")
endif()

if(JSONCPP_LIBRARIES)
    if(NOT EXISTS ${JSONCPP_LIBRARIES})
        message(WARNING "JsonCPP library file does not exist: ${JSONCPP_LIBRARIES}")
    else()
        message(STATUS "JsonCPP library found at: ${JSONCPP_LIBRARIES}")
    endif()
else()
    message(WARNING "JsonCPP library not found")
endif()