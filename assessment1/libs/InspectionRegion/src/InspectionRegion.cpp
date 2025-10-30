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
std::string InspectionRegion::toString() const {
    std::string output;
    output += "X-Coordinate: " + std::to_string(x_coordinate) + " Y-Coordinate: " + std::to_string(y_coordinate);
    output += ", Category: " + std::to_string(category_id);
    output += ", Group: " + std::to_string(group_id);
    return output;
}


/**
* @param row A pqxx::row object containing the fetched data, and turning it to a record.
* @return A fully populated Record object.
*/
InspectionRegion InspectionRegion::map_row_to_record(const pqxx::row& row) {
    // Fetch values and convert using the type-safe 'as<T>()' method
    auto x = row["coord_x"].as<double>();
    auto y = row["coord_y"].as<double>();
    auto category_val = row["category"].as<std::int64_t>();
    auto group_val = row["group_id"].as<std::int64_t>();

    return InspectionRegion(x, y, category_val, group_val);
}


/**
 * @return vector of records from the inspection_region Database table.
 */
std::vector<InspectionRegion> InspectionRegion::readRecordsFromDB(const std::string& conn_string) {

    std::vector<InspectionRegion> list_records;
    pqxx::connection myconnection(conn_string);
    pqxx::work W(myconnection);

    pqxx::result inspection_region_result = W.exec("SELECT * From inspection_region;");
    for (const pqxx::row& row : inspection_region_result) {
        list_records.push_back(InspectionRegion::map_row_to_record(row));
    }

    return list_records;
}