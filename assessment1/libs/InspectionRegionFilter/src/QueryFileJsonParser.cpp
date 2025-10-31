//
// Created by jorda on 10/30/2025.
//

#include "QueryFileJsonParser.h"
#include <LeafNode.h>
#include <QueryBuilder.h>


// bug with solution 2. it's cause it assumes we have minimum 2 nodes and above.
// add check for "null node"??? or maybe the for loop shoold handle it.
QueryFileStructure QueryFileJsonParser::from_json(const nlohmann::json& my_json) {
    // Check if the input object is valid before attempting extraction
    if (my_json.is_null() || my_json.empty()) {
        throw std::runtime_error("Input JSON object is null or empty. Cannot extract data.");
    }

    QueryFileStructure query_data;
    try {
        // --- REQUIRED FIELD EXTRACTION ---
        // 1. Extract Valid Region (required)
        const nlohmann::json& vr_json = my_json.at("valid_region");
        query_data.valid_region.p_min.x = vr_json.at("p_min").at("x").get<double>();
        query_data.valid_region.p_min.y = vr_json.at("p_min").at("y").get<double>();
        query_data.valid_region.p_max.x = vr_json.at("p_max").at("x").get<double>();
        query_data.valid_region.p_max.y = vr_json.at("p_max").at("y").get<double>();

        // 2.) Extract operator_crop Block
        // FOR THIS WE NEED TO USE THE RECURSIVE Stuff / Abstract Syntax Tree.
        auto root = QueryBuilder::parse_query_root(my_json.at("query"));
        auto result = root->evaluate();  // executes all the ANDS and OR Operations, will result in a single LeafNode always.
        query_data.operator_crop = result->getCropParams();


    } catch (const nlohmann::json::exception& e) {
        std::string error_msg = "Required Data Extraction Error: Missing or malformed required JSON field. Details: ";
        error_msg += e.what();
        throw std::runtime_error(error_msg);
    }

    return query_data;
}






