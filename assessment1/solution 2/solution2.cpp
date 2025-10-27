#include <fstream>
#include <iostream>
#include <cstdint>
#include <boost/program_options.hpp>
#include <pqxx/pqxx>


#include "QueryFileStructure.h"
#include "json.hpp"
#include "InspectionGroup.h"
#include "../include/InspectionGroupFilter/InspectionGroupFilter.h"
#include "../include/Writers/ResultWriter.h"

using json = nlohmann::json;
namespace po = boost::program_options;

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
 * @param query_struct - The query struct you are going to filter list_records by.
 * @param list_records - List of records.
 * @return A filtered list of list_records.
 */
std::vector<InspectionGroup> filterWithQueryStruct(const QueryFileStructure &query_struct, const std::vector<InspectionGroup> &list_records) {

    /* For debugging */
    QueryFileStructure::dumpQueryStruct(query_struct); // For debugging can remove later... not needed

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
 *
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


void executeQuery(const std::string &path_to_json) {
        json parsed_json = readJsonFile(path_to_json);
        QueryFileStructure query_struct = extractQueryData(parsed_json); // Extract the json into that class we created to filter against.
        std::vector<InspectionGroup> list_records = readRecordsFromDB(); // Fetch all the database results.
        std::vector<InspectionGroup> filtered_records = filterWithQueryStruct(query_struct, list_records); // Now filter list_records
        ResultWriter::writeToTextFile(filtered_records);
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
        std::string hardCodedFileName = "../../data/testjson/sample_json_1.json";
        executeQuery(hardCodedFileName);
    }

    else {
        std::cout << "No arguments entered" << std::endl;
    }

    return 0;
}
