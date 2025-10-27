cmake_minimum_required(VERSION 3.29)
project(solution2)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_executable(solution2 "solution2.cpp")

target_include_directories(solution2
        PRIVATE
            ${Boost_INCLUDE_DIRS}
            ../include/external/
)

target_link_libraries(solution2
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
        ${INSPECTION_REGION_SRC_DIR}
        ${INSPECTION_REGION_BIN_DIR}
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


## Need to add this because of solution 3. This library isn't used at all in solution 2.
## No time to clean up.
set(IQUERY_OPERATOR_SRC_DIR "../include/IQueryOperator")
set(IQUERY_OPERATOR_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/IQueryOperator")
add_subdirectory(
        ${IQUERY_OPERATOR_SRC_DIR}
        ${IQUERY_OPERATOR_BIN_DIR}
)
