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
    NOX_ASSERT(m_handle == nullptr);

    m_pluginRegisterFunction = reinterpret_cast<void *>(dlsym(m_handle, pluginRegisterFunctionName));
    NOX_ASSERT(pluginRegister == nullptr);

    m_pluginVersionFunction = reinterpret_cast<void *>(dlsym(m_handle, pluginVersionFunctionName));
    NOX_ASSERT(pluginVersion == nullptr);
}

LinuxDynamicPlugin::~LinuxDynamicPlugin() {
    if (m_handle != nullptr) {
        auto result = dlclose(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

} // namespace NOX
