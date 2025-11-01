//
// Created by jorda on 10/26/2025.
//

#include "LeafNode.h"

#include <iostream>
#include <utility>
#include "ThrowWithContext.hpp"

/**
 * Basically parses the JSON and turns it into a CropQueryParameters object.
 * @param crop_json_data The JSON Data
 */
LeafNode::LeafNode(const nlohmann::json &crop_json_data) {
    // std::cout << crop_json_data << std::endl; // FOR DEBUG

    const auto& region_json = crop_json_data.at("operator_crop").at("region");
    const auto& p_min_json = region_json.at("p_min");
    const auto& p_max_json = region_json.at("p_max");

    // If we are creating a node... then only 1 allowed.

    Region my_region;
    my_region.p_min.x = p_min_json.at("x").get<double>();
    my_region.p_min.y = p_min_json.at("y").get<double>();

    my_region.p_max.x = p_max_json.at("x").get<double>();
    my_region.p_max.y = p_max_json.at("y").get<double>();

    cropParams.list_region.push_back(my_region);


    auto oc_json = crop_json_data.at("operator_crop");

    if (oc_json.contains("category")) {
        const auto& cat = oc_json.at("category");

        if(cat.is_number_integer()){
            cropParams.list_category.insert(oc_json.at("category").get<std::int64_t>());
        } else if(cat.is_array()) {
            for (const auto& elem : cat) {
                if (!elem.is_number_integer()) {
                    ThrowWithContext::throw_with_context("Invalid type inside category array — must be numbers", __FILE__, __LINE__);
                }
                cropParams.list_category.insert(elem.get<std::int64_t>());
            }
        }
    } else {
        ThrowWithContext::throw_with_context("Invalid 'category' type — must be integer or array of integers", __FILE__, __LINE__);
    }


    if(oc_json.contains("proper")) {
        cropParams.proper.emplace(oc_json.at("proper").get<bool>());
    }

    if (oc_json.contains("one_of_groups")) {
        const nlohmann::json& groups_array = oc_json.at("one_of_groups");
        if (groups_array.is_array()) {
            std::set<std::int64_t> group_set;
            for (const auto& group_id_json : groups_array) {
                group_set.insert(group_id_json.get<std::int64_t>());
            }
            cropParams.one_of_groups.emplace(std::move(group_set));
        } else {
            std::cerr << "Warning: 'one_of_groups' field exists but is not a JSON array. Ignoring." << std::endl;
        }
    }

    //CropQueryParameters::dumpCropQueryParameters(cropParams);
}


LeafNode::LeafNode(CropQueryParameters p) {
    this->cropParams = std::move(p);
}

// Careful this destroys the original "node"
std::unique_ptr<LeafNode> LeafNode::evaluate() const {
    return std::make_unique<LeafNode>(this->cropParams);
}


CropQueryParameters LeafNode::getCropParams() const {
    return cropParams;
}
