set(GLAD_INCLUDE_DIR)
set(GLAD_SOURCES)

if(NOX_PLATFORM_WINDOWS)
    set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/windows/include)
    set(GLAD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/src/gl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/src/wgl.c
    )
endif()

add_library(glad STATIC ${GLAD_SOURCES})
target_include_directories(
    glad 
    PUBLIC
    "$<BUILD_INTERFACE:${GLAD_INCLUDE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
