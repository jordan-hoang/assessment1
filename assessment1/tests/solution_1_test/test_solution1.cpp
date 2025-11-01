//
// Created by jorda on 10/27/2025.
//
#include <gtest/gtest.h>
#include "InspectionRegion.h"

int runSolution1(int argc, char* argv[]);

// Super simple test: construct an InspectionRegion and check default values
TEST(InspectionRegionTest, DefaultConstructor) {
    InspectionRegion region;

    // Example: assume InspectionRegion has an `id` and `name` member
    EXPECT_EQ(region.get_x_coordinate(), 0);          // default id should be 0
    EXPECT_EQ(region.get_x_coordinate(), 0);          // default id should be 0

}

// Can't do this until you refactor the main function file to be super small, and move all helper functoins out for testing.
//
// TEST(solution1, testStuff) {
//     char* argv[] = { (char*)"solution1", (char*)"--data_directory", (char*)"../data/0" };
//     int argc = 3;
//
//     int result = runSolution1(argc, argv);
//     std::cout << "Testing solution_1";
//     EXPECT_EQ(result, 0);  // expect success
// }



// Optional: demonstrate passing a command-line argument
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (argc > 1) {
        std::cout << "Running test_solution1 with input file: " << argv[1] << std::endl;
    }

    return RUN_ALL_TESTS();
}