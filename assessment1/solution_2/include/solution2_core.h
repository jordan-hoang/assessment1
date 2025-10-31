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


    /**
    * Extracts all necessary query parameters from a parsed JSON object.... Would this work with solution #2?
    * Maybe we can use solutoin 3's variant.
    * into a QueryFileStructure object.
    * * @param my_json The parsed nlohmann::json object (root of the query file).
    * @return A fully populated QueryFileStructure object.
    * @throws json::exception or std::runtime_error if required fields are missing.
    */
    QueryFileStructure extractQueryData(const json& my_json);


    void executeQuery(const std::string &path_to_json);


    /**
    *
    * @param query_struct - The query struct you are going to filter list_records by.
    * @param list_records - List of records.
    * @return A filtered list of list_records.
    */
    std::vector<InspectionRegion> filterWithQueryStruct(const QueryFileStructure &query_struct,
       const std::vector<InspectionRegion> &list_records, bool debug = false);

};
















#endif //SOLUTION2_CORE_H
