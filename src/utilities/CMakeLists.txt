set(NOX_UTILITIES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/assertion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/config_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/string_helper.h
)

target_sources(nox PRIVATE ${NOX_UTILITIES_SOURCES})
