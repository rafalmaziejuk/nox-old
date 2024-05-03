#include "plugins/plugin.h"

#include <algorithm>

namespace nox {

bool Plugin::pluginRegister() const {
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    return m_pluginVersionFunction();
}

template <bool usePrefix, bool usePostfix>
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

template std::string createPluginFilename<false, false>(std::string_view, std::string_view);
template std::string createPluginFilename<true, false>(std::string_view, std::string_view);
template std::string createPluginFilename<false, true>(std::string_view, std::string_view);
template std::string createPluginFilename<true, true>(std::string_view, std::string_view);

} // namespace nox
