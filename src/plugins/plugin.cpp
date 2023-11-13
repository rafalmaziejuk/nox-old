#include "plugins/plugin.h"

#include <algorithm>

#if defined(NOX_STATIC)
static constexpr auto staticEnabled = true;
#else
static constexpr auto staticEnabled = false;
#endif

#if defined(NOX_DEBUG)
static constexpr auto debugEnabled = true;
#else
static constexpr auto debugEnabled = false;
#endif

#if (defined(__GNUC__) || defined(NOX_UNIX))
static constexpr auto unixEnvironment = true;
#else
static constexpr auto unixEnvironment = false;
#endif

namespace NOX {

bool Plugin::pluginRegister() const {
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    return m_pluginVersionFunction();
}

std::string createPluginFilename(std::string_view name, std::string_view extension) {
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
