#include "plugins/windows/windows_dynamic_plugin.h"

namespace NOX {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    constexpr auto extension = "dll";
    const auto filename = createPluginFilename(name, extension);
    return std::make_unique<WindowsDynamicPlugin>(filename);
}

WindowsDynamicPlugin::WindowsDynamicPlugin(std::string_view filename) {
    m_handle = LoadLibrary(filename.data());
    m_pluginRegisterFunction = reinterpret_cast<PluginRegisterFunctionType>(GetProcAddress(m_handle, pluginRegisterFunctionName));
    m_pluginVersionFunction = reinterpret_cast<PluginVersionFunctionType>(GetProcAddress(m_handle, pluginVersionFunctionName));
}

WindowsDynamicPlugin::~WindowsDynamicPlugin() {
    if (m_handle != nullptr) {
        FreeLibrary(m_handle);
        m_handle = nullptr;
    }
}

} // namespace NOX
