add_library(InspectionRegion STATIC
        src/InspectionRegion.cpp
)

target_include_directories(InspectionRegion PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)