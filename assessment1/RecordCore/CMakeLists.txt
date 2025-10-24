# record_lib/CMakeLists.txt
project(RecordLib)
set(CMAKE_CXX_STANDARD 17)

add_library(RecordCore STATIC
        Record.cpp
        Record.h
)

target_include_directories(RecordCore PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)