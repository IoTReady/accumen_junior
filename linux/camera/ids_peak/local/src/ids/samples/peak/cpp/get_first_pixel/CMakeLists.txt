cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project ("get_first_pixel_cpp")
message (STATUS "[${PROJECT_NAME}] Processing ${CMAKE_CURRENT_LIST_FILE}")

# Setup target executable with the same name as our project
add_executable (${PROJECT_NAME}
    get_first_pixel.cpp
)

# Find packages
if (NOT TARGET ids_peak)
    find_package (ids_peak REQUIRED
        HINTS 
            ../../../../../../../lib/
    )
endif ()

if (NOT TARGET ids_peak_ipl)
    find_package (ids_peak_ipl REQUIRED
        HINTS 
            ../../../../../../../lib/
    )
endif()

find_package (Threads REQUIRED)

# Link against libraries
target_link_libraries (${PROJECT_NAME}
    ids_peak
    ids_peak_ipl
    ${CMAKE_THREAD_LIBS_INIT}
)

# Call deploy functions
# These functions will add a post-build steps to your target in order to copy all needed files (e.g. DLL's) to the output directory.
ids_peak_deploy(${PROJECT_NAME})
ids_peak_ipl_deploy(${PROJECT_NAME})

# Set C++ standard to 14 (required for ids_peak)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

# Enable multiprocessing for MSVC
if (MSVC)
    target_compile_options (${PROJECT_NAME}
        PRIVATE "/MP"
    )
endif ()

# For unix Build we need the environment variable GENICAM_GENTL32_PATH respectivily GENICAM_GENTL64_PATH to find the GenTL producer libraries.
# To set these environment variables a shell script is used. This script can be automatically generated via ids_peak_generate_starter_script.
# The shell script will be saved at ${CMAKE_CURRENT_BINARY_DIR}/${targetName}.sh and automatically copied to the output directory during post-build.

# To run the samples run this script, not the binary.
if(UNIX)
    ids_peak_generate_starter_script(${PROJECT_NAME})
endif()
