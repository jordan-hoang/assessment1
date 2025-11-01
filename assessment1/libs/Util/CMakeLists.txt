
project(IQueryOperator)

# Wow it really doesn't need anything else!
add_library(UtilityFunctions INTERFACE)


target_include_directories(UtilityFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}  # where ThrowWithContext.hpp lives
)

