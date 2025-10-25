//
// Created by jorda on 10/25/2025.
//

#include "InspectionGroupFilter.h"

#include "../../solution 2/FilterStruct.h"

// Returns a filtered array.
std::vector<InspectionGroup> InspectionGroupFilter::applyFilter
    (const QueryFileStructure &query_struct, const std::vector<InspectionGroup> &inspection_groups) {

    std::vector<InspectionGroup> result;

    //// select points that meeet the following
    // Check for category.
    if(query_struct.operator_crop.category.has_value()) {
        // Filter the vector by category number.
    }
    if(query_struct.operator_crop.one_of_groups.has_value()) {
        // Filter the vector by w/e is in the set. Check if the set isn't empty.... unless has_value() does that for us.
    }
    if(query_struct.operator_crop.proper.has_value()) {
        // Only include points where the ENTIRE group is valid.

    }


    /// If this is true then we need to do a check to see if all the points are IN VALID_REGION. (not the region inside operrator_crop.)
    bool proper_condition = (
        !query_struct.operator_crop.one_of_groups.has_value() || query_struct.operator_crop.proper);

    std::set<int64_t> my_set;
    if((!query_struct.operator_crop.one_of_groups.has_value())) {
        std::set<int64_t> my_set = query_struct.operator_crop.one_of_groups.value();
    }

    // Filter
    for(int i = 0 ; i < inspection_groups.size() ; i++) {

        bool category_condition = (!query_struct.operator_crop.category.has_value() ||
            inspection_groups[i].get_category() == query_struct.operator_crop.category.value() // Category matches
        );

        /// Handle CROPPED region now.
        int x = inspection_groups[i].get_x_coordinate();
        int y = inspection_groups[i].get_y_coordinate();

        query_struct.operator_crop.region.contains(x,y);



        // Expand this if statement, it needs to satisfy multiple conditions.
        if(category_condition) {
            result.push_back(inspection_groups[i]);
        }


    }





    return result;
}
