#include "plugins/linux/linux_dynamic_plugin.h"

#include <dlfcn.h>

namespace NOX {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    constexpr auto extension = "so";
    const auto filename = createPluginFilename(name, extension);
    return std::make_unique<LinuxDynamicPlugin>(filename);
}

LinuxDynamicPlugin::LinuxDynamicPlugin(std::string_view filename) {
    m_handle = dlopen(filename.data(), RTLD_LAZY);
    m_pluginRegisterFunction = reinterpret_cast<PluginRegisterFunctionType>(dlsym(m_handle, pluginRegisterFunctionName));
    m_pluginVersionFunction = reinterpret_cast<PluginVersionFunctionType>(dlsym(m_handle, pluginVersionFunctionName));
}

LinuxDynamicPlugin::~LinuxDynamicPlugin() {
    if (m_handle != nullptr) {
        dlclose(m_handle);
        m_handle = nullptr;
    }
}

} // namespace NOX
