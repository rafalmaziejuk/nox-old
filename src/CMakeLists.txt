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

target_compile_options(
    ${NOX_LIB_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>: /W4>
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
)

if(NOX_ENABLE_COVERAGE)
    target_compile_options(
        ${NOX_LIB_NAME}
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: -O0 -g --coverage>
    )

    target_link_libraries(
        ${NOX_LIB_NAME}
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: --coverage>
    )
endif()

# --------------------------------------------------
# Specify compile definitions
# --------------------------------------------------
target_compile_definitions(
    ${NOX_LIB_NAME}
    PRIVATE
    # Windows specific definitions
    $<$<PLATFORM_ID:Windows>: WIN32_LEAN_AND_MEAN>
    $<$<PLATFORM_ID:Windows>: NOX_WINDOWS>

    #Linux specific definitions
    $<$<PLATFORM_ID:Linux>: NOX_LINUX>

    # Compiler specific definitions
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: NOX_USE_PLUGIN_PREFIX>

    # Configuration specific definitions
    $<$<CONFIG:Debug>: NOX_DEBUG>
    $<$<CONFIG:Debug>: NOX_USE_PLUGIN_POSTFIX>

    # Options specific definitions
    $<$<BOOL:${BUILD_SHARED_LIBS}>: NOX_SHARED>
    $<$<BOOL:${NOX_DISABLE_ASSERTS}>: NOX_DISABLE_ASSERTS>
    $<$<BOOL:${NOX_BUILD_RENDERER_OPENGL}>: NOX_BUILD_RENDERER_OPENGL>
)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/asserts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/format_descriptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.h
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
