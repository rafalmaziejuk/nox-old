set(NOX_IMPL_PLUGINS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_interface.h
)

target_sources(${NOX_IMPL_LIB_NAME} PRIVATE ${NOX_IMPL_PLUGINS_SOURCES})

if(NOX_PLATFORM_WINDOWS)
    add_subdirectory(windows)
endif()

if(NOX_PLATFORM_LINUX)
    add_subdirectory(linux)
endif()
