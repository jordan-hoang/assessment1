//
// Created by jorda on 10/26/2025.
//

#include "LeafNode.h"
#include <utility>

/**
 * Basically parses the JSON and turns it into a CropQueryParameters object.
 * @param crop_json_data The JSON Data
 */
LeafNode::LeafNode(const nlohmann::json &crop_json_data) {
    // std::cout << crop_json_data << std::endl; // FOR DEBUG

    CropQueryParameters crop_query;

    const auto& region_json = crop_json_data.at("operator_crop").at("region");
    const auto& p_min_json = region_json.at("p_min");
    const auto& p_max_json = region_json.at("p_max");

    crop_query.region.p_min.x = p_min_json.at("x").get<double>();
    crop_query.region.p_min.y = p_min_json.at("y").get<double>();

    crop_query.region.p_max.x = p_max_json.at("x").get<double>();
    crop_query.region.p_max.y = p_max_json.at("y").get<double>();


    // --- OPTIONAL FIELD EXTRACTION --- Duplicated from solution 2 basically, can probably extract to a function, but no time.
    //
    if (crop_json_data.contains("category")) {
        crop_query.category.emplace(crop_json_data.at("category").get<std::int64_t>());
    }

    if(crop_json_data.contains("proper")) {
        crop_query.proper.emplace(crop_json_data.at("proper").get<bool>());
    }

    if (crop_json_data.contains("one_of_groups")) {
        const nlohmann::json& groups_array = crop_json_data.at("one_of_groups");
        if (groups_array.is_array()) {
            std::set<std::int64_t> group_set;
            for (const auto& group_id_json : groups_array) {
                group_set.insert(group_id_json.get<std::int64_t>());
            }
            crop_query.one_of_groups.emplace(std::move(group_set));
        } else {
            std::cerr << "Warning: 'one_of_groups' field exists but is not a JSON array. Ignoring." << std::endl;
        }
    }

    this->cropParams = std::move(crop_query);

}


LeafNode::LeafNode(CropQueryParameters p) {
    this->cropParams = std::move(p);
}


std::unique_ptr<IQueryOperator> LeafNode::evaluate() const {
    return std::make_unique<LeafNode>(*this);
}


const CropQueryParameters &LeafNode::getCropParams() const {
    return cropParams;
}
