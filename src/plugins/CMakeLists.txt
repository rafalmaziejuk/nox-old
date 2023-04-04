set(NOX_PLUGINS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM_DIR_SUFFIX}dynamic_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_abi_compatibility_identifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_loader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin_procedure_names.h
    ${CMAKE_CURRENT_SOURCE_DIR}/static_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/static_plugin.h
)

target_sources(nox PRIVATE ${NOX_PLUGINS_SOURCES})
