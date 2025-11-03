find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_executable(solution3 "solution3.cpp")

target_include_directories(solution3
        PRIVATE
            ${Boost_INCLUDE_DIRS}
)

target_link_libraries(solution3
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionRegion
            InspectionGroupFilter
            ResultWriter
            ExternalHeaders
)



