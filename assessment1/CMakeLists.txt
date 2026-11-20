cmake_minimum_required(VERSION 3.20)

# The only hard requirements are a C++17 compiler and CMake. Everything else is
# bundled in libs/external (nlohmann_json, googletest) or optional.
#
# PostgreSQL support (libpqxx) is optional and only needed for solution_1.
# If libpqxx is installed with vcpkg and VCPKG_ROOT is set, it is picked up
# without needing -DCMAKE_TOOLCHAIN_FILE. Must come before project().
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project(AssessmentSolutions)

# You can set shared compiler flags or options here
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Database support: used when libpqxx is found, skipped otherwise.
# Pass -DENABLE_DATABASE=OFF to build without it even when libpqxx is installed.
option(ENABLE_DATABASE "Build PostgreSQL support (solution_1, DB reads) if libpqxx is found" ON)
set(ASSESSMENT_HAS_DB OFF)
if(ENABLE_DATABASE)
    find_package(libpqxx CONFIG QUIET)
    if(libpqxx_FOUND)
        set(ASSESSMENT_HAS_DB ON)
        message(STATUS "libpqxx found: building with database support")
    else()
        message(STATUS "libpqxx not found: building without database support (solution_1 skipped, records read from data files)")
    endif()
else()
    message(STATUS "ENABLE_DATABASE=OFF: building without database support")
endif()


# Sub Directories for libraries
add_subdirectory(libs/InspectionRegion)
add_subdirectory(libs/InspectionRegionFilter)
add_subdirectory(libs/IQueryOperator)
add_subdirectory(libs/Writers)
add_subdirectory(libs/Utility)

# External headers (header-only)
add_library(ExternalHeaders INTERFACE)

target_include_directories(ExternalHeaders INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/external/
)

if(ASSESSMENT_HAS_DB)
    add_subdirectory("solution_1")

    # Copy schema.pgsql from solution_1 to root build folder
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/solution_1/schema.pgsql
            ${CMAKE_BINARY_DIR}/solution_1/schema.pgsql
            COPYONLY
    )
endif()
add_subdirectory("solution_2")
add_subdirectory("solution_3")

option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
