# --------------------------------------------------
# Create glad target
# --------------------------------------------------
add_library(glad STATIC)
target_include_directories(
    glad
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
set_target_properties(
    glad
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if(NOX_PLATFORM_WINDOWS)
    target_compile_definitions(glad PRIVATE NOX_GLAD_WIN32)
endif()

if(NOX_PLATFORM_LINUX)
    target_compile_definitions(glad PRIVATE NOX_GLAD_UNIX)
endif()

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_GLAD_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

target_sources(glad PRIVATE ${NOX_GLAD_SOURCES})
