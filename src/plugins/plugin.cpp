#include "plugins/plugin.h"

#include <algorithm>

namespace nox {

namespace {

#if defined(NOX_USE_PLUGIN_PREFIX)
constexpr auto usePrefix = true;
#else
constexpr auto usePrefix = false;
#endif

#if defined(NOX_USE_PLUGIN_POSTFIX)
constexpr auto usePostfix = true;
#else
constexpr auto usePostfix = false;
#endif

} // namespace

bool Plugin::pluginRegister() const {
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    return m_pluginVersionFunction();
}

std::string createPluginFilename(std::string_view name, std::string_view extension) {
    constexpr auto prefix = (usePrefix ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto postfix = (usePostfix ? "-d" : "");
    auto toLower = [](std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
            return static_cast<char>(std::tolower(c));
        });
        return str;
    };

    std::string result;
    result += prefix;
    result += infix;
    result += name.data();
    result += postfix;
    result += '.';
    result += extension;
    return toLower(result);
}

} // namespace nox
