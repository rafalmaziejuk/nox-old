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

} // namespace NOX
