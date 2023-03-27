add_example_project(sandbox)

set(NOX_EXAMPLE_SANDBOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_sources(sandbox PRIVATE ${NOX_EXAMPLE_SANDBOX_SOURCES})
