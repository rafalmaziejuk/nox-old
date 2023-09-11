#pragma once

#include "plugins/plugin_manager.h"

#if defined(NOX_STATIC)
#define NOX_PLUGIN_VISIBILITY extern

#define NOX_PLUGIN_REGISTER(name) \
    bool pluginRegister##name()

#define NOX_PLUGIN_IMPORT(name) \
    NOX_PLUGIN_VISIBILITY bool pluginRegister##name()

#define NOX_PLUGIN_LOAD(name) \
    pluginRegister##name()
#endif

#if !defined(NOX_STATIC)
#define NOX_PLUGIN_VISIBILITY extern "C"

#define NOX_PLUGIN_REGISTER(name) \
    NOX_PLUGIN_VISIBILITY NOX_PLUGIN_EXPORT bool pluginRegister()

#define NOX_PLUGIN_IMPORT(name)

#define NOX_PLUGIN_LOAD(name) \
    NOX::PluginManager::instance().load(#name, NOX::createPluginFilename);
#endif

using PluginRegisterFunctionType = bool (*)();
inline static constexpr auto pluginRegisterProcedureName = "pluginRegister";
