#include "asserts.h"
#include "plugins/linux/linux_dynamic_plugin.h"

#include <dlfcn.h>

namespace nox {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    constexpr auto searchPath = "./";
    constexpr auto extension = "so";

    const auto filename = searchPath + createPluginFilename<usePrefix, usePostfix>(name, extension);
    auto plugin = std::make_unique<LinuxDynamicPlugin>();
    NOX_ENSURE_RETURN_NULLPTR(plugin->load(filename));

    return plugin;
}

LinuxDynamicPlugin::~LinuxDynamicPlugin() {
    NOX_ASSERT(dlclose(m_handle) == 0);
    m_handle = nullptr;
}

bool LinuxDynamicPlugin::load(std::string_view filename) {
    m_handle = dlopen(filename.data(), RTLD_LAZY);
    NOX_ENSURE_RETURN_FALSE_MSG(m_handle != nullptr, "Couldn't load dynamic plugin");

    m_pluginRegisterFunction = reinterpret_cast<PluginRegisterFunctionType>(dlsym(m_handle, pluginRegisterFunctionName));
    NOX_ASSERT(m_pluginRegisterFunction != nullptr);

    m_pluginVersionFunction = reinterpret_cast<PluginVersionFunctionType>(dlsym(m_handle, pluginVersionFunctionName));
    NOX_ASSERT(m_pluginRegisterFunction != nullptr);

    return true;
}

} // namespace nox
