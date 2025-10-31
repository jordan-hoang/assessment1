add_executable(test_solution3
        test_solution3.cpp
)

target_link_libraries(test_solution3
        PRIVATE
            GTest::gtest_main
            InspectionRegion
            InspectionGroupFilter
            ExternalHeaders
            ResultWriter
)

add_test(NAME solution3_data_0 COMMAND test_solution3 "${CMAKE_SOURCE_DIR}/data/testjson/json_and_test_three.json")


# Etc.... can add more command line arguments for testing  here just give diff name for each one.
# add_test(NAME solution2_data_1 COMMAND test_solution2 "../data/1")
