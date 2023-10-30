#include "plugins/plugin.h"

namespace NOX {

bool Plugin::pluginRegister() const {
    NOX_ASSERT(m_pluginRegisterFunction == nullptr);
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    NOX_ASSERT(m_pluginVersionFunction == nullptr);
    return m_pluginVersionFunction();
}

std::string createPluginFilename(std::string_view name, std::string_view extension) {
    NOX_ASSERT(name.empty() || extension.empty());

    constexpr auto prefix = (unixEnvironment ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto staticPostfix = (staticEnabled ? "-s" : "");
    constexpr auto debugPostfix = (debugEnabled ? "-d" : "");
    auto toLower = [](std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); });
        return str;
    };

    std::string result;
    result += prefix;
    result += infix;
    result += name.data();
    result += staticPostfix;
    result += debugPostfix;
    result += '.';
    result += extension;
    return toLower(result);
}

} // namespace NOX
