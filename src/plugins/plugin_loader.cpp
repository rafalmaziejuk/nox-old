#include "plugins/dynamic_plugin.h"
#include "plugins/plugin_loader.h"
#include "plugins/static_plugin.h"

namespace NOX {

const Plugin *PluginLoader::loadPlugin(std::string_view pluginName) {
    if constexpr (Utilities::getLinking() == Utilities::Linking::DYNAMIC) {
        return m_plugins.emplace_back(std::make_unique<DynamicPlugin>(pluginName)).get();
    }
    if constexpr (Utilities::getLinking() == Utilities::Linking::STATIC) {
        return m_plugins.emplace_back(std::make_unique<StaticPlugin>(pluginName)).get();
    }
}

void PluginLoader::unloadPlugin(const Plugin *pluginToUnload) {
    auto pluginsIterator = std::find_if(m_plugins.begin(), m_plugins.end(), [pluginToUnload](const auto &plugin) {
        return (pluginToUnload == plugin.get());
    });
    NOX_ASSERT_IF(pluginsIterator == m_plugins.end());

    m_plugins.erase(pluginsIterator);
}

} // namespace NOX
