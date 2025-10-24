//
// Created by jorda on 10/23/2025.
//

#include "Record.h"

#include <iostream>
#include <string>


// Maybe not necessary we could've done a struct and not use getters/setters.
void Record::set_coordinate(double x, double y) {
    this->x_coordinate = x;
    this->y_coordinate = y;
}
void Record::set_category(long long category) {
    this->category_id = category;
}
void Record::set_group(long long group) {
    this->group_id = group;
}

std::string Record::toString() {
    std::string output;
    output += "X-Coordinate: " + std::to_string(x_coordinate) + " Y-Coordinate: " + std::to_string(y_coordinate);
    output += ", Category: " + std::to_string(category_id);
    output += ", Group: " + std::to_string(group_id);
    return output;
}


