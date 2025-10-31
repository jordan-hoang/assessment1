//
// Created by jorda on 10/24/2025.
//
// Data struct variables for holding jn the read JSON file.
// + some helper functions

#ifndef FILTERSTRUCT_H
#define FILTERSTRUCT_H


#include <set>
#include <optional>
#include <cstdint>
#include "InspectionRegion.h"

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
    bool contains(double px, double py) const;

    // Overloaded alternate way.
    bool contains(const InspectionRegion &myGroup) const;

    // AND operand basically.
    static Region intersectRegions(const Region &a, const Region &b);
    // Make it as big as possible. Unfortunatley if their are gaps it will include the gaps as well. THis pretty much OR operarnd.
    static Region unionRegions(const Region& a, const Region& b);

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


    static void dumpCropQueryParameters(const CropQueryParameters &query);
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
    static void dumpQueryStruct(const QueryFileStructure &query);


    /**
    *
    * @param query_struct - The query struct you are going to filter list_records by.
    * @param list_records - List of records.
    * @return A filtered list of list_records.
    */
    std::vector<InspectionRegion> filterWithQueryStruct(const std::vector<InspectionRegion> &list_records, bool debug = false) const;

};


#endif //FILTERSTRUCT_H
