# --------------------------------------------------
# Find required dependencies
# --------------------------------------------------
find_package(OpenGL REQUIRED COMPONENTS EGL)
if (NOT OpenGL_EGL_FOUND)
    message(FATAL_ERROR "EGL not found")
endif()

find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 not found")
endif()

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    ${NOX_LIB_NAME}
    PRIVATE
    OpenGL::EGL
    X11::X11
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_OPENGL_LINUX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/linux_gl_context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/linux_gl_context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/x11_gl_context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/x11_gl_context.h
)

target_sources(${NOX_OPENGL_LIB_NAME} PRIVATE ${NOX_OPENGL_LINUX_SOURCES})
