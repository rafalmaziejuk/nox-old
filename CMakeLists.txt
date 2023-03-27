cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

include(cmake/utilities.cmake)

nox_extract_version()
project(nox VERSION ${NOX_VERSION})

# --------------------------------------------------
# Set global properties/variables
# --------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# --------------------------------------------------
# Specify options
# --------------------------------------------------
option(BUILD_SHARED_LIBS "Build shared libraries." OFF)

# --------------------------------------------------
# Specify build types
# --------------------------------------------------
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Release;Debug")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not specified, using Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "NOX build type: [Release, Debug]" FORCE)
endif()

# --------------------------------------------------
# Add subdirectories
# --------------------------------------------------
add_subdirectory(src)
