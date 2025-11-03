add_library(InspectionRegion STATIC
        src/InspectionRegion.cpp
)

target_include_directories(InspectionRegion PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(InspectionRegion
        PRIVATE
            UtilityFunctions
            libpqxx::pqxx
)