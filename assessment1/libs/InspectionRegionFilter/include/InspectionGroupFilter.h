//
// Created by jorda on 10/25/2025.
//
#pragma once
#include <vector>
#include <unordered_set>
#include "InspectionRegion.h"
#include "QueryFileStructure.h"


struct QueryFileStructure;

// Utility class that applies filter. Only has static methods.
// Actually you can use Namespace to do the same thing and also not worry about constructors.
// Maybe you can do that when you have time.
namespace InspectionGroupFilter {
    std::vector<InspectionRegion> applyFilter(
        const QueryFileStructure& query_struct,
        const std::vector<InspectionRegion>& inspection_groups
    );

    // Maybe not needed.
    void properFilter(const QueryFileStructure& query_struct, std::vector<InspectionRegion>& result);
    bool passesCroppedFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);
    bool passesCategoryFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);
    bool passesOneOfSetFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);

    bool passesProperFilter(const InspectionRegion &my_group,  const QueryFileStructure &query,
                                   std::unordered_set<int64_t> &failed_region_group_ids);

};

