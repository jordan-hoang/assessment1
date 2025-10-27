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
            InspectionRegion
            InspectionGroupFilter
            ResultWriter
)

### Add Record header and cpp file to it.
set(INSPECTION_REGION_SRC_DIR "../include/InspectionRegion")
set(INSPECTION_REGION_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionRegion")
add_subdirectory(
        ${INSPECTION_REGION_SRC_DIR} # 1st argument: Source path
        ${INSPECTION_REGION_BIN_DIR} # 2nd argument: Binary path
)

## Add Inspection Group Filter
set(INSPECTION_FILTER_SRC_DIR "../include/InspectionGroupFilter")
set(INSPECTION_FILTER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/InspectionGroupFilter")
add_subdirectory(
        ${INSPECTION_FILTER_SRC_DIR}
        ${INSPECTION_FILTER_BIN_DIR}
)

## Add ResultWriter...
add_subdirectory(
        "../include/Writers"
        "${CMAKE_CURRENT_BINARY_DIR}/Writers"
) #### Normally you can just do add_subdirectory(../include/writers) if it was nested deeper but you can't so you need to specify a binpath as well.


## Add IQueryOperator, this is a lib just for solution3. It has stuff in common with InspectionGroupFilter
set(IQUERY_OPERATOR_SRC_DIR "../include/IQueryOperator")
set(IQUERY_OPERATOR_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/IQueryOperator")
add_subdirectory(
        ${IQUERY_OPERATOR_SRC_DIR}
        ${IQUERY_OPERATOR_BIN_DIR}
)