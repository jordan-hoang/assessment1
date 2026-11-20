//
// Created by jorda on 10/23/2025.
//

#ifndef RECORD_H
#define RECORD_H
#include <cstdint>
#include <string>
#include <vector>

#ifdef ASSESSMENT_HAS_DB
namespace pqxx {
    class row;
}
#endif

// Basic container class that handles data, from InspectionRegion database
class InspectionRegion {
private:
    double x_coordinate;
    double y_coordinate;

    std::int64_t category_id;
    std::int64_t group_id;

public:

    explicit InspectionRegion(double x_ = 0, double y_ = 0, std::int64_t category_ = 0, std::int64_t group_ = 0)
        : x_coordinate(x_), y_coordinate(y_), category_id(category_), group_id(group_) {}

    std::string toString() const;
    void set_coordinate(double x, double y);
    void set_category(long long category);
    void set_group(long long group);

    std::int64_t get_group_id() const{return this->group_id;}
    std::int64_t get_category() const{return this->category_id;};
    double get_x_coordinate() const{return this->x_coordinate;};
    double get_y_coordinate() const{return this->y_coordinate;};

    // Read from a data directory containing points.txt, groups.txt and categories.txt
    // (e.g. data/0). Always available; throws std::runtime_error on failure.
    static std::vector<InspectionRegion> readRecordsFromDataDir(const std::string& data_dir);

#ifdef ASSESSMENT_HAS_DB
    // Read from database. Only built when libpqxx was found at configure time.
    static InspectionRegion map_row_to_record(const pqxx::row& row);
    static std::vector<InspectionRegion> readRecordsFromDB(const std::string& conn_string);
#endif

    static std::string toString(const std::vector<InspectionRegion>& inspection_regions);

    ~InspectionRegion() = default;
};



#endif //RECORD_H
