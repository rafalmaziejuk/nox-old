set(NOX_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_factory_registry.h
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_RENDERER_SOURCES})

add_subdirectory(visitors)

if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(opengl)
endif()
