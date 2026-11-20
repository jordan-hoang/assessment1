add_library(InspectionRegion STATIC
        src/InspectionRegion.cpp
)

target_include_directories(InspectionRegion PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(InspectionRegion
        PRIVATE
            UtilityFunctions
)

# Database support is optional. ASSESSMENT_HAS_DB is set in the top-level
# CMakeLists.txt when libpqxx is found; without it, records come from data files.
if(ASSESSMENT_HAS_DB)
    target_sources(InspectionRegion PRIVATE src/InspectionRegionDB.cpp)
    target_link_libraries(InspectionRegion PRIVATE libpqxx::pqxx)
    target_compile_definitions(InspectionRegion PUBLIC ASSESSMENT_HAS_DB=1)
endif()
