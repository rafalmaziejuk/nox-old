#include "plugins/plugin.h"

namespace NOX {

std::string createPluginFilename(std::string_view name, std::string_view extension) {
    NOX_ASSERT(name.empty());

    constexpr auto prefix = (unixEnvironment ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto postfix = (debugEnabled ? "-d" : "");
    auto toLower = [](std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](uint8_t c) { return static_cast<uint8_t>(std::tolower(c)); });
        return str;
    };

    std::string result;
    result += prefix;
    result += infix;
    result += toLower(name.data());
    result += postfix;
    result += '.';
    result += extension;
    return result;
}

} // namespace NOX
