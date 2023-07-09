find_package(OpenGL 4.6 REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL 4.6 not found")
endif()

target_compile_definitions(${NOX_LIB_NAME} PRIVATE NOX_BUILD_RENDERER_OPENGL)

# --------------------------------------------------
# Create OpenGL renderer target
# --------------------------------------------------
add_library(${NOX_OPENGL_LIB_NAME})
add_library(nox::${NOX_OPENGL_LIB_NAME} ALIAS ${NOX_OPENGL_LIB_NAME})
target_include_directories(
    ${NOX_OPENGL_LIB_NAME} 
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third_party/spdlog/include>"
)

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
if(MSVC)
    target_compile_options(${NOX_OPENGL_LIB_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${NOX_OPENGL_LIB_NAME} PRIVATE -Wall)
endif()

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(
    ${NOX_OPENGL_LIB_NAME}
    PROPERTIES
    VERSION "${NOX_VERSION}"
    SOVERSION "${NOX_VERSION_MAJOR}.${NOX_VERSION_MINOR}"
    DEBUG_POSTFIX "${NOX_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${NOX_RELEASE_POSTFIX}"
    FOLDER "${NOX_RENDERERS_FOLDER_NAME}"
    EXPORT_COMPILE_COMMANDS ON
)

# --------------------------------------------------
# Create generated files
# --------------------------------------------------
target_precompile_headers(
    nox-opengl
    PRIVATE
    [["utilities/assertion.h"]]
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
    ${NOX_OPENGL_LIB_NAME}
    EXPORT_MACRO_NAME NOX_OPENGL_EXPORT
    EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/nox_opengl_export.h"
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
add_subdirectory(${NOX_THIRD_PARTY_DIR}/glad glad)
set_target_properties(
    glad
    PROPERTIES 
    FOLDER ${NOX_THIRD_PARTY_FOLDER_NAME}/glad
)

target_link_libraries(
    ${NOX_OPENGL_LIB_NAME}
    PRIVATE
    ${NOX_LIB_NAME}
    glad
    OpenGL::GL
)
nox_list_append_global(NOX_INSTALLATION_TARGETS ${NOX_OPENGL_LIB_NAME} glad)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_command_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_command_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_pipeline_state.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_pipeline_state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_program.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_swap_chain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_swap_chain.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_vertex_array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_vertex_array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nox_opengl_export.h
)

target_sources(${NOX_OPENGL_LIB_NAME} PRIVATE ${NOX_OPENGL_SOURCES})

if(NOX_PLATFORM_WINDOWS)
    add_subdirectory(windows)
endif()

nox_create_project_source_tree(${NOX_OPENGL_LIB_NAME})
