#include "plugins/plugin.h"

namespace NOX {

Plugin::Plugin(std::string_view pluginName) : m_pluginName{Utilities::toLower(pluginName.data())} {}

std::string Plugin::createPhysicalPluginName(const std::string &pluginName, const std::string &extension) {
    constexpr auto prefix = "nox-";
    constexpr auto postfix = (Utilities::getBuildConfiguration() == Utilities::BuildConfiguration::DEBUG) ? "-d" : "";
    return prefix + pluginName + postfix + extension;
}

} // namespace NOX
