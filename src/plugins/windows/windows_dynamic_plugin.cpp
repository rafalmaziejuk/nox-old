#include "nox_assert.h"
#include "plugins/windows/windows_dynamic_plugin.h"

namespace nox {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    constexpr auto extension = "dll";
    const auto filename = createPluginFilename(name, extension);
    auto plugin = std::make_unique<WindowsDynamicPlugin>();
    NOX_ENSURE_RETURN_NULLPTR(plugin->load(filename));

    return plugin;
}

WindowsDynamicPlugin::~WindowsDynamicPlugin() {
    NOX_ASSERT(FreeLibrary(m_handle));
    m_handle = nullptr;
}

bool WindowsDynamicPlugin::load(std::string_view filename) {
    m_handle = LoadLibrary(filename.data());
    NOX_ENSURE_RETURN_FALSE_MSG(m_handle != nullptr, "Couldn't load dynamic plugin");

    m_pluginRegisterFunction = reinterpret_cast<PluginRegisterFunctionType>(GetProcAddress(m_handle, pluginRegisterFunctionName));
    NOX_ASSERT(m_pluginRegisterFunction != nullptr);

    m_pluginVersionFunction = reinterpret_cast<PluginVersionFunctionType>(GetProcAddress(m_handle, pluginVersionFunctionName));
    NOX_ASSERT(m_pluginRegisterFunction != nullptr);

    return true;
}

} // namespace nox
