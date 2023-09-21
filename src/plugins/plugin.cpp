#include "plugins/plugin.h"

namespace NOX {

std::string createPluginFilename(std::string_view name) {
    NOX_ASSERT(name.empty());

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
    return toLower(result);
}

} // namespace NOX
