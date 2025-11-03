find_package(Boost COMPONENTS program_options)
find_package(libpqxx CONFIG REQUIRED)

add_library(solution2_core
        solution2_core.cpp
)

target_include_directories(solution2_core
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(solution2_core
        PRIVATE
            ${Boost_LIBRARIES}
            libpqxx::pqxx
            InspectionRegion
            InspectionGroupFilter
            ExternalHeaders
            InspectionGroupFilter
            ResultWriter
)


### Executable.
add_executable(solution2 "solution2.cpp"
)

target_link_libraries(solution2
        PRIVATE
            solution2_core
            InspectionGroupFilter

)