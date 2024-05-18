# --------------------------------------------------
# Add OpenGL renderer sources to be able to test it
# --------------------------------------------------
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif()

target_link_libraries(
    ${NOX_UNIT_TESTS_EXE_NAME}
    PRIVATE
    glad
    OpenGL::GL
)

set(NOX_OPENGL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_buffer.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_command_list.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_command_list.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_context.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_framebuffer.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_graphics_pipeline_state.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_graphics_pipeline_state.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_pipeline_layout.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_pipeline_layout.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_program.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_program.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_render_pass.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_render_pass.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_renderer.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_shader.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_shader.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_state.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_swapchain.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_swapchain.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_texture.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_texture.h
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_vertex_array.cpp
    ${PROJECT_SOURCE_DIR}/src/opengl/gl_vertex_array.h
)

target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_OPENGL_SOURCES})

if(NOX_PLATFORM_WINDOWS)
    set(NOX_OPENGL_WINDOWS_SOURCES
        ${PROJECT_SOURCE_DIR}/src/opengl/windows/windows_gl_context.cpp
        ${PROJECT_SOURCE_DIR}/src/opengl/windows/windows_gl_context.h
    )

    target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_OPENGL_WINDOWS_SOURCES})
endif()

if(NOX_PLATFORM_LINUX)
    find_package(OpenGL REQUIRED COMPONENTS EGL)
    if (NOT OpenGL_EGL_FOUND)
        message(FATAL_ERROR "EGL not found")
    endif()

    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 not found")
    endif()

    target_link_libraries(
        ${NOX_UNIT_TESTS_EXE_NAME}
        PRIVATE
        OpenGL::EGL
        X11::X11
    )

    set(NOX_OPENGL_LINUX_SOURCES
        ${PROJECT_SOURCE_DIR}/src/opengl/linux/linux_gl_context.cpp
        ${PROJECT_SOURCE_DIR}/src/opengl/linux/linux_gl_context.h
        ${PROJECT_SOURCE_DIR}/src/opengl/linux/x11_gl_context.cpp
        ${PROJECT_SOURCE_DIR}/src/opengl/linux/x11_gl_context.h
    )

    target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_OPENGL_LINUX_SOURCES})
endif()

# --------------------------------------------------
# Add tests sources
# --------------------------------------------------
set(NOX_UNIT_TESTS_OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_context_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gl_swapchain_tests.cpp
)

target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_UNIT_TESTS_OPENGL_SOURCES})
