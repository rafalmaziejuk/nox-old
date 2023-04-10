set(NOX_WINDOWS_WINDOW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_class.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_class.h
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_procedure.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/windows_window_procedure.h
)

target_sources(nox PRIVATE ${NOX_WINDOWS_WINDOW_SOURCES})
