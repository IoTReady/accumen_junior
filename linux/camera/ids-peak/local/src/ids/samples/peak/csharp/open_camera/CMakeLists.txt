cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

project ("open_camera_csharp" CSharp)

message (STATUS "[${PROJECT_NAME}] Processing ${CMAKE_CURRENT_LIST_FILE}")

if (MSVC)
    set (SAMPLE_TARGET_DEFINITION_ADDENDUM WIN32)
endif ()

# Setup target executable with the same name as our project
add_executable (${PROJECT_NAME}
    Program.cs
)

if (NOT DEFINED CMAKE_SIZEOF_VOID_P AND ${CMAKE_CSharp_COMPILER_ID} MATCHES "Microsoft Visual Studio" AND CMAKE_GENERATOR MATCHES "Visual Studio")
    # override platform flag according to the generator architecture as cmake does not detect this correctly
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<IF:$<STREQUAL:"win32",$<LOWER_CASE:"${CMAKE_GENERATOR_PLATFORM}">>,/platform:x86,/platform:x64>)
endif()

# Find packages
if (NOT TARGET ids_peak)
    find_package (ids_peak REQUIRED
        HINTS 
            ../../../../../../../lib/
    )
endif ()

# Set include directories
target_include_directories (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against libraries
target_link_libraries (${PROJECT_NAME}
    ids_peak_dotnet
)

# Call deploy function
# This function will add a post-build steps to your target in order to copy all needed files (e.g. DLL's) to the output directory.
ids_peak_dotnet_deploy(${PROJECT_NAME})

# Set .NET version
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6"  # .NET Framework version 4.6 corresponds to C# 6.0
    VS_DOTNET_REFERENCES
        "Microsoft.CSharp;PresentationCore;System;System.Core;System.Data;System.Drawing;System.Windows.Forms;WindowsBase;netstandard"
    FOLDER "API/DotNet"
)
