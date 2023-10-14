#include "plugins/plugin_interface.h"
#include "plugins/plugin_manager.h"

namespace NOX {

bool PluginManager::registerPlugin(std::unique_ptr<Plugin> &&plugin) {
    NOX_LOG_TRACE_DECLARE(PLUGINS);
    NOX_ASSERT(plugin == nullptr);

    if (isLoaded(plugin->getFilename())) {
        return true;
    }

    if (plugin->load()) {
        if (plugin->getVersion() == NOX_PLUGIN_API_VERSION) {
            m_plugins.push_back(std::move(plugin));
            return true;
        }
    }

    return false;
}

bool PluginManager::isLoaded(std::string_view name) const {
    NOX_LOG_TRACE_DECLARE(PLUGINS);
    NOX_ASSERT(name.empty());

    return std::any_of(m_plugins.begin(), m_plugins.end(), [name](const auto &plugin) {
        return (plugin->getFilename() == name);
    });
}

} // namespace NOX
