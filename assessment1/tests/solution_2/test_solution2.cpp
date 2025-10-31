//
// Created by jorda on 10/30/2025.
//


#include <solution2_core.h>
#include <gtest/gtest.h>
#include "ResultFileIO.h"

static std::string G_INPUTFILE;  // file-scope static, only visible here


// void testParseArguments(const std::vector<std::string>& args) {
//     std::vector<char*> argv;
//     for (auto& s : args) {
//         argv.push_back(const_cast<char*>(s.c_str()));
//     }
//     int argc = static_cast<int>(argv.size());
//
//     solution2_core::parseArguments(argc, argv.data());
//
//
//     std::cout << "argv[0] " << argv[0] << std::endl;
// }


TEST(SOLUTION_2_TESTING, myTest) {
    ASSERT_FALSE(G_INPUTFILE.empty()) << "File path not set! Did you pass it from CMake?";

    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current directory: " << cwd << "\n";

    std::string conn_string =
        "host=localhost "
        "port=5432 "      // Standard PostgreSQL port
        "user=postgres "
        "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
        "dbname=postgres ";      /// Generic default database.

    json parsed_json;
    EXPECT_NO_THROW({
         parsed_json = ResultFileIO::readJsonFile(G_INPUTFILE);
    });

    QueryFileStructure query_struct = solution2_core::extractQueryData(parsed_json); // Extract the json into that class we created to filter against.

    //QueryFileStructure::dumpQueryStruct(query_struct);
    EXPECT_EQ(query_struct.valid_region.p_min.x, 0) << "Invalid p_min, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.valid_region.p_min.y, 0) << "Invalid p_min, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.valid_region.p_max.x, 610) << "Invalid p_max, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.valid_region.p_max.y, 700) << "Invalid p_max, for file " << G_INPUTFILE << "\n";

    EXPECT_EQ(query_struct.operator_crop.region.p_min.x, 0) << "Invalid operator_crop.p_min.x, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.operator_crop.region.p_min.y, 0) << "Invalid operator_crop.p_min.y, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.operator_crop.region.p_max.x, 2000) << "Invalid operator_crop.p_max.x, for file " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.operator_crop.region.p_max.y, 2000) << "Invalid operator_crop.p_max.y, for file " << G_INPUTFILE << "\n";

    EXPECT_EQ(query_struct.operator_crop.category, 2) << "Invalid Category value " << G_INPUTFILE << "\n";
    EXPECT_EQ(query_struct.operator_crop.proper, true) << "Invalid Proper Value " << G_INPUTFILE << "\n";


    std::set<int64_t> expected = {0};
    EXPECT_EQ(query_struct.operator_crop.one_of_groups, expected) << "Invalid Proper Value " << G_INPUTFILE << "\n";

    std::vector<InspectionRegion> list_records;

    EXPECT_NO_THROW({
        list_records = InspectionRegion::readRecordsFromDB(conn_string); // Fetch all the database results.
    });

    EXPECT_EQ(list_records.size(), 10);
    std::vector<InspectionRegion> filtered_records = solution2_core::filterWithQueryStruct(query_struct, list_records, false);
    EXPECT_EQ(filtered_records.size(), 6);
    for(const auto &record : filtered_records) {
        EXPECT_EQ(record.get_group_id(), 0);
        EXPECT_EQ(record.get_category(), 2);
    }

}


// Optional: demonstrate passing a command-line argument
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (argc > 1) {
        G_INPUTFILE = std::string(argv[1]);
        std::cout << "Running test_solution2 with input file: " << argv[1] << std::endl;
    } else {
        // fallback for manual runs
        G_INPUTFILE = "../../../data/testjson/sample_json_1.json";
    }

    return RUN_ALL_TESTS();
}