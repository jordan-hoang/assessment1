//
// Created by jordan on 10/26/2025.
//


#include <iostream>
#include <QueryFileJsonParser.h>
#include <boost/program_options.hpp>
#include <nlohmann_json/json.hpp>

#include "InspectionGroupFilter.h"
#include "QueryFileStructure.h"
#include "ResultFileIO.h"



using json = nlohmann::json;
namespace po = boost::program_options;


void executeQuery(const std::string &path_to_json) {

    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    json parsed_json = ResultFileIO::readJsonFile(path_to_json);
    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json); // Extract the json into that class we created to filter against.
    std::cout << std::endl;
    //QueryFileStructure::dumpQueryStruct(query_struct);  UNCOMMENT THIS FOR DUMP GOOD FOR TEST

    std::vector<InspectionRegion> list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    std::vector<InspectionRegion> filtered_records = query_struct.filterWithQueryStruct(list_records); // Now filter list_records
    ResultFileIO::writeToTextFile(filtered_records);

}



int main(int argc, char* argv[])
{
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
        std::string hardCodedFileName = "../../data/testjson/json_and_test_one.json";
        executeQuery(hardCodedFileName);
    }

    else {
        std::cout << "No arguments entered... using relative path for faster testing.";
        //std::string hardCodedFileName = "../../data/testjson/json_and_test_three.json";
        std::string hardCodedFileName = "../../data/testjson/json_or_test.json";
        executeQuery(hardCodedFileName);
    }

    return 0;
}
