//
// Created by jordan on 10/26/2025.
//


#include <fstream>
#include <iostream>
#include <QueryFileJsonParser.h>
#include <boost/program_options.hpp>
#include <nlohmann_json/json.hpp>

#include "InspectionGroupFilter.h"
#include "QueryFileStructure.h"
#include "ResultFileIO.h"
#include "LeafNode.h"
#include "QueryBuilder.h"


using json = nlohmann::json;
namespace po = boost::program_options;


/**
 * Reads a JSON file from path and returns the parsed object.
 * @param queryFilePath The path to the JSON file.
 * @return The parsed json object, or an empty object on failure.
 */
json readJsonFile(const std::string& queryFilePath) {
    std::ifstream i(queryFilePath);
    if (!i.is_open()) {
        std::cerr << "Error: Could not open query file: " << queryFilePath << std::endl;
        return {};
    }

    try {
        json j = json::parse(i);
        return j;
    } catch (const json::exception& e) {
        std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
        return {}; // Return an empty JSON object on parsing error
    }

}


/**
 *
 * @param query_struct - The query struct you are going to filter the list_records by.
 * @param list_records - List of records.
 * @return A filtered list of list_records.
 */
std::vector<InspectionRegion> filterWithQueryStruct(const QueryFileStructure &query_struct, const std::vector<InspectionRegion> &list_records) {
    /* For debugging */
    // QueryFileStructure::dumpQueryStruct(query_struct);
    // std::cout << std::endl;
    // std::cout << std::endl;
    // for(InspectionGroup a : list_records) {
    //     std::cout << a.toString() << std::endl;
    // }

    std::vector<InspectionRegion> filtered_records = InspectionGroupFilter::applyFilter(query_struct, list_records);

    // More debugging dumps
    // std::cout << "Dumping out filtered records to console...." << std::endl;
    // for(InspectionRegion a : filtered_records) {
    //     std::cout << a.toString() << std::endl;
    // }

    return filtered_records;
}

void executeQuery(const std::string &path_to_json) {

    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    json parsed_json = readJsonFile(path_to_json);
    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json); // Extract the json into that class we created to filter against.
    std::cout << std::endl;
    //QueryFileStructure::dumpQueryStruct(query_struct);  UNCOMMENT THIS FOR DUMP GOOD FOR TEST
    std::vector<InspectionRegion> list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    std::vector<InspectionRegion> filtered_records = filterWithQueryStruct(query_struct, list_records); // Now filter list_records
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
