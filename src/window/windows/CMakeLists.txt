set(NOX_WINDOW_WINDOWS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_procedure.inl
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_WINDOW_WINDOWS_SOURCES})

set(NOX_IMPL_WINDOW_WINDOWS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_helper.h
)

target_sources(${NOX_IMPL_LIB_NAME} PRIVATE ${NOX_IMPL_WINDOW_WINDOWS_SOURCES})
