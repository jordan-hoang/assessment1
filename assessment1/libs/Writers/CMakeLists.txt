# Result writer, its just a contains a bunch of utiilty functions
add_library(ResultWriter STATIC
        ResultFileIO.h
        ResultFileIO.cpp
)

# Anything links to this will get header.
target_include_directories(ResultWriter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Keep implementation private.
target_link_libraries(ResultWriter
        PUBLIC
            ExternalHeaders
        PRIVATE
            InspectionRegion
)