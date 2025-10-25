# record_lib/CMakeLists.txt
project(InspectionGroup)
set(CMAKE_CXX_STANDARD 17)

add_library(InspectionGroup STATIC
        InspectionGroup.cpp
        InspectionGroup.h
)

target_include_directories(InspectionGroup PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)