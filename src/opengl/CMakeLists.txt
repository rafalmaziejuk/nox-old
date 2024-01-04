# --------------------------------------------------
# Find required dependencies
# --------------------------------------------------
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

# --------------------------------------------------
# Create OpenGL renderer target
# --------------------------------------------------
add_library(${NOX_OPENGL_LIB_NAME})
add_library(nox::${NOX_OPENGL_LIB_NAME} ALIAS ${NOX_OPENGL_LIB_NAME})
target_include_directories(
    ${NOX_OPENGL_LIB_NAME} 
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Set properties
# --------------------------------------------------
get_target_property(NOX_COMPILE_FEATURES ${NOX_LIB_NAME} COMPILE_FEATURES)
get_target_property(NOX_COMPILE_OPTIONS ${NOX_LIB_NAME} COMPILE_OPTIONS)
get_target_property(NOX_COMPILE_DEFINITIONS ${NOX_LIB_NAME} COMPILE_DEFINITIONS)

set_target_properties(
    ${NOX_OPENGL_LIB_NAME}
    PROPERTIES
    COMPILE_FEATURES "${NOX_COMPILE_FEATURES}"
    COMPILE_OPTIONS "${NOX_COMPILE_OPTIONS}"
    COMPILE_DEFINITIONS "${NOX_COMPILE_DEFINITIONS}"
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
    FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}"
)

target_link_libraries(
    ${NOX_OPENGL_LIB_NAME}
    PRIVATE
    ${NOX_LIB_NAME}
    glad
    OpenGL::GL
)

if(NOT BUILD_SHARED_LIBS)
    target_link_libraries(${NOX_LIB_NAME} PRIVATE ${NOX_OPENGL_LIB_NAME})
endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_framebuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_graphics_pipeline_state.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_graphics_pipeline_state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_program.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_render_pass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_swapchain.h
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

if(NOX_PLATFORM_LINUX)
    add_subdirectory(linux)
endif()

nox_create_project_source_tree(${NOX_OPENGL_LIB_NAME})
