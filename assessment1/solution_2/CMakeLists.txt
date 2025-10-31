cmake_minimum_required(VERSION 3.29)
project(solution2)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)


add_library(solution2_core
        solution2_core.cpp
)


target_include_directories(solution2_core
        PRIVATE
            ${Boost_INCLUDE_DIRS}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(solution2_core
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionRegion
            InspectionGroupFilter
            ResultWriter
            ExternalHeaders
)


### Executable.
add_executable(solution2 "solution2.cpp"
)

target_link_libraries(solution2
        PRIVATE
            solution2_core
            InspectionGroupFilter

)