//
// Created by jordan on 10/26/2025.
//


#include <iostream>
#include <QueryFileJsonParser.h>
#include <nlohmann_json/json.hpp>

#include "ArgParser.hpp"
#include "InspectionGroupFilter.h"
#include "QueryFileStructure.h"
#include "ResultFileIO.h"



using json = nlohmann::json;


// Records come from a data directory (points.txt/groups.txt/categories.txt) when
// --data is given, otherwise from PostgreSQL if the build has database support.
static std::vector<InspectionRegion> loadRecords(const std::string &data_dir) {
    if (!data_dir.empty()) {
        return InspectionRegion::readRecordsFromDataDir(data_dir);
    }
#ifdef ASSESSMENT_HAS_DB
    std::string conn_string = InspectionRegion::defaultConnectionString();
    return InspectionRegion::readRecordsFromDB(conn_string);
#else
    std::cout << "Built without database support, reading ../../data/0 (use --data to choose another directory)." << std::endl;
    return InspectionRegion::readRecordsFromDataDir("../../data/0");
#endif
}


void executeQuery(const std::string &path_to_json, const std::string &data_dir) {

    json parsed_json = ResultFileIO::readJsonFile(path_to_json);
    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json); // Extract the json into that class we created to filter against.
    std::cout << std::endl;
    //QueryFileStructure::dumpQueryStruct(query_struct);  UNCOMMENT THIS FOR DUMP GOOD FOR TEST

    std::vector<InspectionRegion> list_records = loadRecords(data_dir); // Fetch all the records.
    std::vector<InspectionRegion> filtered_records = query_struct.filterWithQueryStruct(list_records); // Now filter list_records
    ResultFileIO::writeToTextFile(filtered_records);

}



int main(int argc, char* argv[])
{
    ArgParser args("Allowed options");
    args.flag("help", 'h', "Show help message")
        .option("query", 'q', "Path to json you want to parse")
        .option("data", 'd', "Data directory with points.txt, groups.txt, categories.txt (instead of the database)")
        .flag("test", 't', "Hardcoded path used for fast testing.");

    try {
        args.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (args.has("help")) {
        std::cout << args.help() << std::endl;
        return 1;
    }

    std::string data_dir = args.has("data") ? args.get("data") : "";

    try {
        // Handle the main parse.
        if (args.has("query")) {
            executeQuery(args.get("query"), data_dir);

        } else if (args.has("test")) {
            std::string hardCodedFileName = "../../data/testjson/json_and_test_one.json";
            executeQuery(hardCodedFileName, data_dir);
        }

        else {
            std::cout << "No arguments entered... using relative path for faster testing.";
            //std::string hardCodedFileName = "../../data/testjson/json_and_test_three.json";
            std::string hardCodedFileName = "../../data/testjson/json_or_test.json";
            executeQuery(hardCodedFileName, data_dir);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
