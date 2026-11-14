#include "clang_parser.h"
#include <json/json.h>  // You'll need to install jsoncpp
#include <memory>
#include <cstring>

// Move convertToJson outside extern "C" block to avoid C-linkage issues
Json::Value convertToJson(const ExtendedCallGraphNodeCpp& node) {
    Json::Value json_node;
    
    json_node["func_name"] = node.func_name;
    json_node["is_definition"] = node.is_definition;
    
    // Location
    Json::Value location;
    location["file"] = node.location.file;
    location["line"] = node.location.line;
    location["column"] = node.location.column;
    location["offset"] = node.location.offset;
    json_node["location"] = location;
    
    // Extent
    Json::Value extent;
    extent["start_line"] = node.extent.start_line;
    extent["start_column"] = node.extent.start_column;
    extent["end_line"] = node.extent.end_line;
    extent["end_column"] = node.extent.end_column;
    json_node["extent"] = extent;
    
    json_node["source_code"] = node.source_code;
    
    // Children and parents
    Json::Value children(Json::arrayValue);
    for (const auto& child : node.children_names) {
        children.append(child);
    }
    json_node["children"] = children;
    
    Json::Value parents(Json::arrayValue);
    for (const auto& parent : node.parent_names) {
        parents.append(parent);
    }
    json_node["parents"] = parents;
    
    // Call sites
    Json::Value call_sites(Json::arrayValue);
    for (const auto& call_site : node.call_sites) {
        Json::Value site;
        site["callee_func_name"] = call_site.callee_func_name;
        Json::Value site_extent;
        site_extent["start_line"] = call_site.extent.start_line;
        site_extent["start_column"] = call_site.extent.start_column;
        site_extent["end_line"] = call_site.extent.end_line;
        site_extent["end_column"] = call_site.extent.end_column;
        site["extent"] = site_extent;
        call_sites.append(site);
    }
    json_node["call_sites"] = call_sites;
    
    // RTE Asserts
    Json::Value rte_asserts(Json::arrayValue);
    for (const auto& assert_info : node.rte_asserts) {
        Json::Value assert_json;
        assert_json["content"] = assert_info.content;
        Json::Value assert_extent;
        assert_extent["start_line"] = assert_info.extent.start_line;
        assert_extent["start_column"] = assert_info.extent.start_column;
        assert_extent["end_line"] = assert_info.extent.end_line;
        assert_extent["end_column"] = assert_info.extent.end_column;
        assert_json["extent"] = assert_extent;
        rte_asserts.append(assert_json);
    }
    json_node["rte_asserts"] = rte_asserts;
    
    // Loops
    Json::Value loops(Json::arrayValue);
    for (const auto& loop_info : node.loops) {
        Json::Value loop_json;
        loop_json["loop_name"] = loop_info.loop_name;
        
        Json::Value loop_location;
        loop_location["file"] = loop_info.location.file;
        loop_location["line"] = loop_info.location.line;
        loop_location["column"] = loop_info.location.column;
        loop_location["offset"] = loop_info.location.offset;
        loop_json["location"] = loop_location;
        
        Json::Value loop_extent;
        loop_extent["start_line"] = loop_info.extent.start_line;
        loop_extent["start_column"] = loop_info.extent.start_column;
        loop_extent["end_line"] = loop_info.extent.end_line;
        loop_extent["end_column"] = loop_info.extent.end_column;
        loop_json["extent"] = loop_extent;
        
        loop_json["source_code"] = loop_info.source_code;
        loops.append(loop_json);
    }
    json_node["loops"] = loops;
    
    // Global variables used
    Json::Value global_vars(Json::arrayValue);
    for (const auto& global_var : node.global_vars_used) {
        Json::Value var_json;
        var_json["var_name"] = global_var.var_name;
        var_json["var_type"] = global_var.var_type;
        
        Json::Value def_location;
        def_location["file"] = global_var.definition_location.file;
        def_location["line"] = global_var.definition_location.line;
        def_location["column"] = global_var.definition_location.column;
        def_location["offset"] = global_var.definition_location.offset;
        var_json["definition_location"] = def_location;
        
        var_json["definition_source"] = global_var.definition_source;
        
        Json::Value usage_locations(Json::arrayValue);
        for (const auto& usage_loc : global_var.usage_locations) {
            Json::Value usage_extent;
            usage_extent["start_line"] = usage_loc.start_line;
            usage_extent["start_column"] = usage_loc.start_column;
            usage_extent["end_line"] = usage_loc.end_line;
            usage_extent["end_column"] = usage_loc.end_column;
            usage_locations.append(usage_extent);
        }
        var_json["usage_locations"] = usage_locations;
        
        global_vars.append(var_json);
    }
    json_node["global_vars_used"] = global_vars;
    
    return json_node;
}

// New helper function to convert GlobalDefinitionInfo to JSON
Json::Value convertToJson(const GlobalDefinitionInfo& def) {
    Json::Value json_def;
    json_def["definition_type"] = def.definition_type;
    json_def["name"] = def.name;
    json_def["full_type"] = def.full_type;
    json_def["source_code"] = def.source_code;
    json_def["order_index"] = def.order_index;

    Json::Value location;
    location["file"] = def.location.file;
    location["line"] = def.location.line;
    location["column"] = def.location.column;
    location["offset"] = def.location.offset;
    json_def["location"] = location;

    Json::Value extent;
    extent["start_line"] = def.extent.start_line;
    extent["start_column"] = def.extent.start_column;
    extent["end_line"] = def.extent.end_line;
    extent["end_column"] = def.extent.end_column;
    json_def["extent"] = extent;

    return json_def;
}

// New helper function to convert FunctionDeclarationInfo to JSON
Json::Value convertToJson(const FunctionDeclarationInfo& decl) {
    Json::Value json_decl;
    json_decl["name"] = decl.name;
    json_decl["signature"] = decl.signature;
    json_decl["source_code"] = decl.source_code;
    json_decl["order_index"] = decl.order_index;

    Json::Value location;
    location["file"] = decl.location.file;
    location["line"] = decl.location.line;
    location["column"] = decl.location.column;
    location["offset"] = decl.location.offset;
    json_decl["location"] = location;

    Json::Value extent;
    extent["start_line"] = decl.extent.start_line;
    extent["start_column"] = decl.extent.start_column;
    extent["end_line"] = decl.extent.end_line;
    extent["end_column"] = decl.extent.end_column;
    json_decl["extent"] = extent;

    return json_decl;
}

extern "C" {
    
struct ClangParserWrapper {
    std::unique_ptr<ClangParser> parser;
    std::string json_output;
};

void* createParser(const char* source_file, const char** args, int num_args) {
    try {
        std::vector<const char*> clang_args;
        for (int i = 0; i < num_args; ++i) {
            clang_args.push_back(args[i]);
        }
        
        auto wrapper = new ClangParserWrapper();
        wrapper->parser = std::make_unique<ClangParser>(source_file, clang_args);
        
        return wrapper;
    } catch (...) {
        return nullptr;
    }
}

void destroyParser(void* parser_ptr) {
    if (parser_ptr) {
        delete static_cast<ClangParserWrapper*>(parser_ptr);
    }
}

int parseFile(void* parser_ptr) {
    if (!parser_ptr) return 0;
    
    auto wrapper = static_cast<ClangParserWrapper*>(parser_ptr);
    try {
        return wrapper->parser->parse() ? 1 : 0;
    } catch (...) {
        return 0;
    }
}

void generateExtendedCallGraph(void* parser_ptr) {
    if (!parser_ptr) return;
    
    auto wrapper = static_cast<ClangParserWrapper*>(parser_ptr);
    try {
        wrapper->parser->generateExtendedCallGraph();
    } catch (...) {
        // Handle error
    }
}

const char* getCallGraphJSON(void* parser_ptr) {
    if (!parser_ptr) return nullptr;
    
    auto wrapper = static_cast<ClangParserWrapper*>(parser_ptr);
    try {
        const auto& call_graph = wrapper->parser->getExtendedCallGraph();
        const auto& global_call_site_orders = wrapper->parser->getGlobalCallSiteOrders();
        const auto& global_call_site_order = wrapper->parser->getGlobalCallSiteOrder();
        const auto& ordered_defs = wrapper->parser->getOrderedGlobalDefinitions();
        const auto& ordered_func_decls = wrapper->parser->getOrderedFunctionDeclarations();
        
        Json::Value root;
        
        // Add call graph nodes
        Json::Value call_graph_json;
        for (const auto& [func_name, node] : call_graph) {
            call_graph_json[func_name] = convertToJson(node);
        }
        root["call_graph"] = call_graph_json;
        
        // Add graph-level call site orders (这个仍然使用pair类型)
        Json::Value orders_json(Json::objectValue); // 修复：初始化为空对象而不是null
        for (const auto& [caller_callee_pair, order] : global_call_site_orders) {
            std::string key = caller_callee_pair.first + "->" + caller_callee_pair.second;
            orders_json[key] = order;
        }
        root["call_site_orders"] = orders_json;
        
        // Add detailed call site order
        Json::Value detailed_orders_json(Json::objectValue); // 修复：初始化为空对象而不是null
        for (const auto& [caller_callee_line_col_tuple, order] : global_call_site_order) {
            std::string key = std::get<0>(caller_callee_line_col_tuple) + "->" + 
                             std::get<1>(caller_callee_line_col_tuple) + "@" + 
                             std::to_string(std::get<2>(caller_callee_line_col_tuple)) + ":" +
                             std::to_string(std::get<3>(caller_callee_line_col_tuple));
            detailed_orders_json[key] = order;
        }
        root["call_site_order"] = detailed_orders_json;
        
        // New: Add ordered global definitions to the JSON root
        Json::Value ordered_defs_json(Json::arrayValue);
        for (const auto& def : ordered_defs) {
            ordered_defs_json.append(convertToJson(def));
        }
        root["ordered_global_definitions"] = ordered_defs_json;
        
        // New: Add ordered function declarations to the JSON root
        Json::Value ordered_func_decls_json(Json::arrayValue);
        for (const auto& decl : ordered_func_decls) {
            ordered_func_decls_json.append(convertToJson(decl));
        }
        root["ordered_function_declarations"] = ordered_func_decls_json;
        
        Json::StreamWriterBuilder builder;
        wrapper->json_output = Json::writeString(builder, root);
        
        return wrapper->json_output.c_str();
    } catch (...) {
        return nullptr;
    }
}

void freeString(const char* str) {
    // The string is managed by the wrapper, so we don't need to free it here
}

// 新增：获取调用图列表的C API
const char* getCallGraphListJSON(void* parser_ptr) {
    if (!parser_ptr) return nullptr;
    
    auto wrapper = static_cast<ClangParserWrapper*>(parser_ptr);
    try {
        const auto& call_graph_list = wrapper->parser->getExtendedCallGraphList();
        
        Json::Value root(Json::arrayValue);
        for (const auto& node : call_graph_list) {
            root.append(convertToJson(node));
        }
        
        Json::StreamWriterBuilder builder;
        wrapper->json_output = Json::writeString(builder, root);
        
        return wrapper->json_output.c_str();
    } catch (...) {
        return nullptr;
    }
}

} // extern "C"
