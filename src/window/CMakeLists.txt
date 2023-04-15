set(NOX_WINDOW_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_WINDOW_SOURCES})

if(NOX_PLATFORM_WINDOWS)
    add_subdirectory(windows)
endif()
