//
// Created by jorda on 10/24/2025.
//

#ifndef FILTERSTRUCT_H
#define FILTERSTRUCT_H


#include <vector>
#include <set>
#include <optional>
#include <cstdint>

// --- Level 3: Basic Coordinates ---

/**
 * @brief Represents a single (x, y) coordinate pair. Used for p_min and p_max.
 */
struct Point {
    double x = 0.0;
    double y = 0.0;
};


/**
 * @brief Represents an axis-aligned rectangular boundary.
 * Used for both the top-level 'valid_region' and the nested 'region'.
 */
struct Region {
    Point p_min;
    Point p_max;
};

// --- Level 2: operator_crop Parameters ---
/**
 * @brief Holds all the parameters defined under the "operator_crop" key.
 */
struct CropQueryParameters {
    Region region;

    // OPTIONAL: Only include points of this category ID.
    std::optional<std::int64_t> category;

    // OPTIONAL: Only include points whose group_id is in this list.
    std::optional<std::set<std::int64_t>> one_of_groups;

    // OPTIONAL: If true, enforce the 'Proper Point' group integrity rule.
    std::optional<bool> proper;
};


struct QueryFileStructure {
    // REQUIRED: The global region used for checking group integrity (Proper Point logic).
    Region valid_region;
    // REQUIRED: The parameters for the specific 'operator_crop' query.
    CropQueryParameters operator_crop;
};







#endif //FILTERSTRUCT_H
