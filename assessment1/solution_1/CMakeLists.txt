cmake_minimum_required(VERSION 3.29)
project(solution1)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)
add_executable(solution1 "solution1.cpp")

# Include Boost headers (ensure IDE sees them)
target_include_directories(solution1
        PRIVATE
        ${Boost_INCLUDE_DIRS}
)

# Link Boost libraries using Boost_LIBRARIES to avoid imported target issues
target_link_libraries(solution1
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionRegion
)

# Copy it over schema files to read from for cmakebuild_debug.
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../schema.pgsql
        ${CMAKE_BINARY_DIR}/assessment1/schema.pgsql
        COPYONLY)

