cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project ("simple_live_qml_cpp")

message (STATUS "[${PROJECT_NAME}] Processing ${CMAKE_CURRENT_LIST_FILE}")

set (CMAKE_AUTOMOC TRUE)
set (CMAKE_AUTORCC TRUE)

if (MSVC)
    set (SAMPLE_TARGET_DEFINITION_ADDENDUM WIN32)
endif ()

# Setup target executable with the same name as our project
add_executable (${PROJECT_NAME} ${SAMPLE_TARGET_DEFINITION_ADDENDUM}
    main.cpp
    backend.cpp
    acquisitionworker.cpp
    imageitem.cpp
    backend.h
    acquisitionworker.h
    imageitem.h
    qml.qrc
    main.qml
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

find_package (Qt5 COMPONENTS Widgets Quick Qml REQUIRED)
set(QT_INSTALL_PREFIX "${_qt5Core_install_prefix}")

# Set include directories
target_include_directories (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against libraries
target_link_libraries (${PROJECT_NAME}
    ids_peak
    ids_peak_ipl
    ${CMAKE_THREAD_LIBS_INIT}
    Qt5::Widgets
    Qt5::Quick
    Qt5::Qml
)

# Call deploy functions
# These functions will add a post-build steps to your target in order to copy all needed files (e.g. DLL's) to the output directory.
ids_peak_deploy(${PROJECT_NAME})
ids_peak_ipl_deploy(${PROJECT_NAME})

if ((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    # ignore errors in headers from qt widgets include directories
    foreach (INCLUDE_DIR ${Qt5Widgets_INCLUDE_DIRS})
        if (NOT "${INCLUDE_DIR}" STREQUAL "/usr/include")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${INCLUDE_DIR}")
        endif ()
    endforeach ()
endif ()

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

if (WIN32)
    # Copy Qt files to the output folder
    message (STATUS "[${PROJECT_NAME}] Add PostBuildStep for copy of the Qt libs. ${QT_INSTALL_PREFIX}")
    add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_INSTALL_PREFIX}/bin/windeployqt"
                --no-translations
                --no-system-d3d-compiler
                --no-webkit2
                --no-angle
                --no-opengl-sw
                -qml
                --qmldir "${CMAKE_CURRENT_LIST_DIR}"#"${QT_INSTALL_PREFIX}/qml"
                --no-qmltooling
                --no-svg
                --dir $<TARGET_FILE_DIR:${PROJECT_NAME}>
                $<TARGET_FILE:${PROJECT_NAME}>
    )
endif ()

# For unix Build we need the environment variable GENICAM_GENTL32_PATH respectivily GENICAM_GENTL64_PATH to find the GenTL producer libraries.
# To set these environment variables a shell script is used. This script can be automatically generated via ids_peak_generate_starter_script.
# The shell script will be saved at ${CMAKE_CURRENT_BINARY_DIR}/${targetName}.sh and automatically copied to the output directory during post-build.

# To run the samples run this script, not the binary.
if(UNIX)
    ids_peak_generate_starter_script(${PROJECT_NAME})
endif()
