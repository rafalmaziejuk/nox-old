find_package(OpenGL 4.6 REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL 4.6 not found")
endif()

target_compile_definitions(nox PRIVATE NOX_BUILD_RENDERER_OPENGL)

# --------------------------------------------------
# Create OpenGL renderer target
# --------------------------------------------------
add_library(nox-opengl)
add_library(nox::nox-opengl ALIAS nox-opengl)
target_include_directories(
    nox-opengl 
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
target_compile_features(nox-opengl PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(nox-opengl PRIVATE /W4 /WX)
else()
    target_compile_options(nox-opengl PRIVATE -Wall)
endif()

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(
    nox-opengl
    PROPERTIES
    VERSION "${NOX_VERSION}"
    SOVERSION "${NOX_VERSION_MAJOR}.${NOX_VERSION_MINOR}"
    DEBUG_POSTFIX "${NOX_DEBUG_POSTFIX}"
    RELEASE_POSTFIX "${NOX_RELEASE_POSTFIX}"
)

set_target_properties(nox-opengl PROPERTIES FOLDER "nox/renderers")
set_target_properties(nox-opengl PROPERTIES EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Create generated files
# --------------------------------------------------
generate_export_header(
    nox-opengl
    EXPORT_MACRO_NAME NOX_OPENGL_EXPORT
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/nox_opengl_export.h
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    nox-opengl 
    PRIVATE
    nox
    OpenGL::GL
    spdlog
)
list_append_global(NOX_INSTALLATION_TARGETS nox-opengl)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nox_opengl_export.h
)

target_sources(nox-opengl PRIVATE ${NOX_OPENGL_SOURCES})
