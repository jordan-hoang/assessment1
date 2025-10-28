//
// Created by jorda on 10/23/2025.
//

#ifndef RECORD_H
#define RECORD_H
#include <string>

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

    std::string toString();
    void set_coordinate(double x, double y);
    void set_category(long long category);
    void set_group(long long group);

    std::int64_t get_group_id() const{return this->group_id;}
    std::int64_t get_category() const{return this->category_id;};
    double get_x_coordinate() const{return this->x_coordinate;};
    double get_y_coordinate() const{return this->y_coordinate;};


    ~InspectionRegion() = default;
};



#endif //RECORD_H
