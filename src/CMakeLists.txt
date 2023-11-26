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

nox_list_append_global(NOX_INSTALLATION_TARGETS ${NOX_LIB_NAME})

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
target_compile_features(${NOX_LIB_NAME} PRIVATE cxx_std_17)

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

if(NOX_DISABLE_ASSERTS)
    target_compile_definitions(${NOX_LIB_NAME} PUBLIC NOX_DISABLE_ASSERTS)
endif()

# --------------------------------------------------
# Set properties
# --------------------------------------------------
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
generate_export_header(
    ${NOX_LIB_NAME}
    EXPORT_MACRO_NAME NOX_EXPORT
    EXPORT_FILE_NAME "${PROJECT_SOURCE_DIR}/include/nox/export.h"
    STATIC_DEFINE NOX_STATIC
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_visitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_visitor.h
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_SOURCES})

# --------------------------------------------------
# Add subdirectories with sources
# --------------------------------------------------
add_subdirectory(plugins)

if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(opengl)
endif()

# --------------------------------------------------
# Add public headers to sources (for IDE)
# --------------------------------------------------
file(GLOB_RECURSE NOX_PUBLIC_HEADERS "${PROJECT_SOURCE_DIR}/include/nox/*.h")
target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_PUBLIC_HEADERS})

nox_create_project_source_tree(${NOX_LIB_NAME})
