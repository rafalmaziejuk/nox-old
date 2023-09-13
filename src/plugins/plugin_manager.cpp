#include "plugins/plugin_manager.h"

namespace NOX {

bool PluginManager::load(std::string_view name, PluginFilenameCreationStrategy createFilename) {
    NOX_ASSERT(name.empty());

    auto plugin = Plugin::create(name, createFilename);
    if (isLoaded(plugin->getFilename())) {
        return true;
    }

    if (plugin->load()) {
        m_plugins.push_back(std::move(plugin));
        return true;
    }

    return false;

}
bool PluginManager::isLoaded(std::string_view name) const {
    NOX_ASSERT(name.empty());

    return std::any_of(m_plugins.begin(), m_plugins.end(), [name](const auto &plugin) {
        return (plugin->getFilename() == name);
    });
}

} // namespace NOX
