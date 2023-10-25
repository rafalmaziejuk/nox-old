#pragma once

#include "plugins/plugin_manager.h"

#define NOX_PLUGIN_API_VERSION 1

#if defined(NOX_STATIC)
#define NOX_PLUGIN_VISIBILITY extern

#define NOX_PLUGIN_REGISTER(pluginName)                                     \
    class StaticPlugin##pluginName : public NOX::Plugin {                   \
      public:                                                               \
        using Plugin::Plugin;                                               \
        ~StaticPlugin##pluginName() override = default;                     \
                                                                            \
        void setPluginVersion();                                            \
        bool load() override;                                               \
    };                                                                      \
    [[nodiscard]] std::unique_ptr<NOX::Plugin> createPlugin##pluginName() { \
        const auto filename = NOX::createPluginFilename(#pluginName);       \
        auto plugin = std::make_unique<StaticPlugin##pluginName>(filename); \
        plugin->setPluginVersion();                                         \
        return plugin;                                                      \
    }                                                                       \
    bool StaticPlugin##pluginName::load()

#define NOX_PLUGIN_VERSION(pluginName) \
    void StaticPlugin##pluginName::setPluginVersion() { m_version = NOX_PLUGIN_API_VERSION; }

#define NOX_PLUGIN_IMPORT(pluginName) \
    NOX_PLUGIN_VISIBILITY std::unique_ptr<NOX::Plugin> createPlugin##pluginName()

#define NOX_PLUGIN_LOAD(pluginName) \
    PluginManager::instance().registerPlugin(createPlugin##pluginName())
#endif

#if !defined(NOX_STATIC)
#include "plugins/dynamic_plugin.h"

#define NOX_PLUGIN_VISIBILITY extern "C"

#define NOX_PLUGIN_REGISTER(pluginName) \
    NOX_PLUGIN_VISIBILITY NOX_PLUGIN_EXPORT bool pluginRegister()

#define NOX_PLUGIN_VERSION(pluginName) \
    NOX_PLUGIN_VISIBILITY NOX_PLUGIN_EXPORT uint8_t pluginVersion() { return NOX_PLUGIN_API_VERSION; }

#define NOX_PLUGIN_IMPORT(pluginName)

#define NOX_PLUGIN_LOAD(pluginName) \
    PluginManager::instance().registerPlugin(DynamicPlugin::create(#pluginName, createPluginFilenameWithExtension))
#endif
