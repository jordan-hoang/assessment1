# Only added when libpqxx is available (see top-level CMakeLists.txt): this
# solution exists to load the data files into PostgreSQL.
add_executable(solution1 "solution1.cpp")

target_link_libraries(solution1
        PRIVATE
            libpqxx::pqxx
            InspectionRegion
            UtilityFunctions
)
