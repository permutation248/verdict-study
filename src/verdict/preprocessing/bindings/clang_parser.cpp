#include "clang_parser.h"
#include <iostream>
#include <vector> // Already included via header, but good for explicitness
#include <set>    // Already included via header
#include <algorithm> // For std::find, std::sort
#include <fstream> // For std::ifstream
#include <sstream> // For std::ostringstream

namespace {
// Helper function to extract text for a given extent from a vector of lines.
// This is refactored from getCursorSourceText to be reusable.
std::string extractTextForExtent(const std::vector<std::string>& source_lines, const ExtentInfo& extInfo) {
    if (extInfo.start_line == 0 || extInfo.end_line == 0 || extInfo.start_column == 0 || extInfo.end_column == 0) {
        return "// Error: Invalid extent for source text extraction (zero value in extent)";
    }
    if (extInfo.start_line > extInfo.end_line || (extInfo.start_line == extInfo.end_line && extInfo.start_column > extInfo.end_column)) {
        return "// Error: Invalid extent for source text extraction (start > end)";
    }

    unsigned int start_line_0based = extInfo.start_line - 1;
    unsigned int end_line_0based = extInfo.end_line - 1;
    unsigned int start_col_0based = extInfo.start_column - 1;
    unsigned int end_col_0based_exclusive = extInfo.end_column - 1;

    std::ostringstream oss;

    if (start_line_0based >= source_lines.size() || end_line_0based >= source_lines.size()) {
        return "// Error: Line numbers out of bounds for loaded source file content";
    }

    if (start_line_0based == end_line_0based) {
        // Single line
        const std::string& line = source_lines[start_line_0based];
        if (start_col_0based < line.length() && start_col_0based <= end_col_0based_exclusive) {
            oss << line.substr(start_col_0based, end_col_0based_exclusive - start_col_0based);
        }
    } else {
        // Multi-line
        // First line
        if (start_line_0based < source_lines.size()){
            const std::string& first_line = source_lines[start_line_0based];
            if (start_col_0based < first_line.length()) {
                oss << first_line.substr(start_col_0based);
            }
            oss << '\n';
        }

        // Middle lines
        for (unsigned int i = start_line_0based + 1; i < end_line_0based; ++i) {
            if (i < source_lines.size()){
                oss << source_lines[i] << '\n';
            } else {
                oss << "// Error: Middle line out of bounds\n";
                break; 
            }
        }

        // Last line
        if (end_line_0based < source_lines.size()) {
            const std::string& last_line = source_lines[end_line_0based];
            if (end_col_0based_exclusive <= last_line.length() && end_col_0based_exclusive > 0) {
                oss << last_line.substr(0, end_col_0based_exclusive);
            } else if (end_col_0based_exclusive > last_line.length()){
                oss << last_line;
            }
        }
    }
    return oss.str();
}
} // anonymous namespace

// Constructor
ClangParser::ClangParser(const std::string& source_file, const std::vector<const char*>& args)
    : index(nullptr), translation_unit(nullptr), source_file_path(source_file), clang_arguments(args) {
    // Create an index: the first argument is for excluding declarations from PCH,
    // the second is for displaying diagnostics.
    index = clang_createIndex(0, 1);
}

// Destructor
ClangParser::~ClangParser() {
    if (translation_unit) {
        clang_disposeTranslationUnit(translation_unit);
    }
    if (index) {
        clang_disposeIndex(index);
    }
    internal_extended_call_graph.clear();
}

// Public helper function to get source code text for a cursor
std::string ClangParser::getCursorSourceText(CXCursor cursor) {
    if (source_file_lines.empty()) {
        // Fallback or error if source_file_lines was not loaded (e.g., parse() failed before loading, or file was empty and loadSourceFileContent didn't populate)
        // std::cerr << "Warning: Source file lines not loaded. Falling back to token-based source extraction for cursor." << std::endl;
        
        // Fallback to token-based (less accurate formatting, but better than nothing)
       CXTranslationUnit current_tu_fb = this->translation_unit; // Try main TU first
       if (!current_tu_fb && !clang_Cursor_isNull(cursor)) { // If no main TU, try from cursor
           current_tu_fb = clang_Cursor_getTranslationUnit(cursor);
       }
       if (!current_tu_fb) return "// Error: TU unavailable for token fallback source text";

       std::string fallbackSourceText = "";
       CXSourceRange fb_extent = clang_getCursorExtent(cursor);
       if (!clang_Range_isNull(fb_extent)) {
           CXToken *fb_tokens = nullptr; unsigned fb_numTokens = 0;
           clang_tokenize(current_tu_fb, fb_extent, &fb_tokens, &fb_numTokens);
           if (fb_tokens) {
               for (unsigned i = 0; i < fb_numTokens; ++i) {
                   CXString ts = clang_getTokenSpelling(current_tu_fb, fb_tokens[i]);
                   fallbackSourceText += clang_getCString(ts);
                   if (i < fb_numTokens - 1) fallbackSourceText += " "; // Original token-based spacing
                   clang_disposeString(ts);
               }
               clang_disposeTokens(current_tu_fb, fb_tokens, fb_numTokens);
           }
       }
       return fallbackSourceText; 
    }

    ExtentInfo extInfo = ClangParser::getExtentInfo(cursor);
    return extractTextForExtent(this->source_file_lines, extInfo);
}


// Parses the source file
bool ClangParser::parse() {
    if (source_file_path.empty()) {
        std::cerr << "Error: Source file path is empty." << std::endl;
        return false;
    }
    if (!index) {
         std::cerr << "Error: CXIndex is null." << std::endl;
        return false;
    }

    if (!loadSourceFileContent()) {
        // Error message already printed by loadSourceFileContent or it will be if file is empty
        return false; 
    }

    translation_unit = clang_parseTranslationUnit(
        index,
        source_file_path.c_str(),
        clang_arguments.data(),
        static_cast<int>(clang_arguments.size()),
        nullptr, 
        0,       
        CXTranslationUnit_DetailedPreprocessingRecord // Essential for comments and accurate source ranges
        // CXTranslationUnit_None 
    );

    if (translation_unit == nullptr) {
        std::cerr << "Error: Unable to parse translation unit for " << source_file_path << std::endl;
        return false;
    }

    // Check for diagnostics (errors/warnings)
    unsigned int numDiagnostics = clang_getNumDiagnostics(translation_unit);
    if (numDiagnostics > 0) {
        std::cerr << "Diagnostics for " << source_file_path << ":" << std::endl;
        bool hasError = false;
        for (unsigned int i = 0; i < numDiagnostics; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(translation_unit, i);
            CXString diagSpelling = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
            std::cerr << clang_getCString(diagSpelling) << std::endl;
            clang_disposeString(diagSpelling);
            if (clang_getDiagnosticSeverity(diag) >= CXDiagnostic_Error) {
                hasError = true;
            }
            clang_disposeDiagnostic(diag);
        }
        if (hasError) {
            std::cerr << "Errors during parsing, aborting further processing for this file." << std::endl;
            // Consider disposing TU here if parse is considered a total failure
            // clang_disposeTranslationUnit(translation_unit);
            // translation_unit = nullptr;
            return false; // Indicate failure if there are errors
        }
    }
    return true;
}

// AST printing visitor
CXChildVisitResult ClangParser::astPrinterVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    VisitorCallbackData* cb_data = static_cast<VisitorCallbackData*>(client_data);
    ClangParser* parser = static_cast<ClangParser*>(cb_data->userPayload); // userPayload is ClangParser*

    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        return CXChildVisit_Continue; 
    }

    CXString kindSpelling = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
    CXString cursorSpelling = clang_getCursorSpelling(cursor);
    LocationInfo locInfo = getLocationInfo(cursor); // Use helper
    std::string sourceText = parser->getCursorSourceText(cursor);

    std::string indent_str(cb_data->indentLevel, ' ');
    
    clang_disposeString(kindSpelling);
    clang_disposeString(cursorSpelling);
    
    VisitorCallbackData child_cb_data;
    child_cb_data.tu = cb_data->tu;
    child_cb_data.indentLevel = cb_data->indentLevel + 2; 
    child_cb_data.userPayload = parser; // Pass the same ClangParser* instance

    clang_visitChildren(cursor, astPrinterVisitor, &child_cb_data);
    return CXChildVisit_Continue; 
}

void ClangParser::printAST() {
    if (!translation_unit) {
        std::cerr << "Cannot print AST: Translation unit is not parsed or failed to parse." << std::endl;
        return;
    }
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    VisitorCallbackData root_cb_data;
    root_cb_data.tu = translation_unit;
    root_cb_data.indentLevel = 0;
    root_cb_data.userPayload = this; // userPayload is ClangParser* (this)
    clang_visitChildren(rootCursor, astPrinterVisitor, &root_cb_data);
}

// Global variable extractor visitor
CXChildVisitResult ClangParser::globalVariableVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    VisitorCallbackData* cb_data = static_cast<VisitorCallbackData*>(client_data);
    GlobalVarPayload* gv_payload = static_cast<GlobalVarPayload*>(cb_data->userPayload);
    ClangParser* parser = gv_payload->parserInstance;
    std::set<std::string>* global_vars_set = gv_payload->varsSet;
    
    CXSourceLocation location = clang_getCursorLocation(cursor);
    bool isInMainFile = clang_Location_isFromMainFile(location) != 0;
    
    CXCursorKind kind = clang_getCursorKind(cursor);

    if (isInMainFile && kind == CXCursor_VarDecl) {
        CXCursor semanticParent = clang_getCursorSemanticParent(cursor);
        if (clang_getCursorKind(semanticParent) == CXCursor_TranslationUnit ||
            (clang_getCursorKind(semanticParent) == CXCursor_Namespace && 
             clang_getCursorKind(clang_getCursorSemanticParent(semanticParent)) == CXCursor_TranslationUnit)) {
            
            CXString cursorSpelling = clang_getCursorSpelling(cursor);
            CXType cursorType = clang_getCursorType(cursor);
            CXString typeSpelling = clang_getTypeSpelling(cursorType);
            std::string sourceText = parser->getCursorSourceText(cursor);
            if (sourceText.length() > 200) { 
                sourceText = sourceText.substr(0, 197) + "...";
            }
            std::string var_name = clang_getCString(cursorSpelling);
            if (!var_name.empty()) {
                 std::string entry = "Global Variable (Main File): " + var_name + 
                                     " (Type: " + clang_getCString(typeSpelling) + ")\n" +
                                     "  Source: [" + sourceText + "]\n";
                 global_vars_set->insert(entry);
            }
            clang_disposeString(cursorSpelling);
            clang_disposeString(typeSpelling);
        }
    }
    VisitorCallbackData child_cb_data = *cb_data; 
    child_cb_data.userPayload = gv_payload;       
    clang_visitChildren(cursor, globalVariableVisitor, &child_cb_data); 
    return CXChildVisit_Continue;
}


void ClangParser::extractGlobalVariables() {
    if (!translation_unit) {
        std::cerr << "Cannot extract global variables: Translation unit is not parsed." << std::endl;
        return;
    }
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    std::set<std::string> global_vars_set; 
    GlobalVarPayload gv_payload_instance;
    gv_payload_instance.parserInstance = this;
    gv_payload_instance.varsSet = &global_vars_set;
    VisitorCallbackData cb_data;
    cb_data.tu = translation_unit;
    cb_data.indentLevel = 0; 
    cb_data.userPayload = &gv_payload_instance; 
    clang_visitChildren(rootCursor, globalVariableVisitor, &cb_data);
}

// Struct/Union/Enum definition extractor visitor
CXChildVisitResult ClangParser::typeDefinitionVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    VisitorCallbackData* cb_data = static_cast<VisitorCallbackData*>(client_data);
    TypeVisitorUserPayload* payload = static_cast<TypeVisitorUserPayload*>(cb_data->userPayload);
    ClangParser* parser = payload->parserInstance;
    std::vector<std::string>* type_defs_list = payload->typeDefinitionsList;

    CXSourceLocation location = clang_getCursorLocation(cursor);
    bool isInMainFile = clang_Location_isFromMainFile(location) != 0;
    
    CXCursorKind kind = clang_getCursorKind(cursor);

    if (isInMainFile && (kind == CXCursor_StructDecl || kind == CXCursor_UnionDecl || kind == CXCursor_EnumDecl)) {
        if (clang_isCursorDefinition(cursor)) {
            CXString cursorSpellingCS = clang_getCursorSpelling(cursor);
            std::string name = clang_getCString(cursorSpellingCS);
            
            std::string kind_str;
            if (kind == CXCursor_StructDecl) kind_str = "Struct";
            else if (kind == CXCursor_UnionDecl) kind_str = "Union";
            else kind_str = "Enum";

            if (name.empty()) { // Handle anonymous types
                CXCursor semantic_parent_cursor = clang_getCursorSemanticParent(cursor);
                if (clang_getCursorKind(semantic_parent_cursor) == CXCursor_TypedefDecl) {
                    CXString typedef_name_cs = clang_getCursorSpelling(semantic_parent_cursor);
                    name = "(anonymous " + kind_str + " in typedef " + clang_getCString(typedef_name_cs) + ")";
                    clang_disposeString(typedef_name_cs);
                } else {
                    name = "(anonymous " + kind_str + ")";
                }
            }
            
            std::string source_snippet = parser->getCursorSourceText(cursor); 
            if (source_snippet.length() > 500) {
                source_snippet = source_snippet.substr(0, 497) + "...";
            }
            
            std::string definition_text = kind_str + " Definition (Main File): " + name + "\nSource:\n" + source_snippet + "\n";
            type_defs_list->push_back(definition_text);
            
            clang_disposeString(cursorSpellingCS);
            return CXChildVisit_Continue;
        }
    }

    // If the current cursor was not a S/U/E definition that we processed and returned from,
    // or for any other cursor, we need to visit its children to find S/U/E definitions deeper.
    VisitorCallbackData child_cb_data;
    child_cb_data.tu = cb_data->tu;
    child_cb_data.indentLevel = cb_data->indentLevel; // Not used for indentation here
    child_cb_data.userPayload = payload; // Pass the same TypeVisitorUserPayload

    clang_visitChildren(cursor, typeDefinitionVisitor, &child_cb_data);
    return CXChildVisit_Continue;
}


void ClangParser::extractTypeDefinitions() {
    if (!translation_unit) {
        std::cerr << "Cannot extract type definitions: Translation unit is not parsed." << std::endl;
        return;
    }
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    
    std::vector<std::string> type_defs_list;
    
    TypeVisitorUserPayload type_payload_instance; // Create the specific payload
    type_payload_instance.parserInstance = this;
    type_payload_instance.typeDefinitionsList = &type_defs_list;

    VisitorCallbackData cb_data; // Create the generic callback data
    cb_data.tu = translation_unit;
    cb_data.indentLevel = 0; 
    cb_data.userPayload = &type_payload_instance; // Assign address of specific payload to void*

    clang_visitChildren(rootCursor, typeDefinitionVisitor, &cb_data);
}

// --- Extended Call Graph Functionality ---

ExtendedCallGraphNodeCpp& ClangParser::ensureEcgNode(const std::string& func_name, std::map<std::string, ExtendedCallGraphNodeCpp>* graph) {
    if (graph->find(func_name) == graph->end()) {
        ExtendedCallGraphNodeCpp newNode;
        newNode.func_name = func_name;
        newNode.is_definition = false; // Initially, assume it's just a declaration or call
        (*graph)[func_name] = newNode;
    }
    return (*graph)[func_name];
}

void ClangParser::ecgRteAssertTokenScanner(CXCursor function_body_cursor, FunctionBodyScanPayload* payload) {
    // ClangParser* parser = payload->parser; // parser instance not directly needed here if TU is passed
    ExtendedCallGraphNodeCpp* currentNode = payload->currentNode;
    CXTranslationUnit tu = payload->tu; // Use TU from payload

    CXSourceRange extent = clang_getCursorExtent(function_body_cursor);
    if (clang_Range_isNull(extent)) return;

    CXToken *tokens = nullptr;
    unsigned numTokens = 0;
    clang_tokenize(tu, extent, &tokens, &numTokens);

    if (tokens) {
        for (unsigned i = 0; i < numTokens; ++i) {
            CXTokenKind tokenKind = clang_getTokenKind(tokens[i]);
            if (tokenKind == CXToken_Comment) {
                CXString tokenSpelling = clang_getTokenSpelling(tu, tokens[i]);
                std::string commentText = clang_getCString(tokenSpelling);
                // Search for both RTE and Eva assertion patterns
                if (commentText.find("rte") != std::string::npos || 
                    commentText.find("Eva") != std::string::npos) { 
                    RteAssertInfoCpp assertInfo;
                    assertInfo.content = commentText;
                    
                    CXSourceRange tokenRange = clang_getTokenExtent(tu, tokens[i]);
                    CXSourceLocation startLoc = clang_getRangeStart(tokenRange);
                    CXSourceLocation endLoc = clang_getRangeEnd(tokenRange);
                    CXFile file; // Dummy for spelling location
                    clang_getSpellingLocation(startLoc, &file, &assertInfo.extent.start_line, &assertInfo.extent.start_column, nullptr);
                    clang_getSpellingLocation(endLoc, &file, &assertInfo.extent.end_line, &assertInfo.extent.end_column, nullptr);
                    
                    currentNode->rte_asserts.push_back(assertInfo);
                }
                clang_disposeString(tokenSpelling);
            }
        }
        clang_disposeTokens(tu, tokens, numTokens);
    }
}

CXChildVisitResult ClangParser::ecgLoopExtractorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    FunctionBodyScanPayload* payload = static_cast<FunctionBodyScanPayload*>(client_data);
    ClangParser* parser = payload->parser;
    ExtendedCallGraphNodeCpp* funcNode = payload->currentNode;

    CXCursorKind kind = clang_getCursorKind(cursor);
    if (kind == CXCursor_ForStmt || kind == CXCursor_WhileStmt || kind == CXCursor_DoStmt) {
        LoopInfoCpp loopInfo;
        loopInfo.loop_name = funcNode->func_name + "_loop_" + std::to_string(funcNode->loops.size());
        loopInfo.location = ClangParser::getLocationInfo(cursor); 
        loopInfo.extent = ClangParser::getExtentInfo(cursor);     
        loopInfo.source_code = parser->getCursorSourceText(cursor); // parser needed here
        funcNode->loops.push_back(loopInfo);
    }
    // Recurse to find nested loops within the current compound statement (or part of it)
    clang_visitChildren(cursor, ecgLoopExtractorVisitor, client_data); // Pass same payload
    return CXChildVisit_Continue; // Continue to siblings of the current loop/statement
}

// Helper struct for finding the function body (CompoundStmt)
struct FindBodyPayload {
    CXCursor bodyCursor;
    bool bodyFound;
};

// Helper visitor to find the CompoundStmt child of a FunctionDecl
static CXChildVisitResult findFunctionBodyVisitor(CXCursor c, CXCursor parent, CXClientData client_data) {
    FindBodyPayload* payload = static_cast<FindBodyPayload*>(client_data);
    if (clang_getCursorKind(c) == CXCursor_CompoundStmt) {
        payload->bodyCursor = c;
        payload->bodyFound = true;
        return CXChildVisit_Break; 
    }
    return CXChildVisit_Continue; 
}

CXChildVisitResult ClangParser::ecgFirstPassVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    EcgFirstPassPayload* payload = static_cast<EcgFirstPassPayload*>(client_data);
    ClangParser* parser = payload->parser;
    std::map<std::string, ExtendedCallGraphNodeCpp>* graph = payload->graph;

    if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl) {
        // First check if function declaration is in main file
        CXSourceLocation location = clang_getCursorLocation(cursor);
        if (clang_Location_isFromMainFile(location) == 0) {
            // Not in main file, skip but continue traversing child nodes
            clang_visitChildren(cursor, ecgFirstPassVisitor, client_data);
            return CXChildVisit_Continue;
        }

        CXString cxFuncName = clang_getCursorSpelling(cursor);
        std::string func_name = clang_getCString(cxFuncName);
        clang_disposeString(cxFuncName);

        if (!func_name.empty()) {
            std::vector<std::string> excluded_functions = {"rand_int", "rand_unsigned", "rand_signed", "printf"};
            if (std::find(excluded_functions.begin(), excluded_functions.end(), func_name) != excluded_functions.end()) {
                // Excluded function, skip
            } else {
                // Create ECG node for both function definitions and declarations
                ExtendedCallGraphNodeCpp& node = parser->ensureEcgNode(func_name, graph);
                
                // Set basic information for both definitions and declarations
                node.location = ClangParser::getLocationInfo(cursor);
                node.extent = ClangParser::getExtentInfo(cursor);
                
                bool is_definition = clang_isCursorDefinition(cursor);
                if (is_definition) {
                    // This is a function definition - process the complete function
                    node.is_definition = true;
                    node.source_code = parser->getCursorSourceText(cursor);

                    FunctionBodyScanPayload bodyScanPayload;
                    bodyScanPayload.parser = parser;
                    bodyScanPayload.currentNode = &node; 
                    bodyScanPayload.tu = clang_Cursor_getTranslationUnit(cursor);

                    FindBodyPayload findBodyData;
                    findBodyData.bodyCursor = clang_getNullCursor();
                    findBodyData.bodyFound = false;
                    clang_visitChildren(cursor, findFunctionBodyVisitor, &findBodyData); 

                    if (findBodyData.bodyFound && !clang_Cursor_isNull(findBodyData.bodyCursor)) {
                         // Extract Loops by visiting children of the *body*
                         clang_visitChildren(findBodyData.bodyCursor, ecgLoopExtractorVisitor, &bodyScanPayload);
                         // Extract RTE Asserts by tokenizing the *body*
                         ecgRteAssertTokenScanner(findBodyData.bodyCursor, &bodyScanPayload);
                        // Extract Global Variable Usage
                        clang_visitChildren(findBodyData.bodyCursor, ecgGlobalVarUsageVisitor, &bodyScanPayload);
                    }
                } else {
                    // This is a function declaration only - store minimal information
                    node.is_definition = false;
                    std::string declaration_source = parser->getCursorSourceText(cursor);
                    
                    // Ensure function declaration ends with exactly one semicolon
                    // First trim whitespace
                    size_t end = declaration_source.find_last_not_of(" \n\r\t");
                    if (end != std::string::npos) {
                        declaration_source = declaration_source.substr(0, end + 1);
                    }
                    // Then check and add semicolon if needed
                    if (!declaration_source.empty() && declaration_source.back() != ';') {
                        declaration_source += ";";
                    }
                    node.source_code = declaration_source;
                    // No body analysis for declarations as they have no implementation
                }
            }
        }
    }
    
    clang_visitChildren(cursor, ecgFirstPassVisitor, client_data); 
    return CXChildVisit_Continue; 
}


CXChildVisitResult ClangParser::ecgSecondPassVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    EcgSecondPassPayload* payload = static_cast<EcgSecondPassPayload*>(client_data);
    ClangParser* parser = payload->parser;
    std::map<std::string, ExtendedCallGraphNodeCpp>* graph = payload->graph;

    // Only process nodes from the main file
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        clang_visitChildren(cursor, ecgSecondPassVisitor, client_data);
        return CXChildVisit_Continue;
    }

    CXCursorKind kind = clang_getCursorKind(cursor);

    if (kind == CXCursor_FunctionDecl) {
        CXString cxFuncName = clang_getCursorSpelling(cursor);
        std::string func_name_at_decl = clang_getCString(cxFuncName);
        clang_disposeString(cxFuncName);
        payload->current_function_name_for_second_pass = func_name_at_decl; 
    }

    if (!payload->current_function_name_for_second_pass.empty() && kind == CXCursor_CallExpr) {
        // Handle both direct function calls and function pointer calls
        
        // First, try to get the referenced function (for direct calls)
        CXCursor referenced = clang_getCursorReferenced(cursor); 
        
        if (!clang_Cursor_isNull(referenced) && clang_getCursorKind(referenced) == CXCursor_FunctionDecl) {
            // This is a direct function call - existing logic
            parser->handleDirectFunctionCall(cursor, referenced, payload, graph);
        }
        else {
            // This might be a function pointer call or indirect call
            parser->handleIndirectFunctionCall(cursor, payload, graph);
        }
    }
    
    EcgSecondPassPayload childPayload = *payload;
    clang_visitChildren(cursor, ecgSecondPassVisitor, &childPayload);
    return CXChildVisit_Continue;
}

// New helper function for handling direct function calls
void ClangParser::handleDirectFunctionCall(CXCursor callExpr, CXCursor referenced, 
                                         EcgSecondPassPayload* payload, 
                                         std::map<std::string, ExtendedCallGraphNodeCpp>* graph) {
    // Check if the referenced function is also in the main file
    CXSourceLocation referenced_location = clang_getCursorLocation(referenced);
    if (clang_Location_isFromMainFile(referenced_location) == 0) {
        return; // Skip external function calls
    }
    
    CXString cxCalleeName = clang_getCursorSpelling(referenced);
    std::string callee_name = clang_getCString(cxCalleeName);
    clang_disposeString(cxCalleeName);

    if (!callee_name.empty() && graph->find(callee_name) != graph->end()) {
        ExtendedCallGraphNodeCpp& callerNode = ensureEcgNode(payload->current_function_name_for_second_pass, graph);
        ExtendedCallGraphNodeCpp& calleeNode = (*graph)[callee_name];

        // Check if this relationship already exists to avoid duplication
        bool relationship_exists = false;
        ExtentInfo current_extent = getExtentInfo(callExpr);
        
        for (const auto& call_site : callerNode.call_sites) {
            if (call_site.callee_func_name == callee_name &&
                call_site.extent.start_line == current_extent.start_line && 
                call_site.extent.start_column == current_extent.start_column) {
                relationship_exists = true;
                break;
            }
        }

        if (!relationship_exists) {
            // Establish parent-child relationships
            if (std::find(callerNode.children_names.begin(), callerNode.children_names.end(), callee_name) == callerNode.children_names.end()) {
                callerNode.children_names.push_back(callee_name);
            }
            if (std::find(calleeNode.parent_names.begin(), calleeNode.parent_names.end(), payload->current_function_name_for_second_pass) == calleeNode.parent_names.end()) {
                calleeNode.parent_names.push_back(payload->current_function_name_for_second_pass);
            }

            // Record call site information
            CallSiteInfoCpp callSite;
            callSite.callee_func_name = callee_name;
            callSite.extent = current_extent;
            callSite.is_function_pointer_call = false;
            callSite.call_type = "direct";
            
            callerNode.call_sites.push_back(callSite);
            
            recordCallSiteOrder(payload->current_function_name_for_second_pass, callee_name, callSite.extent.start_line, callSite.extent.start_column);
        }
    }
}

// New helper function for handling indirect/function pointer calls
void ClangParser::handleIndirectFunctionCall(CXCursor callExpr, 
                                           EcgSecondPassPayload* payload, 
                                           std::map<std::string, ExtendedCallGraphNodeCpp>* graph) {
    
    ExtendedCallGraphNodeCpp& callerNode = ensureEcgNode(payload->current_function_name_for_second_pass, graph);
    
    // Try to analyze the function pointer call
    std::string function_pointer_info = analyzeFunctionPointerCall(callExpr);
    
    if (!function_pointer_info.empty()) {
        // Record call site information for function pointer call
        CallSiteInfoCpp callSite;
        callSite.callee_func_name = "<function_pointer>"; // Placeholder for unknown target
        callSite.extent = getExtentInfo(callExpr);
        callSite.is_function_pointer_call = true;
        callSite.function_pointer_name = function_pointer_info;
        callSite.call_type = "function_pointer";
        
        callerNode.call_sites.push_back(callSite);
    }
    else {
        // Generic indirect call
        CallSiteInfoCpp callSite;
        callSite.callee_func_name = "<indirect_call>";
        callSite.extent = getExtentInfo(callExpr);
        callSite.is_function_pointer_call = true;
        callSite.call_type = "indirect";
        
        callerNode.call_sites.push_back(callSite);
    }
}

// New helper function to analyze function pointer calls
std::string ClangParser::analyzeFunctionPointerCall(CXCursor callExpr) {
    // Get the first child of the CallExpr, which should be the function expression
    CXCursor functionExpr = clang_getNullCursor();
    
    // Visit children to find the function expression
    struct FindFunctionExprPayload {
        CXCursor* functionExpr;
        bool found;
    };
    
    FindFunctionExprPayload findPayload;
    findPayload.functionExpr = &functionExpr;
    findPayload.found = false;
    
    auto findFunctionExprVisitor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
        FindFunctionExprPayload* payload = static_cast<FindFunctionExprPayload*>(client_data);
        if (!payload->found) {
            *(payload->functionExpr) = cursor;
            payload->found = true;
            return CXChildVisit_Break;
        }
    return CXChildVisit_Continue;
    };
    
    clang_visitChildren(callExpr, findFunctionExprVisitor, &findPayload);
    
    if (clang_Cursor_isNull(functionExpr)) {
        return "";
    }
    
    CXCursorKind exprKind = clang_getCursorKind(functionExpr);
    
    // Handle different types of function pointer expressions
    if (exprKind == CXCursor_DeclRefExpr) {
        // Direct function pointer variable reference: func_ptr()
        CXCursor referenced = clang_getCursorReferenced(functionExpr);
        if (!clang_Cursor_isNull(referenced)) {
            CXString varName = clang_getCursorSpelling(referenced);
            std::string result = "function_pointer:" + std::string(clang_getCString(varName));
            clang_disposeString(varName);
            return result;
        }
    }
    else if (exprKind == CXCursor_MemberRefExpr) {
        // Function pointer member access: obj.func_ptr() or obj->func_ptr()
        CXString memberName = clang_getCursorSpelling(functionExpr);
        std::string result = "member_function_pointer:" + std::string(clang_getCString(memberName));
        clang_disposeString(memberName);
        return result;
    }
    else if (exprKind == CXCursor_ArraySubscriptExpr) {
        // Function pointer array access: func_array[index]()
        return "function_pointer_array";
    }
    else if (exprKind == CXCursor_UnaryOperator) {
        // Dereference operation: (*func_ptr)()
        return "dereferenced_function_pointer";
    }
    else if (exprKind == CXCursor_ParenExpr) {
        // Parenthesized expression - need to look deeper
        CXCursor inner = clang_getNullCursor();
        
        auto findInnerVisitor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> CXChildVisitResult {
            CXCursor* innerCursor = static_cast<CXCursor*>(client_data);
            *innerCursor = cursor;
            return CXChildVisit_Break;
        };
        
        clang_visitChildren(functionExpr, findInnerVisitor, &inner);
        
        if (!clang_Cursor_isNull(inner)) {
            return "parenthesized:" + analyzeFunctionPointerCall(inner);
        }
    }
    
    // Fallback: try to get source text of the expression
    std::string sourceText = getCursorSourceText(functionExpr);
    if (!sourceText.empty() && sourceText.length() < 100) { // Reasonable length check
        return "expression:" + sourceText;
    }
    
    return "unknown_indirect_call";
}

// Helper function to record call site orders
void ClangParser::recordCallSiteOrder(const std::string& caller, const std::string& callee, int line, int column) {
    std::pair<std::string, std::string> caller_callee_pair = std::make_pair(caller, callee);
    std::tuple<std::string, std::string, int, int> caller_callee_line_col_tuple = std::make_tuple(caller, callee, line, column);
    
    // Update global call site orders (at parser level)
    if (global_call_site_orders.find(caller_callee_pair) == global_call_site_orders.end()) {
        global_call_site_orders[caller_callee_pair] = 0;
        global_call_site_order[caller_callee_line_col_tuple] = 0;
    } else {
        global_call_site_orders[caller_callee_pair] += 1;
        global_call_site_order[caller_callee_line_col_tuple] = global_call_site_orders[caller_callee_pair];
    }
}

void ClangParser::generateExtendedCallGraph() {
    if (!translation_unit) {
        std::cerr << "Cannot generate extended call graph: Translation unit is not parsed." << std::endl;
        return;
    }
    
    // 清空所有内部状态
    internal_extended_call_graph.clear(); 
    global_var_definitions.clear();
    global_call_site_orders.clear();
    global_call_site_order.clear();
    ordered_global_definitions.clear(); // Clear ordered definitions
    ordered_function_declarations.clear(); // Clear function declarations

    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);

    // Preprocessing: collect global variable definitions
    collectGlobalVariableDefinitions();

    // New: Collect ordered global definitions
    collectOrderedGlobalDefinitions();

    // New: Collect ordered function declarations
    collectOrderedFunctionDeclarations();

    // First Pass: Identify function definitions and extract function-level information
    EcgFirstPassPayload firstPassPayload;
    firstPassPayload.parser = this;
    firstPassPayload.graph = &internal_extended_call_graph;
    clang_visitChildren(rootCursor, ecgFirstPassVisitor, &firstPassPayload);

    // Second Pass: Establish call relationships and record call site orders
    EcgSecondPassPayload secondPassPayload;
    secondPassPayload.parser = this;
    secondPassPayload.graph = &internal_extended_call_graph;
    secondPassPayload.current_function_name_for_second_pass = ""; 
    clang_visitChildren(rootCursor, ecgSecondPassVisitor, &secondPassPayload);
}

const std::map<std::string, ExtendedCallGraphNodeCpp>& ClangParser::getExtendedCallGraph() const {
    return internal_extended_call_graph;
}

// --- Helper function implementations ---
ExtentInfo ClangParser::getExtentInfo(CXCursor cursor) {
    ExtentInfo extent;
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXSourceLocation startLoc = clang_getRangeStart(range);
    CXSourceLocation endLoc = clang_getRangeEnd(range);

    CXFile file; // Dummy file, not strictly needed for line/col of extent
    clang_getSpellingLocation(startLoc, &file, &extent.start_line, &extent.start_column, nullptr);
    clang_getSpellingLocation(endLoc, &file, &extent.end_line, &extent.end_column, nullptr);
    return extent;
}

LocationInfo ClangParser::getLocationInfo(CXCursor cursor) {
    LocationInfo locInfo;
    CXSourceLocation location = clang_getCursorLocation(cursor);
    CXFile file;
    clang_getSpellingLocation(location, &file, &locInfo.line, &locInfo.column, &locInfo.offset);
    if (file) { // file can be NULL if location is invalid
      CXString fileName = clang_getFileName(file);
      locInfo.file = clang_getCString(fileName);
      clang_disposeString(fileName);
    } else {
        locInfo.file = "<?>"; // Indicate unknown file if CXFile is NULL
    }
    return locInfo;
}

bool ClangParser::loadSourceFileContent() {
    source_file_lines.clear();
    std::ifstream file_stream(source_file_path);
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open source file for reading: " << source_file_path << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file_stream, line)) {
        source_file_lines.push_back(line);
    }
    file_stream.close();
    // You can add a check here if source_file_lines is empty even after a successful open,
    // which would mean the source file itself is empty.
    return true;
}

// 收集全局变量定义
void ClangParser::collectGlobalVariableDefinitions() {
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    clang_visitChildren(rootCursor, globalVarCollectorVisitor, this);
}

// 收集全局变量定义的访问器
CXChildVisitResult ClangParser::globalVarCollectorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    ClangParser* parser = static_cast<ClangParser*>(client_data);
    
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        clang_visitChildren(cursor, globalVarCollectorVisitor, client_data);
        return CXChildVisit_Continue;
    }
    
    CXCursorKind kind = clang_getCursorKind(cursor);
    if (kind == CXCursor_VarDecl) {
        CXCursor semanticParent = clang_getCursorSemanticParent(cursor);
        // 检查是否为全局变量（语义父节点是翻译单元）
        if (clang_getCursorKind(semanticParent) == CXCursor_TranslationUnit) {
            CXString varName = clang_getCursorSpelling(cursor);
            CXType varType = clang_getCursorType(cursor);
            CXString typeName = clang_getTypeSpelling(varType);
            
            std::string var_name = clang_getCString(varName);
            std::string var_type = clang_getCString(typeName);
            
            if (clang_getCursorLinkage(cursor) == CXLinkage_External || 
                clang_getCursorLinkage(cursor) == CXLinkage_Internal) {
                
                GlobalVarDefinitionInfo globalVarInfo;
                globalVarInfo.var_name = var_name;
                globalVarInfo.var_type = var_type;
                globalVarInfo.definition_location = ClangParser::getLocationInfo(cursor);
                globalVarInfo.definition_source = parser->getCursorSourceText(cursor);
                
                parser->global_var_definitions[var_name] = globalVarInfo;
            }
            
            clang_disposeString(varName);
            clang_disposeString(typeName);
        }
    }
    
    clang_visitChildren(cursor, globalVarCollectorVisitor, client_data);
    return CXChildVisit_Continue;
}

// Enhanced global definition usage scanner visitor
CXChildVisitResult ClangParser::ecgGlobalVarUsageVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    FunctionBodyScanPayload* payload = static_cast<FunctionBodyScanPayload*>(client_data);
    ClangParser* parser = payload->parser;
    ExtendedCallGraphNodeCpp* funcNode = payload->currentNode;
    
    CXCursorKind kind = clang_getCursorKind(cursor);
    
    // Check for variable references (existing functionality)
    if (kind == CXCursor_DeclRefExpr) {
        CXCursor referenced = clang_getCursorReferenced(cursor);
        CXCursorKind refKind = clang_getCursorKind(referenced);
        
        // Handle global variable references
        if (refKind == CXCursor_VarDecl) {
            CXString refVarName = clang_getCursorSpelling(referenced);
            std::string var_name = clang_getCString(refVarName);
            
            // Check if it's a global variable
            if (parser->global_var_definitions.find(var_name) != parser->global_var_definitions.end()) {
                const GlobalVarDefinitionInfo& globalVarDef = parser->global_var_definitions[var_name];
                
                // Check if variable is already recorded for this function
                bool already_exists = false;
                for (const auto& usage : funcNode->global_vars_used) {
                    if (usage.var_name == var_name) {
                        already_exists = true;
                        break;
                    }
                }
                
                if (!already_exists) {
                    GlobalVarUsageInfoCpp usage;
                    usage.var_name = globalVarDef.var_name;
                    usage.var_type = globalVarDef.var_type;
                    usage.definition_location = globalVarDef.definition_location;
                    usage.definition_source = globalVarDef.definition_source;
                    usage.usage_locations.push_back(ClangParser::getExtentInfo(cursor));
                    
                    funcNode->global_vars_used.push_back(usage);
                }
            }
            clang_disposeString(refVarName);
        }
        // Handle enum constant references
        else if (refKind == CXCursor_EnumConstantDecl) {
            CXString enumName = clang_getCursorSpelling(referenced);
            std::string enum_constant_name = clang_getCString(enumName);
            
            // Get the parent enum declaration
            CXCursor enumParent = clang_getCursorSemanticParent(referenced);
            if (clang_getCursorKind(enumParent) == CXCursor_EnumDecl) {
                CXSourceLocation enumLocation = clang_getCursorLocation(enumParent);
                if (clang_Location_isFromMainFile(enumLocation)) {
                    CXString enumTypeName = clang_getCursorSpelling(enumParent);
                    std::string enum_type_name = clang_getCString(enumTypeName);
                    
                    // Create a unique identifier for this enum constant usage
                    std::string usage_id = "enum::" + enum_type_name + "::" + enum_constant_name;
                    
                    // Check if this enum usage is already recorded
                    bool already_exists = false;
                    for (const auto& usage : funcNode->global_vars_used) {
                        if (usage.var_name == usage_id) {
                            already_exists = true;
                            break;
                        }
                    }
                    
                    if (!already_exists) {
                        GlobalVarUsageInfoCpp usage;
                        usage.var_name = usage_id;
                        usage.var_type = "enum " + enum_type_name;
                        usage.definition_location = ClangParser::getLocationInfo(enumParent);
                        usage.definition_source = parser->getCursorSourceText(enumParent);
                        usage.usage_locations.push_back(ClangParser::getExtentInfo(cursor));
                        
                        funcNode->global_vars_used.push_back(usage);
                    }
                    
                    clang_disposeString(enumTypeName);
                }
            }
            clang_disposeString(enumName);
        }
    }
    
    // Check for type references (struct, union, enum types used in declarations)
    else if (kind == CXCursor_TypeRef) {
        CXCursor referenced = clang_getCursorReferenced(cursor);
        CXCursorKind refKind = clang_getCursorKind(referenced);
        
        if (refKind == CXCursor_StructDecl || refKind == CXCursor_UnionDecl || refKind == CXCursor_EnumDecl) {
            CXSourceLocation refLocation = clang_getCursorLocation(referenced);
            if (clang_Location_isFromMainFile(refLocation)) {
                CXString typeName = clang_getCursorSpelling(referenced);
                std::string type_name = clang_getCString(typeName);
                
                std::string kind_prefix;
                if (refKind == CXCursor_StructDecl) kind_prefix = "struct";
                else if (refKind == CXCursor_UnionDecl) kind_prefix = "union";
                else kind_prefix = "enum";
                
                std::string usage_id = kind_prefix + "::" + type_name;
                
                // Check if this type usage is already recorded
                bool already_exists = false;
                for (const auto& usage : funcNode->global_vars_used) {
                    if (usage.var_name == usage_id) {
                        already_exists = true;
                        break;
                    }
                }
                
                if (!already_exists) {
                    GlobalVarUsageInfoCpp usage;
                    usage.var_name = usage_id;
                    usage.var_type = kind_prefix + " " + type_name;
                    usage.definition_location = ClangParser::getLocationInfo(referenced);
                    usage.definition_source = parser->getCursorSourceText(referenced);
                    usage.usage_locations.push_back(ClangParser::getExtentInfo(cursor));
                    
                    funcNode->global_vars_used.push_back(usage);
                }
                
                clang_disposeString(typeName);
            }
        }
    }
    
    // Check for member references (accessing struct/union members)
    else if (kind == CXCursor_MemberRefExpr) {
        CXCursor referenced = clang_getCursorReferenced(cursor);
        if (clang_getCursorKind(referenced) == CXCursor_FieldDecl) {
            // Get the parent struct/union of this field
            CXCursor parentType = clang_getCursorSemanticParent(referenced);
            CXCursorKind parentKind = clang_getCursorKind(parentType);
            
            if ((parentKind == CXCursor_StructDecl || parentKind == CXCursor_UnionDecl)) {
                CXSourceLocation parentLocation = clang_getCursorLocation(parentType);
                if (clang_Location_isFromMainFile(parentLocation)) {
                    CXString parentTypeName = clang_getCursorSpelling(parentType);
                    CXString memberName = clang_getCursorSpelling(referenced);
                    
                    std::string parent_type_name = clang_getCString(parentTypeName);
                    std::string member_name = clang_getCString(memberName);
                    
                    std::string kind_prefix = (parentKind == CXCursor_StructDecl) ? "struct" : "union";
                    std::string usage_id = kind_prefix + "::" + parent_type_name + "::" + member_name;
                    
                    // Check if this member usage is already recorded
                    bool already_exists = false;
                    for (const auto& usage : funcNode->global_vars_used) {
                        if (usage.var_name == usage_id) {
                            already_exists = true;
                            break;
                        }
                    }
                    
                    if (!already_exists) {
                        GlobalVarUsageInfoCpp usage;
                        usage.var_name = usage_id;
                        usage.var_type = kind_prefix + " " + parent_type_name + " member";
                        usage.definition_location = ClangParser::getLocationInfo(parentType);
                        usage.definition_source = parser->getCursorSourceText(parentType);
                        usage.usage_locations.push_back(ClangParser::getExtentInfo(cursor));
                        
                        funcNode->global_vars_used.push_back(usage);
                    }
                    
                    clang_disposeString(parentTypeName);
                    clang_disposeString(memberName);
                }
            }
        }
    }
    
    // Check for variable declarations that use global types
    else if (kind == CXCursor_VarDecl) {
        CXType varType = clang_getCursorType(cursor);
        CXType canonicalType = clang_getCanonicalType(varType);
        CXCursor typeDecl = clang_getTypeDeclaration(canonicalType);
        CXCursorKind typeDeclKind = clang_getCursorKind(typeDecl);
        
        if (typeDeclKind == CXCursor_StructDecl || typeDeclKind == CXCursor_UnionDecl || typeDeclKind == CXCursor_EnumDecl) {
            CXSourceLocation typeDeclLocation = clang_getCursorLocation(typeDecl);
            if (clang_Location_isFromMainFile(typeDeclLocation)) {
                CXString typeName = clang_getCursorSpelling(typeDecl);
                std::string type_name = clang_getCString(typeName);
                
                std::string kind_prefix;
                if (typeDeclKind == CXCursor_StructDecl) kind_prefix = "struct";
                else if (typeDeclKind == CXCursor_UnionDecl) kind_prefix = "union";
                else kind_prefix = "enum";
                
                std::string usage_id = kind_prefix + "::" + type_name + "::declaration";
                
                // Check if this type usage is already recorded
                bool already_exists = false;
                for (const auto& usage : funcNode->global_vars_used) {
                    if (usage.var_name == usage_id) {
                        already_exists = true;
                        break;
                    }
                }
                
                if (!already_exists) {
                    GlobalVarUsageInfoCpp usage;
                    usage.var_name = usage_id;
                    usage.var_type = kind_prefix + " " + type_name + " (used in declaration)";
                    usage.definition_location = ClangParser::getLocationInfo(typeDecl);
                    usage.definition_source = parser->getCursorSourceText(typeDecl);
                    usage.usage_locations.push_back(ClangParser::getExtentInfo(cursor));
                    
                    funcNode->global_vars_used.push_back(usage);
                }
                
                clang_disposeString(typeName);
            }
        }
    }
    
    // Continue visiting children
    clang_visitChildren(cursor, ecgGlobalVarUsageVisitor, client_data);
    return CXChildVisit_Continue;
}

// 新增：获取扩展调用图列表的方法
std::vector<ExtendedCallGraphNodeCpp> ClangParser::getExtendedCallGraphList() const {
    std::vector<ExtendedCallGraphNodeCpp> extended_call_graph_list;
    
    for (const auto& [func_name, node] : internal_extended_call_graph) {
        extended_call_graph_list.push_back(node);
    }
    
    return extended_call_graph_list;
}

// New method: Get ordered global definitions
const std::vector<GlobalDefinitionInfo>& ClangParser::getOrderedGlobalDefinitions() const {
    return ordered_global_definitions;
}

// New method: Get ordered function declarations
const std::vector<FunctionDeclarationInfo>& ClangParser::getOrderedFunctionDeclarations() const {
    return ordered_function_declarations;
}

// New method: Collect ordered global definitions
void ClangParser::collectOrderedGlobalDefinitions() {
    if (!translation_unit) {
        std::cerr << "Cannot collect ordered global definitions: Translation unit is not parsed." << std::endl;
        return;
    }
    
    // This function is now called from generateExtendedCallGraph, so no need for a separate print header.
    
    // Clear previous data
    ordered_global_definitions.clear();
    
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    unsigned int current_order_index = 0;
    
    GlobalDefinitionsOrderedPayload payload;
    payload.parser = this;
    payload.ordered_definitions = &ordered_global_definitions;
    payload.current_order_index = &current_order_index;
    
    clang_visitChildren(rootCursor, orderedGlobalDefinitionsVisitor, &payload);
}

// New method: Collect ordered function declarations
void ClangParser::collectOrderedFunctionDeclarations() {
    if (!translation_unit) {
        std::cerr << "Cannot collect ordered function declarations: Translation unit is not parsed." << std::endl;
        return;
    }
    
    ordered_function_declarations.clear();
    
    CXCursor rootCursor = clang_getTranslationUnitCursor(translation_unit);
    unsigned int current_order_index = 0;
    
    // This set will track function names we have already added to avoid duplicates.
    std::set<std::string> processed_functions;
    
    FunctionDeclarationPayload payload;
    payload.parser = this;
    payload.ordered_declarations = &ordered_function_declarations;
    payload.current_order_index = &current_order_index;
    payload.processed_functions = &processed_functions; // Pass the set to the visitor
    
    clang_visitChildren(rootCursor, orderedFunctionDeclarationVisitor, &payload);
}

// New visitor: Collect global definitions in source order
CXChildVisitResult ClangParser::orderedGlobalDefinitionsVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    GlobalDefinitionsOrderedPayload* payload = static_cast<GlobalDefinitionsOrderedPayload*>(client_data);
    ClangParser* parser = payload->parser;
    std::vector<GlobalDefinitionInfo>* definitions = payload->ordered_definitions;
    unsigned int* order_index = payload->current_order_index;
    
    // Only process items from the main file at the top level
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        return CXChildVisit_Continue;
    }
    
    CXCursor semantic_parent = clang_getCursorSemanticParent(cursor);
    if (clang_getCursorKind(semantic_parent) != CXCursor_TranslationUnit) {
        // Recurse to find top-level declarations inside constructs like extern "C" {}
        return CXChildVisit_Recurse;
    }
    
    CXCursorKind kind = clang_getCursorKind(cursor);
    
    GlobalDefinitionInfo def_info;
    bool should_include = false;
    
    switch (kind) {
        case CXCursor_VarDecl:
        case CXCursor_TypedefDecl:
        case CXCursor_StructDecl:
        case CXCursor_UnionDecl:
        case CXCursor_EnumDecl: {
            // Check if it's a definition (for structs, unions, enums) or just a declaration
            if ((kind == CXCursor_StructDecl || kind == CXCursor_UnionDecl || kind == CXCursor_EnumDecl) && !clang_isCursorDefinition(cursor)) {
                break; // Skip forward declarations of structs/unions/enums
            }

            def_info.location = ClangParser::getLocationInfo(cursor);
            def_info.extent = ClangParser::getExtentInfo(cursor);
            def_info.source_code = parser->getCursorSourceText(cursor);
            def_info.order_index = (*order_index);

            CXString cursor_spelling = clang_getCursorSpelling(cursor);
            def_info.name = clang_getCString(cursor_spelling);
            clang_disposeString(cursor_spelling);

            CXType type = clang_getCursorType(cursor);
            CXString type_spelling = clang_getTypeSpelling(type);
            def_info.full_type = clang_getCString(type_spelling);
            clang_disposeString(type_spelling);
            
            if (kind == CXCursor_VarDecl) def_info.definition_type = "variable";
            else if (kind == CXCursor_TypedefDecl) def_info.definition_type = "typedef";
            else if (kind == CXCursor_StructDecl) def_info.definition_type = "struct";
            else if (kind == CXCursor_UnionDecl) def_info.definition_type = "union";
            else if (kind == CXCursor_EnumDecl) def_info.definition_type = "enum";

            should_include = true;
            break;
        }
        default:
            break;
    }
    
    if (should_include) {
        if (def_info.name.empty()) {
            if (kind == CXCursor_StructDecl) def_info.name = "(anonymous_struct)";
            else if (kind == CXCursor_UnionDecl) def_info.name = "(anonymous_union)";
            else if (kind == CXCursor_EnumDecl) def_info.name = "(anonymous_enum)";
        }
        
        definitions->push_back(def_info);
        (*order_index)++;
    }
    
    // Only continue to siblings, do not recurse into the body of the definition.
    return CXChildVisit_Continue;
}

// New visitor: Collect function declarations in source order
CXChildVisitResult ClangParser::orderedFunctionDeclarationVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    FunctionDeclarationPayload* payload = static_cast<FunctionDeclarationPayload*>(client_data);
    ClangParser* parser = payload->parser;
    std::vector<FunctionDeclarationInfo>* declarations = payload->ordered_declarations;
    unsigned int* order_index = payload->current_order_index;
    std::set<std::string>* processed_functions = payload->processed_functions;
    
    // Only process items from the main file at the top level
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0) {
        return CXChildVisit_Continue;
    }
    
    CXCursor semantic_parent = clang_getCursorSemanticParent(cursor);
    if (clang_getCursorKind(semantic_parent) != CXCursor_TranslationUnit) {
        // Recurse to find top-level declarations inside constructs like extern "C" {}
        return CXChildVisit_Recurse;
    }
    
    CXCursorKind kind = clang_getCursorKind(cursor);
    
    if (kind == CXCursor_FunctionDecl) {
        CXString cursor_spelling = clang_getCursorSpelling(cursor);
        std::string func_name = clang_getCString(cursor_spelling);

        // If we have already processed this function, skip it.
        // We prioritize the first one we see, which should be the declaration if it exists.
        if (processed_functions->count(func_name)) {
            clang_disposeString(cursor_spelling);
            return CXChildVisit_Continue;
        }

        FunctionDeclarationInfo decl_info;
        decl_info.location = ClangParser::getLocationInfo(cursor);
        decl_info.extent = ClangParser::getExtentInfo(cursor);
        decl_info.order_index = (*order_index);
        decl_info.name = func_name;

        if (clang_isCursorDefinition(cursor)) {
            // It's a definition; we only want the signature.
            struct FindBodyVisitorData { CXCursor bodyCursor; };
            auto findBodyVisitor = [](CXCursor c, CXCursor p, CXClientData d) -> CXChildVisitResult {
                if (clang_getCursorKind(c) == CXCursor_CompoundStmt) {
                    auto* data = static_cast<FindBodyVisitorData*>(d);
                    data->bodyCursor = c;
                    return CXChildVisit_Break;
                }
                return CXChildVisit_Continue;
            };

            FindBodyVisitorData bodyData = { clang_getNullCursor() };
            clang_visitChildren(cursor, findBodyVisitor, &bodyData);

            if (!clang_Cursor_isNull(bodyData.bodyCursor)) {
                CXSourceLocation startLoc = clang_getRangeStart(clang_getCursorExtent(cursor));
                CXSourceLocation endLoc = clang_getRangeStart(clang_getCursorExtent(bodyData.bodyCursor));
                
                ExtentInfo signatureExtent;
                clang_getSpellingLocation(startLoc, nullptr, &signatureExtent.start_line, &signatureExtent.start_column, nullptr);
                clang_getSpellingLocation(endLoc, nullptr, &signatureExtent.end_line, &signatureExtent.end_column, nullptr);

                std::string signature = extractTextForExtent(parser->source_file_lines, signatureExtent);
                
                // Trim trailing whitespace
                size_t last = signature.find_last_not_of(" \n\r\t");
                if (std::string::npos != last) {
                    signature.erase(last + 1);
                }
                decl_info.source_code = signature;

            } else {
                // Fallback for a definition without a found body (should not happen)
                decl_info.source_code = parser->getCursorSourceText(cursor);
            }
        } else {
            // It's a simple declaration.
            std::string declaration_source = parser->getCursorSourceText(cursor);
            
            // Ensure function declaration ends with exactly one semicolon
            // First trim whitespace
            size_t end = declaration_source.find_last_not_of(" \n\r\t");
            if (end != std::string::npos) {
                declaration_source = declaration_source.substr(0, end + 1);
            }
            // Then check and add semicolon if needed
            if (!declaration_source.empty() && declaration_source.back() != ';') {
                declaration_source += ";";
            }
            decl_info.source_code = declaration_source;
        }
        
        CXType type = clang_getCursorType(cursor);
        CXString type_spelling = clang_getTypeSpelling(type);
        decl_info.signature = clang_getCString(type_spelling);
        clang_disposeString(type_spelling);
        
        declarations->push_back(decl_info);
        processed_functions->insert(func_name); // Mark this function as processed.
        (*order_index)++;

        clang_disposeString(cursor_spelling);
    }
    
    return CXChildVisit_Continue;
}