#include "plugins/dynamic_plugin.h"

namespace NOX {

std::string createPluginFilenameWithExtension(std::string_view name, std::string_view extension) {
    NOX_LOG_TRACE_DECLARE(PLUGINS);
    NOX_ASSERT(name.empty() || extension.empty());

    std::string result;
    result += createPluginFilename(name);
    result += '.';
    result += extension;
    return result;
}

} // namespace NOX
