#ifndef CLANG_PARSER_H
#define CLANG_PARSER_H

#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map> // For the call graph

// Forward declaration
class ClangParser;

// --- Data structures for Extended Call Graph ---
struct LocationInfo {
    std::string file;
    unsigned int line = 0;
    unsigned int column = 0;
    unsigned int offset = 0;
};

struct ExtentInfo {
    unsigned int start_line = 0;
    unsigned int start_column = 0;
    unsigned int end_line = 0;
    unsigned int end_column = 0;
};

struct LoopInfoCpp {
    std::string loop_name;
    LocationInfo location;
    ExtentInfo extent;
    std::string source_code;
};

struct RteAssertInfoCpp {
    std::string content;
    ExtentInfo extent;
};

struct CallSiteInfoCpp {
    std::string callee_func_name;
    ExtentInfo extent;
    bool is_function_pointer_call = false;  // Flag to indicate if this is a function pointer call
    std::string function_pointer_name;      // Name of the function pointer variable (if applicable)
    std::string call_type;                  // "direct", "function_pointer", "indirect"
    // unsigned int order_in_caller; // If specific ordering index is needed beyond source location
};

// 新增：全局变量使用信息结构体
struct GlobalVarUsageInfoCpp {
    std::string var_name;
    std::string var_type;
    LocationInfo definition_location;  // 全局变量定义位置
    std::string definition_source;     // 全局变量定义源码
    std::vector<ExtentInfo> usage_locations; // 函数中使用该变量的位置
};

// New: Data structure for ordered global definitions
struct GlobalDefinitionInfo {
    std::string definition_type;    // "variable", "typedef", "struct", "union", "enum", "function_decl"
    std::string name;
    std::string full_type;          // Complete type information
    LocationInfo location;          // Source location
    ExtentInfo extent;              // Source extent
    std::string source_code;        // Complete source code
    unsigned int order_index;       // Order in the source file (0-based)
};

// New: Data structure for ordered function declarations
struct FunctionDeclarationInfo {
    std::string name;
    std::string signature;
    LocationInfo location;
    ExtentInfo extent;
    std::string source_code;
    unsigned int order_index;
};

struct ExtendedCallGraphNodeCpp {
    std::string func_name;
    LocationInfo location;
    ExtentInfo extent;
    std::string source_code;
    std::vector<std::string> children_names; // Names of functions called by this function
    std::vector<std::string> parent_names;   // Names of functions that call this function
    std::vector<CallSiteInfoCpp> call_sites; // Detailed info about calls made from this function
    std::vector<RteAssertInfoCpp> rte_asserts;
    std::vector<LoopInfoCpp> loops;
    std::vector<GlobalVarUsageInfoCpp> global_vars_used; // 新增：函数中使用的全局变量
    
    // 新增：调用点顺序记录
    std::map<std::pair<std::string, std::string>, int> call_sites_orders; // (caller_func_name, callee_func_name) -> call_order
    std::map<std::tuple<std::string, std::string, int>, int> call_site_order; // (caller_func_name, callee_func_name, call_site_start_line) -> call_order
    
    bool is_definition = false; // To mark if this node represents a function definition
    std::string current_function_name_for_second_pass; // To track the caller
};

// --- Visitor Payloads ---

// Generic callback data passed to clang_visitChildren's client_data
struct VisitorCallbackData {
    CXTranslationUnit tu;
    int indentLevel;
    void* userPayload; // Points to one of the specific payloads below, or directly to ClangParser*
};

// Specific payload for TypeDefinitionVisitor
struct TypeVisitorUserPayload {
    ClangParser* parserInstance;
    std::vector<std::string>* typeDefinitionsList;
};

// Specific payload for GlobalVariableVisitor
struct GlobalVarPayload {
    ClangParser* parserInstance;
    std::set<std::string>* varsSet;
};

// Payload for the first pass of Extended Call Graph generation
struct EcgFirstPassPayload {
    ClangParser* parser;
    std::map<std::string, ExtendedCallGraphNodeCpp>* graph;
};

// Payload for scanning within a function body (loops, RTE asserts)
struct FunctionBodyScanPayload {
    ClangParser* parser;
    ExtendedCallGraphNodeCpp* currentNode; // The function node being processed
    CXTranslationUnit tu; // Needed for tokenization
};

// Payload for the second pass of Extended Call Graph generation
struct EcgSecondPassPayload {
    ClangParser* parser;
    std::map<std::string, ExtendedCallGraphNodeCpp>* graph;
    std::string current_function_name_for_second_pass; // To track the caller
};

// 修正：全局变量定义信息结构体
struct GlobalVarDefinitionInfo {
    std::string var_name;
    std::string var_type;
    LocationInfo definition_location;  // 保持与使用处一致的字段名
    std::string definition_source;     // 保持与使用处一致的字段名
    // 移除 cursor 字段，因为 CXCursor 不适合长期存储
};

// New: Payload structure for ordered global definitions collection
struct GlobalDefinitionsOrderedPayload {
    ClangParser* parser;
    std::vector<GlobalDefinitionInfo>* ordered_definitions;
    unsigned int* current_order_index;
};

// New: Payload structure for ordered function declarations
struct FunctionDeclarationPayload {
    ClangParser* parser;
    std::vector<FunctionDeclarationInfo>* ordered_declarations;
    unsigned int* current_order_index;
    std::set<std::string>* processed_functions; // Set to track already added functions
};

class ClangParser {
public:
    // Constructor
    ClangParser(const std::string& source_file, const std::vector<const char*>& clang_args);

    // Destructor
    ~ClangParser();

    // Parses the source file. Returns true on success, false on failure.
    bool parse();

    // --- Original Functionality ---
    void printAST();
    void extractGlobalVariables();  // Filters to main file
    void extractTypeDefinitions(); // Filters to main file

    // --- Extended Call Graph Functionality ---
    // Generates the extended call graph. Call parse() before this.
    void generateExtendedCallGraph();
    // Retrieves the generated extended call graph.
    const std::map<std::string, ExtendedCallGraphNodeCpp>& getExtendedCallGraph() const;

    // Public helper to get source text, needed by static visitors or internal logic
    std::string getCursorSourceText(CXCursor cursor);
    // Public helper to get extent info
    static ExtentInfo getExtentInfo(CXCursor cursor);
    // Public helper to get location info
    static LocationInfo getLocationInfo(CXCursor cursor);

    // 新增：以列表形式获取扩展调用图
    std::vector<ExtendedCallGraphNodeCpp> getExtendedCallGraphList() const;

    // New: Get ordered global definitions
    const std::vector<GlobalDefinitionInfo>& getOrderedGlobalDefinitions() const;

    // New: Get ordered function declarations
    const std::vector<FunctionDeclarationInfo>& getOrderedFunctionDeclarations() const;

    // 新增：获取全局调用点顺序信息的访问器
    const std::map<std::pair<std::string, std::string>, int>& getGlobalCallSiteOrders() const {
        return global_call_site_orders;
    }

    const std::map<std::tuple<std::string, std::string, int, int>, int>& getGlobalCallSiteOrder() const {
        return global_call_site_order;
    }

    // 保持原有的公共API
    void recordCallSiteOrder(const std::string& caller, const std::string& callee, int line, int column);

private:
    CXIndex index;
    CXTranslationUnit translation_unit;
    std::string source_file_path;
    std::vector<const char*> clang_arguments;

    // Internal storage for the extended call graph
    std::map<std::string, ExtendedCallGraphNodeCpp> internal_extended_call_graph;
    
    // New: Storage for ordered global definitions
    std::vector<GlobalDefinitionInfo> ordered_global_definitions;
    
    // New: Storage for ordered function declarations
    std::vector<FunctionDeclarationInfo> ordered_function_declarations;
    
    // 新增：全局调用点顺序记录（类级别的记录）
    std::map<std::pair<std::string, std::string>, int> global_call_site_orders; // (caller_func_name, callee_func_name) -> call_order
    std::map<std::tuple<std::string, std::string, int, int>, int> global_call_site_order; // (caller_func_name, callee_func_name, call_site_start_line, call_site_start_column) -> call_order
    
    // Helper to ensure a node exists in the graph (e.g., for forward declarations encountered as callees)
    ExtendedCallGraphNodeCpp& ensureEcgNode(const std::string& func_name, std::map<std::string, ExtendedCallGraphNodeCpp>* graph);

    // 新增：存储全局变量定义信息
    std::map<std::string, GlobalVarDefinitionInfo> global_var_definitions;

    std::vector<std::string> source_file_lines;
    bool loadSourceFileContent();

    // --- Static Visitors for Original Functionality ---
    static CXChildVisitResult astPrinterVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    static CXChildVisitResult globalVariableVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    static CXChildVisitResult typeDefinitionVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    // --- Static Visitors and Helpers for Extended Call Graph ---
    static CXChildVisitResult ecgFirstPassVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    static CXChildVisitResult ecgLoopExtractorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    // RTE asserts are extracted by tokenizing the function body, not a separate visitor for children of the function.
    static void ecgRteAssertTokenScanner(CXCursor function_body_cursor, FunctionBodyScanPayload* payload);
    static CXChildVisitResult ecgSecondPassVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    // 新增：扫描全局变量使用的函数
    static CXChildVisitResult ecgGlobalVarUsageVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);
    void collectGlobalVariableDefinitions(); // 收集全局变量定义
    static CXChildVisitResult globalVarCollectorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    // New: Collect ordered global definitions
    void collectOrderedGlobalDefinitions();
    static CXChildVisitResult orderedGlobalDefinitionsVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    // New: Collect ordered function declarations
    void collectOrderedFunctionDeclarations();
    static CXChildVisitResult orderedFunctionDeclarationVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

    // --- Helper functions for function call handling ---
    void handleDirectFunctionCall(CXCursor callExpr, CXCursor referenced, 
                                EcgSecondPassPayload* payload, 
                                std::map<std::string, ExtendedCallGraphNodeCpp>* graph);
    
    void handleIndirectFunctionCall(CXCursor callExpr, 
                                  EcgSecondPassPayload* payload, 
                                  std::map<std::string, ExtendedCallGraphNodeCpp>* graph);
    
    std::string analyzeFunctionPointerCall(CXCursor callExpr);
    
    // 添加一个私有的重载版本用于内部使用
    void recordCallSiteOrderInternal(const std::string& caller, const std::string& callee, int line, int column);
};

#endif // CLANG_PARSER_H