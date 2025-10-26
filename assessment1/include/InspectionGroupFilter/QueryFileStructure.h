//
// Created by jorda on 10/24/2025.
//
// Data struct variables for holding jn the read JSON file.
// + some helper functions

#ifndef FILTERSTRUCT_H
#define FILTERSTRUCT_H


#include <vector>
#include <set>
#include <optional>
#include <cstdint>
#include <iostream>
#include <ostream>

#include "InspectionGroup.h"


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
    // Checks if a given point is inside the region.
    bool contains(int px, int py) const {
        return (px < p_max.x &&
                px > p_min.x &&
                py < p_max.y &&
                py > p_min.y);
    }

    // Overloaded alternate way.
    bool contains(const InspectionGroup &myGroup) const {
        return contains(myGroup.get_x_coordinate(), myGroup.get_y_coordinate());
    }


};

// --- Level 2: operator_crop Parameters ---
/**
 * @brief Holds all the parameters defined under the "operator_crop" key.
 */
struct CropQueryParameters {
    //Req
    Region region;

    std::optional<std::int64_t> category;

    //Only include points whose group_id is in this list.
    std::optional<std::set<std::int64_t>> one_of_groups;

    // If true, enforce the 'Proper Point' group integrity rule. (Where all points must be in valid_region.
    // It seems like if valid_region is only used for this.
    std::optional<bool> proper;
};

// The top level struct. T
struct QueryFileStructure {
    // REQUIRED: The global region used for checking group integrity (Proper Point logic).
    Region valid_region;
    // REQUIRED: The parameters for the specific 'operator_crop' query.
    CropQueryParameters operator_crop;



    /**
     * Dumps out the values for query for debugging... maybe move this into the struct class or something.
     * @param query
     */
    static void dumpQueryStruct(const QueryFileStructure &query) {

        std::cout << "Valid_Region Min x,y: " << query.valid_region.p_min.x << " " << query.valid_region.p_min.y << std::endl;
        std::cout << "Valid_Region Max x,y: " << query.valid_region.p_max.x << " " << query.valid_region.p_max.y << std::endl;

        std::cout << "Valid_Region_operator_crop Min x,y: " << query.operator_crop.region.p_min.x << " " << query.valid_region.p_min.y << std::endl;
        std::cout << "Valid_Region_operator_crop Max x,y: " << query.operator_crop.region.p_max.x << " " << query.valid_region.p_max.y << std::endl;


        if(query.operator_crop.category.has_value()) {
            std::cout << "Category: " << query.operator_crop.category.value() << std::endl;
        } else {
            std::cout << "Category has no value (optional)" << std::endl;
        }

        if(query.operator_crop.one_of_groups.has_value()) {
            const auto& groups_set = query.operator_crop.one_of_groups.value();
            std::cout << "One of Groups: {";
            bool first = true;
            for (std::int64_t group_id : groups_set) {
                if (!first) {
                    std::cout << ", ";
                }
                std::cout << group_id;
                first = false;
            }
            std::cout << "}" << std::endl;
        } else {
            std::cout << "OneOf groups has no value assigned (optional)" << std::endl;
        }

        if(query.operator_crop.proper.has_value()) {
            std::cout << "Proper: " << query.operator_crop.proper.value() << std::endl;
        } else {
            std::cout << "Proper has no value assigned (optional), so we assume false!" << std::endl;
        }

    }

};


#endif //FILTERSTRUCT_H
