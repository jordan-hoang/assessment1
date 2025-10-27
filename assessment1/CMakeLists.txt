cmake_minimum_required(VERSION 3.16)
project(AssessmentSolutions)

# You can set shared compiler flags or options here
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Sub Directories for libraries
add_subdirectory(include/InspectionRegion)
add_subdirectory(include/InspectionGroupFilter)
add_subdirectory(include/IQueryOperator)
add_subdirectory(include/Writers)

add_library(ExternalHeaders INTERFACE)
target_include_directories(ExternalHeaders INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/external/nlohmann_json
)


add_subdirectory("solution_1")
add_subdirectory("solution_2")
add_subdirectory("solution_3")