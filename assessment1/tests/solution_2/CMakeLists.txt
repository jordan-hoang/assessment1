add_executable(test_solution2
        test_solution2.cpp
)



target_link_libraries(test_solution2
        PRIVATE
            GTest::gtest_main
            InspectionRegion
            solution2_core
)

add_test(NAME solution2_data_0 COMMAND test_solution2 "../data/0")

# Etc.... can add more command line arguments for testing  here just give diff name for each one.
# add_test(NAME solution2_data_1 COMMAND test_solution2 "../data/1")
