cmake_minimum_required(VERSION 3.29)
project(solution2)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)
add_executable(solution2 "solution2.cpp" filterStruct.h)

target_include_directories(solution2
        PRIVATE
        ${Boost_INCLUDE_DIRS}
        json.hpp
)

target_link_libraries(solution2
        PRIVATE
        ${Boost_LIBRARIES}
        libpqxx::pqxx
        RecordCore
)

### Add Record header and cpp file to it.
set(RECORD_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/record_core_lib")
add_subdirectory(
        "../RecordCore" # 1st argument: Source path
        ${CMAKE_CURRENT_BINARY_DIR}/record_core_lib # 2nd argument: Binary path (required for out-of-tree sources)
)