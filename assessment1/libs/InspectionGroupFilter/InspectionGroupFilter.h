//
// Created by jorda on 10/25/2025.
//

#ifndef INSPECTIONGROUPFILTER_H
#define INSPECTIONGROUPFILTER_H

#include <unordered_set>
#include <vector>
#include "InspectionRegion.h"


struct QueryFileStructure;

// Utility class that applies filter. Only has static methods.
// Actually you can use Namespace to do the same thing and also not worry about constructors.
// Maybe you can do that when you have time.
class InspectionGroupFilter {

public:
    InspectionGroupFilter() = delete;

    static std::vector<InspectionRegion> applyFilter(
        const QueryFileStructure& query_struct,
        const std::vector<InspectionRegion>& inspection_groups
    );
private:

    // Maybe not needed.
    static void properFilter(const QueryFileStructure& query_struct, std::vector<InspectionRegion>& result);
    static bool passesCroppedFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);
    static bool passesCategoryFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);
    static bool passesOneOfSetFilter(const InspectionRegion &my_group,  const QueryFileStructure &query);

    static bool passesProperFilter(const InspectionRegion &my_group,  const QueryFileStructure &query,
                                   std::unordered_set<int64_t> &failed_region_group_ids);

};



#endif //INSPECTIONGROUPFILTER_H
