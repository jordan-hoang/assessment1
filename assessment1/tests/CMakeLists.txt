# GoogleTest is vendored in libs/external/googletest (v1.15.2), so tests build
# out of the box after cloning, with no system install required.
set(GOOGLETEST_DIR ${PROJECT_SOURCE_DIR}/libs/external/googletest)

# Use the same MSVC runtime (/MD) as the rest of the project to avoid link errors on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Don't install gtest alongside this project, and skip gmock (not used).
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_BINARY_DIR}/googletest EXCLUDE_FROM_ALL)

# Add subdirectories for each solution's tests
add_subdirectory(solution_1_test)
if(ASSESSMENT_HAS_DB)
    # These tests read from a live PostgreSQL database.
    add_subdirectory(solution_2_test)
endif()
add_subdirectory(solution_3_test)
