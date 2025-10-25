//
// Created by jorda on 10/25/2025.
//

#ifndef INSPECTIONGROUPFILTER_H
#define INSPECTIONGROUPFILTER_H

#include <vector>
#include "InspectionGroup.h"
struct QueryFileStructure;

class InspectionGroupFilter {

public:
    static std::vector<InspectionGroup> applyFilter(
        const QueryFileStructure& query_struct,
        const std::vector<InspectionGroup>& inspection_groups
    );
private:




};



#endif //INSPECTIONGROUPFILTER_H
