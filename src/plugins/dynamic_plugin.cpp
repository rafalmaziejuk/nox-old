#include "plugins/dynamic_plugin.h"

namespace NOX {

std::string DynamicPlugin::preparePluginName(std::string_view name, std::string_view extension) {
    std::string result{};
    result += (Config::gccEnabled ? "lib" : "");
    result += "nox-";
    result += name.data();
    result += (Config::debugEnabled ? "-d" : "");
    result += extension.data();
    return result;
}

} // namespace NOX
