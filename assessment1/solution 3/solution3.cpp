//
// Created by jordan on 10/26/2025.
//


#include <fstream>
#include <iostream>
#include <pqxx/pqxx>
#include <boost/program_options.hpp>

#include "InspectionGroupFilter.h"
#include "QueryFileStructure.h"
#include "ResultWriter.h"
#include "json.hpp"
#include "QueryBuilder.h"


using json = nlohmann::json;
namespace po = boost::program_options;


/**
 * Reads a JSON file from path and returns the parsed object.
 * @param queryFilePath The path to the JSON file.
 * @return The parsed json object, or an empty object on failure.
 */
json readJsonFile(const std::string& queryFilePath) {

    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << '\n';


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
 * Extracts all necessary query parameters from a parsed JSON object
 * into a QueryFileStructure object.
 * * @param my_json The parsed nlohmann::json object (root of the query file).
 * @return A fully populated QueryFileStructure object.
 * @throws json::exception or std::runtime_error if required fields are missing.
 */
QueryFileStructure extractQueryData(const json& my_json) {
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
        // FOR THIS WE NEED TO USE THE RECURSIVE STUFF WE JUST CREATED.
        //const json& oc_json = my_json.at("query").at("operator_crop");

        auto root = QueryBuilder::parse_query_root(my_json.at("query"));
        auto result = root->evaluate();  // executes all the ANDS and OR Operations.







    } catch (const json::exception& e) {
        // Catch exceptions caused by missing REQUIRED fields (due to .at())
        // or incorrect type conversion (due to .get<T>()).
        std::string error_msg = "Required Data Extraction Error: Missing or malformed required JSON field. Details: ";
        error_msg += e.what();
        throw std::runtime_error(error_msg);
    }

    return query_data;
}


/**
 * @param row A pqxx::row object containing the fetched data, and turning it to a record.
 * @return A fully populated Record object.
 */
InspectionGroup map_row_to_record(const pqxx::row& row) {
    // Fetch values and convert using the type-safe 'as<T>()' method
    auto x = row["coord_x"].as<double>();
    auto y = row["coord_y"].as<double>();
    auto category_val = row["category"].as<std::int64_t>();
    auto group_val = row["group_id"].as<std::int64_t>();

    return InspectionGroup(x, y, category_val, group_val);
}


/**
 * @param queryFilePath
 * @return vector of records from the inspection_region table.
 */
std::vector<InspectionGroup> readRecordsFromDB() {
    std::string connection_string =
            "host=localhost "
            "port=5432 "      // Standard PostgreSQL port
            "user=postgres "
            "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
            "dbname=postgres ";      /// Generic default database.

    std::vector<InspectionGroup> list_records;

    pqxx::connection myconnection(connection_string);
    pqxx::work W(myconnection);

    pqxx::result inspection_region_result = W.exec("SELECT * From inspection_region;");
    for (const pqxx::row& row : inspection_region_result) {
        list_records.push_back(map_row_to_record(row));
    }

    return list_records;
}



/**
 *
 * @param query_struct - The query struct you are going to filter the list_records by.
 * @param list_records - List of records.
 * @return A filtered list of list_records.
 */
std::vector<InspectionGroup> filterWithQueryStruct(const QueryFileStructure &query_struct, const std::vector<InspectionGroup> &list_records) {

    /* For debugging */
    QueryFileStructure::dumpQueryStruct(query_struct);
    std::cout << std::endl;
    std::cout << std::endl;
    for(InspectionGroup a : list_records) {
        std::cout << a.toString() << std::endl;
    }


    std::vector<InspectionGroup> filtered_records = InspectionGroupFilter::applyFilter(query_struct, list_records);

    // More debugging dumps
    std::cout << "\n\nDumping out filtered records" << std::endl;
    for(InspectionGroup a : filtered_records) {
        std::cout << a.toString() << std::endl;
    }

    return filtered_records;
}





void executeQuery(const std::string &path_to_json) {
    json parsed_json = readJsonFile(path_to_json);
    QueryFileStructure query_struct = extractQueryData(parsed_json); // Extract the json into that class we created to filter against.


    //std::vector<InspectionGroup> list_records = readRecordsFromDB(); // Fetch all the database results.
    //std::vector<InspectionGroup> filtered_records = filterWithQueryStruct(query_struct, list_records); // Now filter list_records
    //ResultWriter::writeToTextFile(filtered_records);
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
        // For faster testing
        std::string hardCodedFileName = "../../data/testjson/json_and_test_one.json";
        executeQuery(hardCodedFileName);
        //std::cout << "No arguments entered" << std::endl;
    }

    return 0;
}
