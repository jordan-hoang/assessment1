add_executable(solution3 "solution3.cpp")

target_link_libraries(solution3
        PRIVATE
            InspectionRegion
            InspectionGroupFilter
            ResultWriter
            ExternalHeaders
            UtilityFunctions
)
