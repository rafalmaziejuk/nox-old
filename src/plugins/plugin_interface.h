#pragma once

#include "plugins/plugin.h"

#define NOX_PLUGIN_API_VERSION 1

#if !defined(NOX_SHARED)
#define NOX_PLUGIN_VISIBILITY extern

#define NOX_PLUGIN_REGISTER(pluginName)                                     \
    bool pluginRegister##pluginName();                                      \
    uint8_t pluginVersion##pluginName();                                    \
                                                                            \
    class StaticPlugin##pluginName : public nox::Plugin {                   \
      public:                                                               \
        StaticPlugin##pluginName() {                                        \
            m_pluginRegisterFunction = pluginRegister##pluginName;          \
            m_pluginVersionFunction = pluginVersion##pluginName;            \
        }                                                                   \
        ~StaticPlugin##pluginName() override = default;                     \
    };                                                                      \
                                                                            \
    [[nodiscard]] std::unique_ptr<nox::Plugin> createPlugin##pluginName() { \
        return std::make_unique<StaticPlugin##pluginName>();                \
    }                                                                       \
                                                                            \
    bool pluginRegister##pluginName()

#define NOX_PLUGIN_VERSION(pluginName) \
    uint8_t pluginVersion##pluginName() { return NOX_PLUGIN_API_VERSION; }

#define NOX_PLUGIN_IMPORT(pluginName) \
    NOX_PLUGIN_VISIBILITY std::unique_ptr<nox::Plugin> createPlugin##pluginName()

#define NOX_PLUGIN_CREATE(pluginName) \
    createPlugin##pluginName()
#endif

#if defined(NOX_SHARED)
#define NOX_PLUGIN_VISIBILITY extern "C"

#define NOX_PLUGIN_REGISTER(pluginName) \
    NOX_PLUGIN_VISIBILITY NOX_PLUGIN_EXPORT bool pluginRegister()

#define NOX_PLUGIN_VERSION(pluginName) \
    NOX_PLUGIN_VISIBILITY NOX_PLUGIN_EXPORT uint8_t pluginVersion() { return NOX_PLUGIN_API_VERSION; }

#define NOX_PLUGIN_IMPORT(pluginName)

#define NOX_PLUGIN_CREATE(pluginName) \
    nox::Plugin::create(createPluginName(#pluginName))
#endif
