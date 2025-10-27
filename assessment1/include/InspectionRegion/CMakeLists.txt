add_library(InspectionRegion STATIC
        InspectionRegion.cpp
        InspectionRegion.h
)

target_include_directories(InspectionRegion PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)