#include "core/utilities.h"
#include "plugins/plugin.h"

namespace NOX {

std::string createPluginFilename(std::string_view name) {
    NOX_LOG_TRACE_DECLARE(PLUGINS);
    NOX_ASSERT(name.empty());

    constexpr auto prefix = (unixEnvironment ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto staticPostfix = (staticEnabled ? "-s" : "");
    constexpr auto debugPostfix = (debugEnabled ? "-d" : "");

    std::string result;
    result += prefix;
    result += infix;
    result += name.data();
    result += staticPostfix;
    result += debugPostfix;
    return Utilities::toLower(result);
}

} // namespace NOX
