set(NOX_PLUGINS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_loader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/static_plugin.h
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_PLUGINS_SOURCES})

if(NOX_PLATFORM_WINDOWS)
    add_subdirectory(windows)
endif()
