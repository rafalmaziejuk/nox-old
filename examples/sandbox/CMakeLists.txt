add_example_project(sandbox)

set(NOX_EXAMPLE_SANDBOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sandbox_example.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sandbox_example.h
)

target_sources(sandbox PRIVATE ${NOX_EXAMPLE_SANDBOX_SOURCES})
