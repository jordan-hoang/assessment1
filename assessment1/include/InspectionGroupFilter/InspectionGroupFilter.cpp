//
// Created by jorda on 10/25/2025.
//

#include "InspectionGroupFilter.h"
#include "QueryFileStructure.h"

/**
 * This is always present in the JSON file, it's mandatory
 * @param my_group - A single row of InspectionGroup.
 * @param query_struct - The struct you are going to filter by.
 * @return true if it passes conditions
 */
bool InspectionGroupFilter::passesCroppedFilter(const InspectionGroup &my_group, const QueryFileStructure &query_struct) {
    return query_struct.operator_crop.region.contains(my_group);
}

bool InspectionGroupFilter::passesCategoryFilter(const InspectionGroup &my_group, const QueryFileStructure &query_struct) {
    // If it doesn't have a value or it matches the query struct than we are "good"
    return !query_struct.operator_crop.category.has_value() ||
        my_group.get_category() == query_struct.operator_crop.category.value();
}

bool InspectionGroupFilter::passesOneOfSetFilter(const InspectionGroup &my_group, const QueryFileStructure &query_struct) {
    if (const auto& group_set_opt = query_struct.operator_crop.one_of_groups; group_set_opt.has_value()) {
        return group_set_opt.value().count(my_group.get_group_id()) > 0;
    }
    return true; // Default to true if filter wasn't added in JSON file.
}

bool InspectionGroupFilter::passesProperFilter(const InspectionGroup &my_group,
                                               const QueryFileStructure &query,
                                               std::unordered_set<int64_t> &failed_region_group_ids ) {

    if (const auto& proper_opt = query.operator_crop.proper; proper_opt.has_value()) {
        // It's its false, we don't care about the filter so we just return true
        if (!proper_opt.value()) {
            return true;
        }
        if(query.valid_region.contains(my_group)) { // Satsifies region.
            return true;
        }

        failed_region_group_ids.insert(my_group.get_group_id());
        return false;
    }

    // If the filter was not supplied (has_value() is false), we pass automatically.
    return true;
}




// Returns a filtered array, based on the inspection gruop parameters
std::vector<InspectionGroup> InspectionGroupFilter::applyFilter
    (const QueryFileStructure &query_struct, const std::vector<InspectionGroup> &inspection_groups) {

    std::vector<InspectionGroup> result;

    std::unordered_set<int64_t> failed_region_group_ids;
    for(const auto &inspection_group : inspection_groups) {
        if (passesCroppedFilter(inspection_group, query_struct) &&
            passesCategoryFilter(inspection_group, query_struct) &&
            passesOneOfSetFilter(inspection_group, query_struct) &&
            passesProperFilter(inspection_group, query_struct, failed_region_group_ids)
            )
            {
                result.push_back(inspection_group);
            }
    } // Nice and compact, but tricky to use with debugger.

    auto new_end = std::remove_if(result.begin(), result.end(),
        [&failed_region_group_ids](const InspectionGroup &group) {
            return failed_region_group_ids.count(group.get_group_id());
        });
    result.erase(new_end, result.end()); // Can also use erase_if but that's C++20.

    return result;
}
