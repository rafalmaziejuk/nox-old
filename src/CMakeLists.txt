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
# Specify nox compile properties
# --------------------------------------------------
target_compile_features(nox PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(nox PRIVATE /W4 /WX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DNOX_DEBUG")
else()
    target_compile_options(nox PRIVATE -Wall)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNOX_DEBUG")
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(nox PUBLIC NOX_SHARED)
endif()

target_precompile_headers(
    nox
    PRIVATE
    [["utilities/logging/log_categories.h"]]
    <algorithm>
    <cstdint>
    <memory>
    <string>
    <string_view>
    <unordered_map>
    <utility>
    <vector>
)

# --------------------------------------------------
# Set nox properties
# --------------------------------------------------
if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

set_target_properties(
    nox
    PROPERTIES
    VERSION "${NOX_VERSION}"
    SOVERSION "${NOX_VERSION_MAJOR}.${NOX_VERSION_MINOR}"
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
)

set_target_properties(nox PROPERTIES FOLDER "nox")
set_target_properties(nox PROPERTIES EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Specify OS specific definitions
# --------------------------------------------------
if(WIN32)
    target_compile_definitions(nox PUBLIC NOX_WIN32)
endif()

# --------------------------------------------------
# Link nox core libraries
# --------------------------------------------------
set(NOX_DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/third_party)

add_subdirectory(${NOX_DEPENDENCIES_DIR}/spdlog spdlog)
set_target_properties(
    spdlog
    PROPERTIES 
    FOLDER "nox/third_party/spdlog"
)

target_link_libraries(
    nox 
    PRIVATE
    spdlog
)
list_append_global(NOX_INSTALLATION_TARGETS nox spdlog)

# --------------------------------------------------
# Add subdirectories with sources
# --------------------------------------------------
add_subdirectory(utilities)

# --------------------------------------------------
# Add public headers to nox sources (for IDE)
# --------------------------------------------------
file(GLOB_RECURSE NOX_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/include/nox/*.h)
target_sources(nox PRIVATE ${NOX_PUBLIC_HEADERS})

create_project_source_tree(nox)
