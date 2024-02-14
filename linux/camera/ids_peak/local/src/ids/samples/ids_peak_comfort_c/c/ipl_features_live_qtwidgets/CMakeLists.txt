cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Set project name and languagees
project("ipl_features_live_qtwidgets_c"
        LANGUAGES
        C CXX
        )

# Enable automoc and autorcc (needed for Qt)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)

# Append WIN32 to executable setup, otherwise
# Windows will show command prompt when running
# GUI application
if (MSVC)
    set(SAMPLE_TARGET_DEFINITION_ADDENDUM WIN32)
endif ()

# Add executable target and add source files
add_executable(${PROJECT_NAME} ${SAMPLE_TARGET_DEFINITION_ADDENDUM}
        acquisitionworker.cpp
        acquisitionworker.h
        backend.c
        backend.h
        display.cpp
        display.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        iplfeatureswidget.cpp
        iplfeatureswidget.h
        )

# Find ids_peak_comfort_c
if (NOT TARGET ids_peak_comfort_c)
    find_package(ids_peak_comfort_c REQUIRED
            HINTS
            ../../../../../../../lib/
            )
endif ()

# Find std threads package and needed Qt5 components
find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# Set include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        )

# Set target link libraries
target_link_libraries(${PROJECT_NAME}
        ids_peak_comfort_c::ids_peak_comfort_c
        ${CMAKE_THREAD_LIBS_INIT}
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        )

# Set C++ standard
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS NO
        )

# Command compiler to use C99 standard (required by ids_peak_comfort_c)
# If you are not using C++ in your project, uncomment the following lines
# set_target_properties(${PROJECT_NAME} PROPERTIES
#         C_STANDARD 99
#         C_STANDARD_REQUIRED ON
# )

# Configure Qt deprecation warnings
target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        # Disable all the deprecated APIs in Qt <= 5.9.9,
        # resulting in errors instead of warnings
        QT_DISABLE_DEPRECATED_BEFORE=0x050909

        # Disable all (remaining) qt deprecation warnings
        QT_NO_DEPRECATED_WARNINGS
        )

# Compiler specifics
if (!MSVC)
    target_link_libraries(${PROJECT_NAME}
            m # Math library
            )
endif ()

if ((CMAKE_CXX_COMPILER_ID MATCHES GNU) OR (CMAKE_CXX_COMPILER_ID MATCHES Clang))
    # Link against atomic library
    target_link_libraries(${PROJECT_NAME}
            atomic
            )

    # Ignore errors in headers from qt widgets include directories
    foreach (INCLUDE_DIR ${Qt5Widgets_INCLUDE_DIRS})
        if (NOT "${INCLUDE_DIR}" STREQUAL "/usr/include")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${INCLUDE_DIR}")
        endif ()
    endforeach ()
endif ()

# Post build steps
ids_peak_comfort_c_deploy(${PROJECT_NAME})

set(QT_INSTALL_PREFIX "${_qt5Core_install_prefix}")
if (WIN32)
    # Copy Qt dll's to output folder
    message(STATUS "[${PROJECT_NAME}] Add PostBuildStep for copy of the Qt libs. ${QT_INSTALL_PREFIX}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${QT_INSTALL_PREFIX}/bin/windeployqt"
            --no-quick-import
            --no-translations
            --no-system-d3d-compiler
            --no-webkit2
            --no-angle
            --no-opengl-sw
            --no-svg
            --dir $<TARGET_FILE_DIR:${PROJECT_NAME}>
            $<TARGET_FILE:${PROJECT_NAME}>
            )
endif ()

# For unix Build we need the environment variable GENICAM_GENTL32_PATH respectively GENICAM_GENTL64_PATH to find the GenTL producer libraries.
# To set these environment variables a shell script is used. This script can be automatically generated via ids_peak_comfort_c_generate_starter_script.
# The shell script will be saved at ${CMAKE_CURRENT_BINARY_DIR}/${targetName}.sh and automatically copied to the output directory during post-build.

# To run the samples run this script, not the binary.
if (UNIX)
    ids_peak_comfort_c_generate_starter_script(${PROJECT_NAME})
endif ()
