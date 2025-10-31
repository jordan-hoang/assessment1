# Try to find gtest
find_package(GTest CONFIG)

if(GTest_FOUND)
    message(STATUS "Found GoogleTest, building tests")

    # Add subdirectories for each solution's tests
    add_subdirectory(solution_1_test)
    add_subdirectory(solution_2_test)
    add_subdirectory(solution_3_test)

else()
    message(WARNING "GoogleTest not found, skipping building tests")
endif()