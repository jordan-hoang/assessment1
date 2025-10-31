
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
std::vector<Region> Region::intersectRegions(const std::vector<Region> &a, const std::vector<Region> &b) {

    // You need to take the more strict value so for minimum values take the bigger one.
    std::vector<Region> result;
    for (const auto &ra : a) {
        for (const auto &rb : b) {
            Region inter;
            inter.p_min.x = std::max(ra.p_min.x, rb.p_min.x);
            inter.p_min.y = std::max(ra.p_min.y, rb.p_min.y);
            inter.p_max.x = std::min(ra.p_max.x, rb.p_max.x);
            inter.p_max.y = std::min(ra.p_max.y, rb.p_max.y);

            // Only add if intersection is valid
            if (inter.p_min.x < inter.p_max.x && inter.p_min.y < inter.p_max.y)
                result.push_back(inter);
        }
    }

    return result;
}
// Make it as big as possible. Unfortunatley if their are gaps it will include the gaps as well.
std::vector<Region> Region::unionRegions(const std::vector<Region>& a, const std::vector<Region>& b) {
    // You know if they overlap you can simply merge them for better performance, but for now don't do that
    // instead write tests first. than do that.
    std::vector<Region> result;
    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());
    return result;
}


/// Can be used to merge.
bool Region::overlapsOrTouches(const Region& a, const Region& b) {
    return !(a.p_max.x < b.p_min.x || b.p_max.x < a.p_min.x ||
             a.p_max.y < b.p_min.y || b.p_max.y < a.p_min.y);
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


    for (const auto &r : query.operator_crop.list_region) {
        std::cout << "Min(" << r.p_min.x << "," << r.p_min.y << ") Max(" << r.p_max.x << "," << r.p_max.y << ")\n";
    }

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
