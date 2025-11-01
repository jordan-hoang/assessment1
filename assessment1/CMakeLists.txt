cmake_minimum_required(VERSION 3.16)
project(AssessmentSolutions)

# This is for vcpkg, good to know
#if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
#endif()


# You can set shared compiler flags or options here
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Sub Directories for libraries
add_subdirectory(libs/InspectionRegion)
add_subdirectory(libs/InspectionRegionFilter)
add_subdirectory(libs/IQueryOperator)
add_subdirectory(libs/Writers)
add_subdirectory(libs/Util)

# External headers (header-only)
add_library(ExternalHeaders INTERFACE)

target_include_directories(ExternalHeaders INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/external/
)



add_subdirectory("solution_1")
add_subdirectory("solution_2")
add_subdirectory("solution_3")

# Copy schema.pgsql from solution_1 to root build folder
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/solution_1/schema.pgsql
        ${CMAKE_BINARY_DIR}/solution_1/schema.pgsql
        COPYONLY
)

option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()