set(NOX_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_RENDERER_SOURCES})

set(NOX_IMPL_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_plugin.h
)

target_sources(${NOX_IMPL_LIB_NAME} PRIVATE ${NOX_IMPL_RENDERER_SOURCES})

if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(opengl)
endif()
