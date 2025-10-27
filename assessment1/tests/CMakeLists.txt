# Try to find gtest
find_package(GTest CONFIG)

if(GTest_FOUND)
    message(STATUS "Found GoogleTest, building tests")

    # Add subdirectories for each solution's tests
    add_subdirectory(solution1)
    add_subdirectory(solution2)
    add_subdirectory(solution3)

else()
    message(WARNING "GoogleTest not found, skipping building tests")
endif()