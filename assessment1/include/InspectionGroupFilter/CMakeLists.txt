cmake_minimum_required(VERSION 3.29)

project(InspectionGroupFilter)
set(CMAKE_CXX_STANDARD 17)

add_library(InspectionGroupFilter STATIC
        InspectionGroupFilter.cpp
        InspectionGroupFilter.h
        QueryFileStructure.h
)

target_include_directories(InspectionGroupFilter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(InspectionGroupFilter PRIVATE InspectionGroup)