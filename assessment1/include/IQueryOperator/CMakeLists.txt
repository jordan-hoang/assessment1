### This is for solution 3 only, decided to make a fresh folder and just keep solution 2.

cmake_minimum_required(VERSION 3.29)

project(IQueryOperator)
set(CMAKE_CXX_STANDARD 17)

add_library(IQueryOperator
        IQueryOperator.h
        LeafNode.cpp
        LeafNode.h
        QueryBuilder.cpp
        QueryBuilder.h
        OrOperator.h
        AndOperator.h
)

target_include_directories(IQueryOperator
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/../external"
)

target_link_libraries(IQueryOperator
        PRIVATE
            InspectionGroup
            InspectionGroupFilter
)

