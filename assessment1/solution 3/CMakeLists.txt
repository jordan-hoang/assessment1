cmake_minimum_required(VERSION 3.29)
project(solution3)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_executable(solution3 "solution3.cpp")

target_include_directories(solution3
        PRIVATE
            ${Boost_INCLUDE_DIRS}
            ../include/external/  # For  nhlohhmann, json and header only libs.
)

target_link_libraries(solution3
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionGroup
            InspectionGroupFilter
            ResultWriter
)

### Add Record header and cpp file to it.
set(INSPECTION_GROUP_SRC_DIR "../include/InspectionGroup")
set(INSPECTION_GROUP_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionGroup")
add_subdirectory(
        ${INSPECTION_GROUP_SRC_DIR} # 1st argument: Source path
        ${INSPECTION_GROUP_BIN_DIR} # 2nd argument: Binary path
)

## Add Inspection Group Filter
set(INSPECTION_FILTER_SRC_DIR "../include/InspectionGroupFilter")
set(INSPECTION_FILTER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionGroupFilter")
add_subdirectory(
        ${INSPECTION_FILTER_SRC_DIR}
        ${INSPECTION_FILTER_BIN_DIR}
)

## Add ResultWriter...
set(RESULT_WRITER_SRC_DIR "")
set(RESULT_WRITER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/Writers")
add_subdirectory(
        "../include/Writers"
        "${CMAKE_CURRENT_BINARY_DIR}/Writers"
) #### Normally you can just do add_subdirectory(../include/writers) if it was nested deeper but you can't so you need to specify a binpath as well.