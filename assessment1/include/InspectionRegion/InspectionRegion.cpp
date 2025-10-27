//
// Created by jorda on 10/23/2025.
//

#include "InspectionRegion.h"

#include <iostream>
#include <string>


// Maybe not necessary we could've done a struct and not use getters/setters.
void InspectionRegion::set_coordinate(double x, double y) {
    this->x_coordinate = x;
    this->y_coordinate = y;
}
void InspectionRegion::set_category(long long category) {
    this->category_id = category;
}
void InspectionRegion::set_group(long long group) {
    this->group_id = group;
}

// Dump out for debugging.
std::string InspectionRegion::toString() {
    std::string output;
    output += "X-Coordinate: " + std::to_string(x_coordinate) + " Y-Coordinate: " + std::to_string(y_coordinate);
    output += ", Category: " + std::to_string(category_id);
    output += ", Group: " + std::to_string(group_id);
    return output;
}


