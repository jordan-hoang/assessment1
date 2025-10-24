cmake_minimum_required(VERSION 3.29)
project(assessment)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

if (Boost_FOUND)
    message(STATUS "Found Boost at: ${Boost_INCLUDE_DIRS}")
else()
    message(WARNING "⚠️ Boost not found automatically, using C:/Boost....")
    set(BOOST_ROOT "C:/Boost/boost_1_87_0")
    set(BOOST_INCLUDEDIR "C:/Boost/boost_1_87_0")        # parent folder of 'boost/'
    set(BOOST_LIBRARYDIR "C:/Boost/boost_1_87_0/stage/lib")
    set(Boost_NO_SYSTEM_PATHS TRUE)                       # only use these paths
    find_package(Boost REQUIRED COMPONENTS program_options)
endif()


add_executable(assessment "main.cpp"
        Record.cpp
        Record.h)

# Include Boost headers (ensure IDE sees them)
target_include_directories(assessment
        PRIVATE
        ${Boost_INCLUDE_DIRS}
)

# Link Boost libraries using Boost_LIBRARIES to avoid imported target issues
target_link_libraries(assessment
        PRIVATE
        ${Boost_LIBRARIES}
)

target_link_libraries(assessment PRIVATE libpqxx::pqxx)

# Copy it over.
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../schema.pgsql
        ${CMAKE_BINARY_DIR}/assessment1/schema.pgsql
        COPYONLY)