//
// Created by jorda on 10/23/2025.
//

#include "InspectionRegion.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
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
std::string InspectionRegion::toString() const {
    std::string output;
    output += "X-Coordinate: " + std::to_string(x_coordinate) + " Y-Coordinate: " + std::to_string(y_coordinate);
    output += ", Category: " + std::to_string(category_id);
    output += ", Group: " + std::to_string(group_id);
    return output;
}


/**
 * @param data_dir Directory holding points.txt, groups.txt and categories.txt, one record per line
 * (same layout as data/0). This is the database-free way to get records.
 */
std::vector<InspectionRegion> InspectionRegion::readRecordsFromDataDir(const std::string& data_dir) {
    std::string dir = data_dir;
    if (!dir.empty() && dir.back() != '/' && dir.back() != '\\') {
        dir += '/';
    }

    std::ifstream points(dir + "points.txt");
    std::ifstream groups(dir + "groups.txt");
    std::ifstream categories(dir + "categories.txt");
    if (!points || !groups || !categories) {
        throw std::runtime_error("Could not open points.txt, groups.txt and categories.txt in: " + data_dir);
    }

    std::vector<InspectionRegion> records;
    std::string line_points, line_groups, line_categories;
    while (std::getline(points, line_points) &&
           std::getline(groups, line_groups) &&
           std::getline(categories, line_categories)) {
        if (line_points.find_first_not_of(" \t\r") == std::string::npos) {
            continue; // skip blank lines
        }
        std::istringstream ss(line_points);
        double x = 0, y = 0;
        if (!(ss >> x >> y)) {
            throw std::runtime_error("Bad line in " + dir + "points.txt: " + line_points);
        }
        // groups/categories are stored as floats like 2.000000000000000000e+00
        auto group_id = static_cast<std::int64_t>(std::stod(line_groups));
        auto category_id = static_cast<std::int64_t>(std::stod(line_categories));
        records.emplace_back(x, y, category_id, group_id);
    }
    return records;
}


std::string InspectionRegion::toString(const std::vector<InspectionRegion>& inspection_regions) {
    std::string output;
    for(const auto &i : inspection_regions) {
        output += i.toString() + "\n";
    }
    return output;
}
