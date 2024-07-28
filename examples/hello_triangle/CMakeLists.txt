add_example_project(hello_triangle)

set(NOX_EXAMPLE_HELLO_TRIANGLE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_sources(hello_triangle PRIVATE ${NOX_EXAMPLE_HELLO_TRIANGLE_SOURCES})
