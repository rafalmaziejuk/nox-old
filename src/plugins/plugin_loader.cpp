#include "plugins/dynamic_plugin.h"
#include "plugins/plugin_loader.h"
#include "plugins/static_plugin.h"

namespace NOX {

const Plugin *PluginLoader::load(std::string_view name) {
    if constexpr (Config::staticEnabled) {
        return m_plugins.emplace_back(std::make_unique<StaticPlugin>()).get();
    } else {
        return m_plugins.emplace_back(std::make_unique<DynamicPlugin>(name)).get();
    }
}

void PluginLoader::unload(const Plugin *plugin) {
    auto iterator = std::find_if(m_plugins.begin(), m_plugins.end(), [plugin](const auto &element) {
        return (plugin == element.get());
    });
    NOX_ASSERT(iterator == m_plugins.end());

    m_plugins.erase(iterator);
}

} // namespace NOX
