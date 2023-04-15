set(NOX_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_RENDERER_SOURCES})

if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(opengl)
endif()
