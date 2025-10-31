//
// Created by jorda on 10/30/2025.
//

#include "include/solution2_core.h"
#include <pqxx/pqxx>
#include <nlohmann_json/json.hpp>
#include <iostream>
#include <cstdint>

#include "QueryFileStructure.h"
#include "InspectionRegion.h"
#include "InspectionGroupFilter.h"
#include "ResultFileIO.h"



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


/**
 * @param my_json - Takes in the raw Json and returns it a QueryFileStructure that can be used to filer InspectionRegions
 * @return InspectionRegion
 */
QueryFileStructure solution2_core::extractQueryData(const json& my_json) {

    // Check if the input object is valid before attempting extraction
    if (my_json.is_null() || my_json.empty()) {
        throw std::runtime_error("Input JSON object is null or empty. Cannot extract data.");
    }

    QueryFileStructure query_data;
    try {
        // --- REQUIRED FIELD EXTRACTION ---
        // 1. Extract Valid Region (required)
        const json& vr_json = my_json.at("valid_region");
        query_data.valid_region.p_min.x = vr_json.at("p_min").at("x").get<double>();
        query_data.valid_region.p_min.y = vr_json.at("p_min").at("y").get<double>();
        query_data.valid_region.p_max.x = vr_json.at("p_max").at("x").get<double>();
        query_data.valid_region.p_max.y = vr_json.at("p_max").at("y").get<double>();

        // 2. Extract operator_crop Block (required)
        const json& oc_json = my_json.at("query").at("operator_crop");

        // 2a. Extract Required Crop Region
        const json& region_json = oc_json.at("region");
        query_data.operator_crop.region.p_min.x = region_json.at("p_min").at("x").get<double>();
        query_data.operator_crop.region.p_min.y = region_json.at("p_min").at("y").get<double>();
        query_data.operator_crop.region.p_max.x = region_json.at("p_max").at("x").get<double>();
        query_data.operator_crop.region.p_max.y = region_json.at("p_max").at("y").get<double>();


        // --- OPTIONAL FIELD EXTRACTION ---
        // 3. Category (Optional std::int64_t)
        if (oc_json.contains("category")) {
            query_data.operator_crop.category.emplace(oc_json.at("category").get<std::int64_t>());
        }

        // 4. Proper (Optional bool)
        if (oc_json.contains("proper")) {
            query_data.operator_crop.proper.emplace(oc_json.at("proper").get<bool>());
        }

        // 5. one_of_groups (Optional array of std::int64_t) -> Converts to std::set also must be [] if just single element. I guess you can add support for single number if  you wanted.
        if (oc_json.contains("one_of_groups")) {
            const json& groups_array = oc_json.at("one_of_groups");

            if (groups_array.is_array()) {
                std::set<std::int64_t> group_set;
                for (const auto& group_id_json : groups_array) {
                    group_set.insert(group_id_json.get<std::int64_t>());
                }
                query_data.operator_crop.one_of_groups.emplace(std::move(group_set));

            } else {
                 std::cerr << "Warning: 'one_of_groups' field exists but is not a JSON array. Ignoring." << std::endl;
            }
        }

    } catch (const json::exception& e) {
        // Catch exceptions caused by missing REQUIRED fields (due to .at())
        // or incorrect type conversion (due to .get<T>()).
        std::string error_msg = "Required Data Extraction Error: Missing or malformed required JSON field. Details: ";
        error_msg += e.what();
        throw std::runtime_error(error_msg);
    }

    return query_data;
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

    QueryFileStructure query_struct = solution2_core::extractQueryData(parsed_json); // Extract the json into that class we created to filter against.
    //QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json); // Extract the json into that class we created to filter against.
    QueryFileStructure::dumpQueryStruct(query_struct);
    // std::vector<InspectionRegion> list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    // std::vector<InspectionRegion> filtered_records = filterWithQueryStruct(query_struct, list_records); // Now filter list_records
    // ResultWriter::writeToTextFile(filtered_records);
}


