cmake_minimum_required(VERSION 3.29)
project(assessment)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_executable(solution2 "solution2.cpp")

target_include_directories(solution2
        PRIVATE
        ${Boost_INCLUDE_DIRS}
        json.hpp
)

target_link_libraries(solution2
        PRIVATE
        ${Boost_LIBRARIES}
)


