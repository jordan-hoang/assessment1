//
// Created by jordan on 10/30/2025.
//

#pragma once
#include <QueryFileStructure.h>
#include <nlohmann_json/json.hpp>

namespace QueryFileJsonParser {
    QueryFileStructure from_json(const nlohmann::json& j);
};