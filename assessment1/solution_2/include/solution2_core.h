//
// Created by jorda on 10/30/2025.
//

#ifndef SOLUTION2_CORE_H
#define SOLUTION2_CORE_H


#include <QueryFileStructure.h>
#include <boost/program_options.hpp>
#include <nlohmann_json/json.hpp>

using json = nlohmann::json;
namespace po = boost::program_options;

namespace solution2_core {

    // Basically the main loop of solution_2 we do this so we can test the functions easier.
    int parseArguments(int argc, char* argv[]);



    void executeQuery(const std::string &path_to_json);


};
















#endif //SOLUTION2_CORE_H
