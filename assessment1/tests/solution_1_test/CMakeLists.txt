add_executable(test_solution1
        test_solution1.cpp
)

target_link_libraries(test_solution1
        PRIVATE
            GTest::gtest_main
            InspectionRegion
)

add_test(NAME solution1_tests COMMAND test_solution1 "../data/0")
