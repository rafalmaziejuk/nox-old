#include "plugins/windows/windows_dynamic_plugin.h"

namespace NOX {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    constexpr auto extension = "dll";
    const auto filename = createPluginFilename(name, extension);
    return std::make_unique<WindowsDynamicPlugin>(filename);
}

WindowsDynamicPlugin::WindowsDynamicPlugin(std::string_view filename) {
    m_handle = LoadLibrary(filename.data());
    NOX_ASSERT(m_handle == nullptr);

    m_pluginRegisterFunction = reinterpret_cast<PluginRegisterFunctionType>(GetProcAddress(m_handle, pluginRegisterFunctionName));
    NOX_ASSERT(m_pluginRegisterFunction == nullptr);

    m_pluginVersionFunction = reinterpret_cast<PluginVersionFunctionType>(GetProcAddress(m_handle, pluginVersionFunctionName));
    NOX_ASSERT(m_pluginVersionFunction == nullptr);
}

WindowsDynamicPlugin::~WindowsDynamicPlugin() {
    if (m_handle != nullptr) {
        auto result = FreeLibrary(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

} // namespace NOX
