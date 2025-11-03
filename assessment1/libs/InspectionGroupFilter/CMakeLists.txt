add_library(InspectionGroupFilter STATIC
        InspectionGroupFilter.cpp
        InspectionGroupFilter.h
        QueryFileStructure.h
)

target_include_directories(InspectionGroupFilter
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(InspectionGroupFilter
        PRIVATE
            InspectionRegion
            IQueryOperator
)
