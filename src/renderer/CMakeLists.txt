set(NOX_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/command_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/format_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_state.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/swap_chain.cpp
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_RENDERER_SOURCES})

if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(opengl)
endif()
