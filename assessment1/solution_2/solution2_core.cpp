//
// Created by jorda on 10/30/2025.
//

#include "include/solution2_core.h"
#include <nlohmann_json/json.hpp>
#include <iostream>
#include <QueryFileJsonParser.h>
#include "QueryFileStructure.h"
#include "InspectionRegion.h"
#include "ResultFileIO.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using json = nlohmann::json;

int solution2_core::parseArguments(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help message")
        ("query,q", po::value<std::string>(), "Path to json you want to parse")
        ("test,t", "Hardcoded path used for fast testing.")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    // Handle the main parse.
    if (vm.count("query")) {
        std::string path = vm["query"].as<std::string>();
        executeQuery(path);

    } else if (vm.count("test")) {
        std::string hardCodedFileName = "../../data/testjson/sample_json_1.json";
        executeQuery(hardCodedFileName);
    }

    else {
        std::string hardCodedFileName = "../../data/testjson/sample_json_1.json";
        executeQuery(hardCodedFileName);
        std::cout << "No arguments entered" << std::endl;
    }

    return 0;

}


// Generic method that takes in the main and "executes the main loop" actually maybe not even needed.
void solution2_core::executeQuery(const std::string &path_to_json) {

    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    json parsed_json = ResultFileIO::readJsonFile(path_to_json);

    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json); // Extract the json into that class we created to filter against.
    std::vector<InspectionRegion> list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    std::vector<InspectionRegion> filtered_records = query_struct.filterWithQueryStruct(list_records, false); // Now filter list_records
    ResultFileIO::writeToTextFile(filtered_records);

}


