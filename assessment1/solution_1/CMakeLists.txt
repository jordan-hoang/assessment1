find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)
add_executable(solution1 "solution1.cpp")

# Include Boost headers (ensure IDE sees them)
target_include_directories(solution1
        PRIVATE
            ${Boost_INCLUDE_DIRS}
)

# Link Boost libraries using Boost_LIBRARIES to avoid imported target issues
target_link_libraries(solution1
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionRegion
            UtilityFunctions
)