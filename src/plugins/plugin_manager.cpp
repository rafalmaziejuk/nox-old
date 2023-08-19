#include "plugins/plugin.h"
#include "plugins/plugin_manager.h"

namespace NOX {

PluginHandle PluginManager::registerPlugin(Plugin *plugin) {
    NOX_ASSERT(plugin == nullptr);
    NOX_ASSERT(isPluginRegistered(plugin->getFilename()));

    const auto handle = static_cast<PluginHandle>(m_plugins.size() + 1);
    m_plugins.insert({handle, std::unique_ptr<Plugin>(plugin)});
    return handle;
}

void PluginManager::unregisterPlugin(PluginHandle handle) {
    NOX_ASSERT(m_plugins.find(handle) == m_plugins.end());

    m_plugins.erase(handle);
}

bool PluginManager::isPluginRegistered(std::string_view pluginName) const {
    NOX_ASSERT(pluginName.empty());

    return std::any_of(m_plugins.begin(), m_plugins.end(), [pluginName](const auto &element) {
        const auto &plugin = element.second;
        return (plugin->getFilename() == pluginName);
    });
}

PluginHandle PluginManager::getPluginHandle(std::string_view pluginName) const {
    NOX_ASSERT(pluginName.empty());

    const auto &iterator = std::find_if(m_plugins.begin(), m_plugins.end(), [pluginName](const auto &element) {
        const auto &plugin = element.second;
        return (plugin->getFilename() == pluginName);
    });
    NOX_ASSERT(iterator == m_plugins.end());

    return (iterator->first);
}

Plugin &PluginManager::operator[](PluginHandle handle) {
    NOX_ASSERT(m_plugins.find(handle) == m_plugins.end());

    auto &plugin = *m_plugins.at(handle);
    return plugin;
}

const Plugin &PluginManager::operator[](PluginHandle handle) const {
    NOX_ASSERT(m_plugins.find(handle) == m_plugins.end());

    const auto &plugin = *m_plugins.at(handle);
    return plugin;
}

} // namespace NOX
