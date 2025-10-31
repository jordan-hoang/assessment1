//
// Created by jorda on 10/30/2025.
//


#include <solution2_core.h>
#include <gtest/gtest.h>
#include "InspectionRegion.h"



TEST(solution2_test, bad_name) {


    //solution2_core::executeQuery();




}





// Optional: demonstrate passing a command-line argument
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if (argc > 1) {
        std::cout << "Running test_solution2 with input file: " << argv[1] << std::endl;
    }

    return RUN_ALL_TESTS();
}