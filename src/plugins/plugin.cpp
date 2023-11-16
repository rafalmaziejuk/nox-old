#include "plugins/plugin.h"

#include <algorithm>

namespace NOX {

namespace {

#if defined(NOX_DEBUG)
constexpr auto debugEnabled = true;
#else
constexpr auto debugEnabled = false;
#endif

#if (defined(__GNUC__) || defined(NOX_UNIX))
constexpr auto unixEnvironment = true;
#else
constexpr auto unixEnvironment = false;
#endif

} // namespace

bool Plugin::pluginRegister() const {
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    return m_pluginVersionFunction();
}

std::string createPluginFilename(std::string_view name, std::string_view extension) {
    constexpr auto prefix = (unixEnvironment ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto debugPostfix = (debugEnabled ? "-d" : "");
    auto toLower = [](std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); });
        return str;
    };

    std::string result;
    result += prefix;
    result += infix;
    result += name.data();
    result += debugPostfix;
    result += '.';
    result += extension;
    return toLower(result);
}

} // namespace NOX
