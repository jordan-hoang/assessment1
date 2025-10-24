cmake_minimum_required(VERSION 3.29)
project(assessment)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost COMPONENTS program_options)
if (Boost_FOUND)
    message(STATUS "✅ Found Boost automatically at: ${Boost_INCLUDE_DIRS}")
else()
    message(WARNING "⚠️ Boost not found automatically, using C:/Boost....")


    set(BOOST_ROOT "C:/Boost/boost_1_87_0")
    set(BOOST_INCLUDEDIR "C:/Boost/boost_1_87_0")        # parent folder of 'boost/'
    set(BOOST_LIBRARYDIR "C:/Boost/boost_1_87_0/stage/lib")
    set(Boost_NO_SYSTEM_PATHS TRUE)                       # only use these paths
    find_package(Boost REQUIRED COMPONENTS program_options)


endif()


add_executable(assessment main.cpp
        record.cpp
        record.h)

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


# Optional: print debug info
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")