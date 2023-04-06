#include "plugins/plugin.h"

namespace NOX {

Plugin::Plugin(std::string_view pluginName) : m_pluginName{Utilities::toLower(pluginName.data())} {}

Plugin::Plugin(std::string_view pluginName, std::string_view extension) : m_pluginName{Utilities::toLower(pluginName.data())} {
    constexpr auto prefix = "nox-";
    constexpr auto postfix = isDebugConfiguration ? "-d" : "";
    m_pluginName = prefix + m_pluginName + postfix + extension.data();
}

} // namespace NOX
