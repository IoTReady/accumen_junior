cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project("message_queue_c" LANGUAGES C CXX)

option(USE_QT6_VERSION OFF)

# Enable automoc and autorcc (needed for Qt)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)

# Append WIN32 to executable setup, otherwise
# Windows will show command prompt when running
# GUI application
if (MSVC)
    set(SAMPLE_TARGET_DEFINITION_ADDENDUM WIN32)
endif ()

# Create target executable
add_executable(${PROJECT_NAME} ${SAMPLE_TARGET_DEFINITION_ADDENDUM}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    queue_worker.cpp
    queue_worker.h
    backend.c
    backend.h
    queue_model.cpp
    queue_model.h
    camera_dialog.cpp
    camera_dialog.h
    camera_model.cpp
    camera_model.h
    control_widget.cpp
    control_widget.h
    cameralist_updater.cpp
    cameralist_updater.h
)

# Command compiler to use C99 standard (required by ids_peak_comfort_c)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED ON
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)

# Find std threads package and needed Qt5 components
find_package(Threads REQUIRED)

if(USE_QT6_VERSION)
    find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
    set(QT_VERSION 6)
else()
    find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)
    set(QT_VERSION 5)
endif()

# Find ids_peak_comfort_c
if (NOT TARGET ids_peak_comfort_c)
    find_package(ids_peak_comfort_c REQUIRED
        HINTS ../../../../../../../lib/
    )
endif ()

# Add postbuild step to copy ids_peak_comfort_c dependencies
ids_peak_comfort_c_deploy(${PROJECT_NAME})

# Setup include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

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

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ids_peak_comfort_c::ids_peak_comfort_c
    ${CMAKE_THREAD_LIBS_INIT}
    Qt${QT_VERSION}::Widgets
    Qt${QT_VERSION}::Core
    Qt${QT_VERSION}::Gui
)

# Post build steps
ids_peak_comfort_c_deploy(${PROJECT_NAME})

set(QT_INSTALL_PREFIX "${_qt${QT_VERSION}Core_install_prefix}")
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
