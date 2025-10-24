//
// Created by jorda on 10/23/2025.
//

#ifndef RECORD_H
#define RECORD_H
#include <string>
#include <boost/container/container_fwd.hpp>

// Basic container class that handles data
class Record {
private:
    double x_coordinate;
    double y_coordinate;

    double category;
    double group;

public:

    explicit Record(double x_ = 0, double y_ = 0, double category_ = 0, double group_ = 0)
        : x_coordinate(x_), y_coordinate(y_), category(category_), group(group_) {}

    std::string toString();
    void set_coordinate(double x, double y);
    void set_category(double category);
    void set_group(double group);


    double get_group() const{return this->group;}
    double get_category() const{return this->category;};
    double get_x_coordinate() const{return this->x_coordinate;};
    double get_y_coordinate() const{return this->y_coordinate;};




    ~Record() = default;
};



#endif //RECORD_H
