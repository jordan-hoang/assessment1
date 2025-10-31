#include <QueryFileJsonParser.h>
#include <QueryFileStructure.h>
#include <gtest/gtest.h>
#include <nlohmann_json/json.hpp>
#include "ResultFileIO.h"

static std::string G_INPUTFILE;  // file-scope static, only visible here



TEST(SOLUTION_3_TESTING, TEST_AND) {

    // NOTE THIS ASSUMES YOU HAVE DATA from 0 loaded. Perhaps you should write a function to clear tables and load only that.
    //std::string filePath  = G_INPUTFILE.empty() ? "../../../data/testjson/json_and_test_three.json" : G_INPUTFILE;

    std::string filePath = "../../../data/testjson/json_and_test_three.json" ;
    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    nlohmann::json parsed_json;
    EXPECT_NO_THROW({
         parsed_json = ResultFileIO::readJsonFile(filePath);
    });

    //QueryFileStructure query_struct = solution2_core::extractQueryData(parsed_json); // Extract the json into that class we created to filter against.
    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json);


    EXPECT_EQ(query_struct.valid_region[0].p_min.x, 0) << "Invalid p_min, for file " << filePath << "\n";
    EXPECT_EQ(query_struct.valid_region[0].p_min.y, 0) << "Invalid p_min, for file " << filePath << "\n";
    EXPECT_EQ(query_struct.valid_region[0].p_max.x, 1000) << "Invalid p_max, for file " << filePath << "\n";
    EXPECT_EQ(query_struct.valid_region[0].p_max.y, 1000) << "Invalid p_max, for file " << filePath << "\n";

    EXPECT_EQ(query_struct.operator_crop.region.p_min.x, 0) << "Invalid operator_crop.p_min.x, for file " << filePath << "\n";
    EXPECT_EQ(query_struct.operator_crop.region.p_min.y, 0) << "Invalid operator_crop.p_min.y, for file " << filePath << "\n";

    EXPECT_EQ(query_struct.operator_crop.region.p_max.x, 600) << "Invalid operator_crop.p_max.x, for file " << filePath << "\n";
    EXPECT_EQ(query_struct.operator_crop.region.p_max.y, 1000) << "Invalid operator_crop.p_max.y, for file " << filePath << "\n";

    EXPECT_EQ(query_struct.operator_crop.category, 2) << "Invalid Category value " << filePath << "\n";
    EXPECT_EQ(query_struct.operator_crop.proper, false) << "Invalid Proper Value " << filePath << "\n";

    std::set<int64_t> expected = {0, 5};
    EXPECT_EQ(query_struct.operator_crop.one_of_groups, expected) << "Invalid Proper Value " << filePath << "\n";

    std::vector<InspectionRegion> list_records;
    EXPECT_NO_THROW({
        list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    });

    EXPECT_EQ(list_records.size(), 10) << "Invalid dBSize for " << filePath << "\n";
    std::vector<InspectionRegion> filtered_records = query_struct.filterWithQueryStruct(list_records);
    EXPECT_EQ(filtered_records.size(), 5);
    for(const auto &record : filtered_records) {
        EXPECT_EQ(record.get_group_id(), 0);
        EXPECT_EQ(record.get_category(), 2);
        EXPECT_LE(record.get_x_coordinate(), 600);
        EXPECT_LE(record.get_y_coordinate(), 1000);
    }



}

TEST(SOLUTION_3_TESTING, TEST_OR) {

    std::string filePath =  "../../../data/testjson/json_or_test.json";
    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    nlohmann::json parsed_json;
    EXPECT_NO_THROW({
         parsed_json = ResultFileIO::readJsonFile(filePath);
    });

    //QueryFileStructure query_struct = solution2_core::extractQueryData(parsed_json); // Extract the json into that class we created to filter against.
    QueryFileStructure query_struct = QueryFileJsonParser::from_json(parsed_json);

    QueryFileStructure::dumpQueryStruct(query_struct);

    // EXPECT_EQ(query_struct.valid_region.p_min.x, 0) << "Invalid p_min, for file " << filePath << "\n";
    // EXPECT_EQ(query_struct.valid_region.p_min.y, 0) << "Invalid p_min, for file " << filePath << "\n";
    // EXPECT_EQ(query_struct.valid_region.p_max.x, 1000) << "Invalid p_max, for file " << filePath << "\n";
    // EXPECT_EQ(query_struct.valid_region.p_max.y, 1000) << "Invalid p_max, for file " << filePath << "\n";
    //
    // EXPECT_EQ(query_struct.operator_crop.region.p_min.x, 0) << "Invalid operator_crop.p_min.x, for file " << filePath << "\n";
    // EXPECT_EQ(query_struct.operator_crop.region.p_min.y, 0) << "Invalid operator_crop.p_min.y, for file " << filePath << "\n";
    //
    // EXPECT_EQ(query_struct.operator_crop.region.p_max.x, 600) << "Invalid operator_crop.p_max.x, for file " << filePath << "\n";
    // EXPECT_EQ(query_struct.operator_crop.region.p_max.y, 1000) << "Invalid operator_crop.p_max.y, for file " << filePath << "\n";

    // EXPECT_EQ(query_struct.operator_crop.category, 2) << "Invalid Category value " << filePath << "\n";
    // EXPECT_EQ(query_struct.operator_crop.proper, false) << "Invalid Proper Value " << filePath << "\n";
    //
    std::set<int64_t> expected = {0,1,2,3,4,5,6,7,8,9,10};
    EXPECT_EQ(query_struct.operator_crop.one_of_groups, expected) << "Invalid Proper Value " << filePath << "\n";
    //
    // std::vector<InspectionRegion> list_records;
    // EXPECT_NO_THROW({
    //     list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    // });
    //
    // EXPECT_EQ(list_records.size(), 10) << "Invalid dBSize for " << filePath << "\n";
    // std::vector<InspectionRegion> filtered_records = query_struct.filterWithQueryStruct(list_records);
    // EXPECT_EQ(filtered_records.size(), 5);
    // for(const auto &record : filtered_records) {
    //     EXPECT_EQ(record.get_group_id(), 0);
    //     EXPECT_EQ(record.get_category(), 2);
    //     EXPECT_LE(record.get_x_coordinate(), 600);
    //     EXPECT_LE(record.get_y_coordinate(), 1000);
    // }
    //

}



// Optional: demonstrate passing a command-line argument
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (argc > 1) {
        G_INPUTFILE = std::string(argv[1]);
        std::cout << "Running test_solution2 with input file: " << argv[1] << std::endl;
    } else {
        // fallback for manual runs
        G_INPUTFILE = "../../../data/testjson/json_and_test_three.json";
    }

    return RUN_ALL_TESTS();
}