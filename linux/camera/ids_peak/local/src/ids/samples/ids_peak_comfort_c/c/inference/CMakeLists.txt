cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project ("inference_c"
    LANGUAGES
        C
)

# Create target executable
add_executable (${PROJECT_NAME}
    main.c
    helper.h
    helper.c
)

# Command compiler to use C99 standard (required by ids_peak_comfort_c)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
)

# Find ids_peak_comfort_c
if (NOT TARGET ids_peak_comfort_c)
    find_package (ids_peak_comfort_c REQUIRED)
endif ()

# Add postbuild step to copy ids_peak_comfort_c dependencies
ids_peak_comfort_c_deploy(${PROJECT_NAME})

# Setup include directories
target_include_directories (${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries (${PROJECT_NAME}
    ids_peak_comfort_c::ids_peak_comfort_c
)

# For unix Build we need the environment variable GENICAM_GENTL32_PATH respectively GENICAM_GENTL64_PATH to find the GenTL producer libraries.
# To set these environment variables a shell script is used. This script can be automatically generated via ids_peak_comfort_c_generate_starter_script.
# The shell script will be saved at ${CMAKE_CURRENT_BINARY_DIR}/${targetName}.sh and automatically copied to the output directory during post-build.

# To run the samples run this script, not the binary.
if(UNIX)
    ids_peak_comfort_c_generate_starter_script(${PROJECT_NAME})
endif()
