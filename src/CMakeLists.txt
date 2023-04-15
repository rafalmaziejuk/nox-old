# --------------------------------------------------
# Create nox target
# --------------------------------------------------
add_library(nox)
add_library(nox::nox ALIAS nox)
target_include_directories(
    nox 
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
target_compile_features(nox PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(nox PRIVATE /W4 /WX)
else()
    target_compile_options(nox PRIVATE -Wall)
endif()

# --------------------------------------------------
# Specify compile definitions
# --------------------------------------------------
target_compile_definitions(nox PUBLIC $<$<CONFIG:Debug>:NOX_DEBUG>)

if(NOX_PLATFORM_WINDOWS)
    target_compile_definitions(nox PRIVATE NOX_WIN32)
endif()

if(NOX_PLATFORM_LINUX)
    target_compile_definitions(nox PRIVATE NOX_UNIX)
endif()

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(nox PRIVATE NOX_STATIC)
endif()

# --------------------------------------------------
# Set properties
# --------------------------------------------------
if(BUILD_SHARED_LIBS)
    set(NOX_DEBUG_POSTFIX "-d")
else()
    set(NOX_DEBUG_POSTFIX "-s-d")
    set(NOX_RELEASE_POSTFIX "-s")
endif()

set_target_properties(
    nox
    PROPERTIES
    VERSION "${NOX_VERSION}"
    SOVERSION "${NOX_VERSION_MAJOR}.${NOX_VERSION_MINOR}"
    DEBUG_POSTFIX "${NOX_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${NOX_RELEASE_POSTFIX}"
)

set_target_properties(nox PROPERTIES FOLDER "nox")
set_target_properties(nox PROPERTIES EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Create generated files
# --------------------------------------------------
target_precompile_headers(
    nox
    PRIVATE
    [["core/core.h"]]
    [["utilities/assertion.h"]]
    [["utilities/utilities.h"]]
    <algorithm>
    <cstdint>
    <memory>
    <string>
    <string_view>
    <unordered_map>
    <utility>
    <vector>
)

generate_export_header(
    nox
    EXPORT_MACRO_NAME NOX_EXPORT
    EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/include/nox/export.h
    STATIC_DEFINE NOX_STATIC
)

# --------------------------------------------------
# Set convenience variables
# --------------------------------------------------
set(NOX_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/third_party)

if(NOX_PLATFORM_WINDOWS)
    set(PLATFORM_DIR_SUFFIX "windows/")
else()
    message(FATAL_ERROR "Sources for this platform are not supported")
endif()

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
add_subdirectory(${NOX_DEPENDENCIES_DIR}/spdlog spdlog)

target_link_libraries(
    nox 
    PRIVATE
    spdlog
)
list_append_global(NOX_INSTALLATION_TARGETS nox)

# --------------------------------------------------
# Add subdirectories with sources
# --------------------------------------------------
add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(renderer)
add_subdirectory(utilities)
add_subdirectory(window)

# --------------------------------------------------
# Add public headers to sources (for IDE)
# --------------------------------------------------
file(GLOB_RECURSE NOX_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/include/nox/*.h)
target_sources(nox PRIVATE ${NOX_PUBLIC_HEADERS})

create_project_source_tree(nox)
