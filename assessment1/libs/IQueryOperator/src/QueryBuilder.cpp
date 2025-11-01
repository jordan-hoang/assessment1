#include "QueryBuilder.h"
#include "LeafNode.h"    // For "operator_crop"
#include "AndOperator.h" // For "operator_and"
#include "OrOperator.h"  // For "operator_or"
#include <iostream>
#include <stdexcept>

/**
 * Combines everything
 * @return std::unique_ptr<IQueryOperator> The fully constructed C++ node  with all Queries combined into 1 object (at the end)
 */
std::unique_ptr<IQueryOperator> parseJson(const nlohmann::json& my_json) {
    // Each query node (leaf or composite) must be a JSON object with exactly one key (the operator type).
    // Base case this is the "leaf node"
    if(my_json.contains("operator_crop")) {
        return std::make_unique<LeafNode>(my_json); // This one isn't recursive it's like the base case!
    }
    if(my_json.contains("operator_and")) {
        std::cout << "OPERATOR_AND TRIGGERED CREATING...\n";
        auto andOp = std::make_unique<AndOperator>();
        for (const auto& child : my_json["operator_and"]) {
            andOp->add_child(parseJson(child));
        }
        return andOp;
    }

    if(my_json.contains("operator_or")) {
        std::cout << "OPERATOR_OR TRIGGERED CREATING...\n";
        auto orOP = std::make_unique<OrOperator>();
        for (const auto& child : my_json["operator_or"]) {
            orOP->add_child(parseJson(child));
        }
        return orOP;
    }

    throw std::runtime_error("Unknown operator type in JSON");

}

/**
 * Pass in the beginning of the json that can be nested a huge amount of times.
 * That would be json.at("query")
 * Since thats the part where you can start having multiple nested crop_operators.
 * Honestly this function is unnecessary you can jsut directly pass above but oh well I
 * already wrote it.
 * @param json_query_root
 * @return - A single node that is combined with ANDS and ORs that you can use to filter your dataset. (You'll have to cast it to InspectionGroup)
 *
 */
 std::unique_ptr<IQueryOperator>  QueryBuilder::parse_query_root(const nlohmann::json& json_query_root){
    std::cout << "Starting query tree construction..." << std::endl;
    return parseJson(json_query_root);
}
