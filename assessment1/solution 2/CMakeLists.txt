cmake_minimum_required(VERSION 3.29)
project(solution2)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_executable(solution2 "solution2.cpp" filterStruct.h)

target_include_directories(solution2
        PRIVATE
            ${Boost_INCLUDE_DIRS}
            ../include/external/
)

target_link_libraries(solution2
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionGroup
            InspectionGroupFilter
)

### Add Record header and cpp file to it.
set(INSPECTION_GROUP_SRC_DIR "../include/InspectionGroup")
set(INSPECTION_GROUP_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionGroup")
add_subdirectory(
        ${INSPECTION_GROUP_SRC_DIR} # 1st argument: Source path
        ${INSPECTION_GROUP_BIN_DIR} # 2nd argument: Binary path
)

set(INSPECTION_FILTER_SRC_DIR "../include/InspectionGroupFilter")
set(INSPECTION_FILTER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionGroupFilter")
add_subdirectory(
        ${INSPECTION_FILTER_SRC_DIR}
        ${INSPECTION_FILTER_BIN_DIR}
)