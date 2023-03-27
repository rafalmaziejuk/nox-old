set(NOX_UTILITIES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/string_helper.h
)

target_sources(nox PRIVATE ${NOX_UTILITIES_SOURCES})

add_subdirectory(logging)
