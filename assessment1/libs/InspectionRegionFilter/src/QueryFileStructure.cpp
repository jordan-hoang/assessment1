
#include "QueryFileStructure.h"

#include <InspectionGroupFilter.h>
#include <iostream>

// Checks if a given point is inside the region.
bool Region::contains(double px, double py) const {
    return (px < p_max.x &&
            px > p_min.x &&
            py < p_max.y &&
            py > p_min.y);
}
// Overloaded alternate way.
bool Region::contains(const InspectionRegion &myGroup) const {
    return contains(myGroup.get_x_coordinate(), myGroup.get_y_coordinate());
}

// And 2 regions.
Region Region::intersectRegions(const Region &a, const Region &b) {
    Region result;
    // You need to take the more strict value so for minimum values take the bigger one.
    result.p_min.x = std::max(a.p_min.x, b.p_min.x);
    result.p_min.y = std::max(a.p_min.y, b.p_min.y);

    // And for maxes take the minimum.
    result.p_max.x = std::min(a.p_max.x, b.p_max.x);
    result.p_max.y = std::min(a.p_max.y, b.p_max.y);

    return result;
}
// Make it as big as possible. Unfortunatley if their are gaps it will include the gaps as well.
Region Region::unionRegions(const Region& a, const Region& b) {
    Region result;
    result.p_min.x = std::min(a.p_min.x, b.p_min.x);
    result.p_min.y = std::min(a.p_min.y, b.p_min.y);
    result.p_max.x = std::max(a.p_max.x, b.p_max.x);
    result.p_max.y = std::max(a.p_max.y, b.p_max.y);
    return result;
}


void CropQueryParameters::dumpCropQueryParameters(const CropQueryParameters &query) {

    if(query.category.has_value()) {
        std::cout << "Category: " << query.category.value() << std::endl;
    } else {
        std::cout << "Category has no value (optional)" << std::endl;
    }

    if(query.one_of_groups.has_value()) {
        const auto& groups_set = query.one_of_groups.value();
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

    if(query.proper.has_value()) {
        std::cout << "Proper: " << query.proper.value() << std::endl;
    } else {
        std::cout << "Proper has no value assigned (optional), so we assume false!" << std::endl;
    }

}

/**
 * Dumps out the values for query for debugging... maybe move this into the struct class or something.
 * @param query
 */
void QueryFileStructure::dumpQueryStruct(const QueryFileStructure &query) {

    auto p_min = query.valid_region.p_min;
    auto p_max = query.valid_region.p_max;

    std::cout << "Valid_Region Min x,y: " << p_min.x << ", " << p_min.y
              << " | Max x,y: " << p_max.x << ", " << p_max.y << '\n';

    std::cout << "Valid_Region_operator_crop Min x,y: " << query.operator_crop.region.p_min.x << ", " << query.operator_crop.region.p_min.y  << std::endl;
    std::cout << "Valid_Region_operator_crop Max x,y: " << query.operator_crop.region.p_max.x << ", " << query.operator_crop.region.p_max.y << std::endl;

    CropQueryParameters::dumpCropQueryParameters(query.operator_crop);
}


std::vector<InspectionRegion> QueryFileStructure::filterWithQueryStruct(const std::vector<InspectionRegion> &list_records, bool debug) const {

    if(debug) { /* For debugging */
        dumpQueryStruct(*this); // For debugging can remove later... not needed
        std::cout << std::endl;
        std::cout << std::endl;
        for(InspectionRegion a : list_records) {
            std::cout << a.toString() << std::endl;
        }
    }

    std::vector<InspectionRegion> filtered_records = InspectionGroupFilter::applyFilter(*this, list_records);

    if(debug) {  // More debugging dumps
        std::cout << "\nDumping out filtered records" << std::endl;
        for(InspectionRegion a : filtered_records) {
            std::cout << a.toString() << std::endl;
        }
    }
    return filtered_records;

}
