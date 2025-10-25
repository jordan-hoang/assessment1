# record_lib/CMakeLists.txt
project(InspectionGroupFilter)
set(CMAKE_CXX_STANDARD 17)

add_library(InspectionGroupFilter STATIC
        InspectionGroupFilter.cpp
        InspectionGroupFilter.h
)

target_include_directories(InspectionGroupFilter PUBLIC
        "."
)

target_link_libraries(InspectionGroupFilter
        PRIVATE
            InspectionGroup
)