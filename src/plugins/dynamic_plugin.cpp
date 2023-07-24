#pragma once

#include "plugins/dynamic_plugin.h"

namespace NOX {

std::string DynamicPlugin::preparePluginName(std::string_view name, std::string_view extension) {
    std::string result{};
    result += (isGcc ? "lib" : "");
    result += "nox-";
    result += name.data();
    result += (isDebugConfiguration ? "-d" : "");
    result += extension.data();
    return result;
}

} // namespace NOX
