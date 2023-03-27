set(NOX_LOGGING_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log.h
    ${CMAKE_CURRENT_SOURCE_DIR}/log_categories.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/log_categories.h
    ${CMAKE_CURRENT_SOURCE_DIR}/log_macros.h
)

target_sources(nox PRIVATE ${NOX_LOGGING_SOURCES})
