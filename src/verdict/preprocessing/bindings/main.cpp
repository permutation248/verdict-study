#include "clang_parser.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

// Get environment variable or use default value
std::string getEnvVar(const char* name, const std::string& defaultValue = "") {
    const char* value = std::getenv(name);
    return value ? std::string(value) : defaultValue;
}

// Build Clang arguments
std::vector<const char*> buildClangArgs() {
    std::vector<const char*> clang_args;
    
    // Get configuration paths
    std::string framac_libc_path = getEnvVar("FRAMAC_LIBC_PATH");
    std::string project_root = getEnvVar("PROJECT_ROOT");
    std::string macos_sdk_path = getEnvVar("MACOS_SDK_PATH");
    
    // macOS SDK path
#ifdef __APPLE__
    if (!macos_sdk_path.empty()) {
        static std::string isysroot_arg = "-isysroot";
        static std::string sdk_path = macos_sdk_path;
        clang_args.push_back(isysroot_arg.c_str());
        clang_args.push_back(sdk_path.c_str());
    } else {
        // Default SDK path
        clang_args.push_back("-isysroot");
        clang_args.push_back("/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk");
    }
#endif
    
    // Frama-C libc path
    if (!framac_libc_path.empty()) {
        static std::string framac_include = "-I" + framac_libc_path;
        clang_args.push_back(framac_include.c_str());
    }
    
    // Project path
    if (!project_root.empty()) {
        static std::string project_include = "-I" + project_root;
        clang_args.push_back(project_include.c_str());
    } else {
        // Use relative path as fallback
        clang_args.push_back("-I.");
    }
    
    // Other standard arguments
    clang_args.push_back("-std=c11");
    
    return clang_args;
}

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <source_file_to_parse.c> [options]\n"
              << "\n"
              << "Options:\n"
              << "  -h, --help              Show this help message\n"
              << "  -v, --verbose           Verbose output\n"
              << "  --print-ast            Print AST\n"
              << "  --skip-call-graph      Skip call graph generation\n"
              << "\n"
              << "Environment Variables:\n"
              << "  FRAMAC_LIBC_PATH       Frama-C libc path\n"
              << "  PROJECT_ROOT           Project root directory path\n"
              << "  MACOS_SDK_PATH         macOS SDK path (macOS only)\n"
              << "\n"
              << "Examples:\n"
              << "  " << program_name << " test.c\n"
              << "  " << program_name << " test.c --verbose\n"
              << "  FRAMAC_LIBC_PATH=/opt/frama-c/libc " << program_name << " test.c\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc < 2) {
        std::cerr << "Error: Source file required\n" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    std::string source_file;
    bool verbose = false;
    bool print_ast = false;
    bool skip_call_graph = false;
    bool show_help = false;
    
    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            show_help = true;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "--print-ast") {
            print_ast = true;
        } else if (arg == "--skip-call-graph") {
            skip_call_graph = true;
        } else if (arg[0] != '-') {
            if (source_file.empty()) {
                source_file = arg;
            } else {
                std::cerr << "Error: Only one source file can be specified\n" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown option: " << arg << "\n" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (show_help) {
        printUsage(argv[0]);
        return 0;
    }
    
    if (source_file.empty()) {
        std::cerr << "Error: Source file required\n" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    // Build Clang arguments
    auto clang_args = buildClangArgs();
    
    if (verbose) {
        std::cout << "=== Parse Configuration ===" << std::endl;
        std::cout << "Source file: " << source_file << std::endl;
        std::cout << "Clang arguments:" << std::endl;
        for (const char* arg : clang_args) {
            std::cout << "  " << arg << std::endl;
        }
        std::cout << "===========================" << std::endl;
    }

    // Create parser
    ClangParser parser(source_file, clang_args);

    // Parse file
    std::cout << "Parsing: " << source_file << std::endl;
    
    if (parser.parse()) {
        std::cout << "✓ Parse successful" << std::endl;
        
        // Print AST (optional)
        if (print_ast) {
            std::cout << "\n=== AST Structure ===" << std::endl;
            parser.printAST();
        }
        
        // Extract global variables
        if (verbose) std::cout << "Extracting global variables..." << std::endl;
        parser.extractGlobalVariables();
        
        // Extract type definitions
        if (verbose) std::cout << "Extracting type definitions..." << std::endl;
        parser.extractTypeDefinitions();
        
        // Generate extended call graph (optional)
        if (!skip_call_graph) {
            if (verbose) std::cout << "Generating extended call graph..." << std::endl;
            parser.generateExtendedCallGraph();
        }

        // Display ordered global definitions
        std::cout << "\n=== Ordered Global Definitions ===" << std::endl;
        const auto& ordered_defs = parser.getOrderedGlobalDefinitions();
        if (ordered_defs.empty()) {
            std::cout << "No top-level global definitions found" << std::endl;
        } else {
            for (const auto& def : ordered_defs) {
                std::cout << "[" << def.order_index << "] "
                          << "Type: " << def.definition_type
                          << ", Name: '" << def.name << "'"
                          << ", Location: " << def.location.line << ":" << def.location.column
                          << std::endl;
                if (verbose) {
                    std::cout << "  Full Type: " << def.full_type << std::endl;
                    std::cout << "  Source: " << def.source_code << std::endl;
                }
            }
        }

        std::cout << "\n✓ Processing complete" << std::endl;
    } else {
        std::cerr << "\n✗ Parse failed or errors found" << std::endl;
        return 1;
    }

    return 0;
}