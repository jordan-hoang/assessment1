cmake_minimum_required(VERSION 3.29)
project(InspectionGroupFilter)

set(CMAKE_CXX_STANDARD 17)

add_library(InspectionGroupFilter
        src/InspectionGroupFilter.cpp
        src/QueryFileStructure.cpp
)

target_include_directories(InspectionGroupFilter
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(InspectionGroupFilter
        PUBLIC
            IQueryOperator
        PRIVATE
            InspectionRegion
)
