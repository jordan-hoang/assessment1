//
// Created by jorda on 10/30/2025.
//

#ifndef SOLUTION2_CORE_H
#define SOLUTION2_CORE_H


#include <QueryFileStructure.h>


namespace solution2_core {

    // Basically the main loop of solution_2 we do this so we can test the functions easier.
    // Honestly not necessary anymore since alot of the functoins were moved to libs.
    int parseArguments(int argc, char* argv[]);

    void executeQuery(const std::string &path_to_json);

};
















#endif //SOLUTION2_CORE_H
