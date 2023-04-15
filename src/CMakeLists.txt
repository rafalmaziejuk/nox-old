# --------------------------------------------------
# Set convenience variables
# --------------------------------------------------
set(NOX_LIB_NAME "nox")
set(NOX_OPENGL_LIB_NAME "nox-opengl")

set(NOX_FOLDER_NAME "nox")
set(NOX_RENDERERS_FOLDER_NAME "${NOX_FOLDER_NAME}/renderers")
set(NOX_THIRD_PARTY_FOLDER_NAME "${NOX_FOLDER_NAME}/third_party")

set(NOX_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

# --------------------------------------------------
# Create nox target
# --------------------------------------------------
add_library(${NOX_LIB_NAME})
add_library(nox::${NOX_LIB_NAME} ALIAS ${NOX_LIB_NAME})
target_include_directories(
    ${NOX_LIB_NAME} 
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
target_compile_features(${NOX_LIB_NAME} PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(${NOX_LIB_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${NOX_LIB_NAME} PRIVATE -Wall)
endif()

# --------------------------------------------------
# Specify compile definitions
# --------------------------------------------------
target_compile_definitions(${NOX_LIB_NAME} PUBLIC $<$<CONFIG:Debug>:NOX_DEBUG>)

if(NOX_PLATFORM_WINDOWS)
    target_compile_definitions(${NOX_LIB_NAME} PUBLIC NOX_WIN32)
endif()

if(NOX_PLATFORM_LINUX)
    target_compile_definitions(${NOX_LIB_NAME} PUBLIC NOX_UNIX)
endif()

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${NOX_LIB_NAME} PUBLIC NOX_STATIC)
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
    ${NOX_LIB_NAME}
    PROPERTIES
    VERSION "${NOX_VERSION}"
    SOVERSION "${NOX_VERSION_MAJOR}.${NOX_VERSION_MINOR}"
    DEBUG_POSTFIX "${NOX_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${NOX_RELEASE_POSTFIX}"
    FOLDER "${NOX_FOLDER_NAME}"
    EXPORT_COMPILE_COMMANDS ON
)

# --------------------------------------------------
# Create generated files
# --------------------------------------------------
target_precompile_headers(
    ${NOX_LIB_NAME}
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
    ${NOX_LIB_NAME}
    EXPORT_MACRO_NAME NOX_EXPORT
    EXPORT_FILE_NAME "${PROJECT_SOURCE_DIR}/include/nox/export.h"
    STATIC_DEFINE NOX_STATIC
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
add_subdirectory(${NOX_THIRD_PARTY_DIR}/spdlog spdlog)

target_link_libraries(
    ${NOX_LIB_NAME}
    PRIVATE
    spdlog
)
nox_list_append_global(NOX_INSTALLATION_TARGETS ${NOX_LIB_NAME})

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
file(GLOB_RECURSE NOX_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/nox/*.h")
target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_PUBLIC_HEADERS})

nox_create_project_source_tree(${NOX_LIB_NAME})
