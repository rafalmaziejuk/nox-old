#include "plugins/plugin.h"

namespace NOX {

Plugin::Plugin(std::string_view name, std::string_view extension) {
    m_name += (isGcc ? "lib" : "");
    m_name += "nox-";
    m_name += name.data();
    m_name += (isDebugConfiguration ? "-d" : "");
    m_name += extension.data();
}

} // namespace NOX
