#include "plugins/linux/linux_dynamic_plugin.h"

#include <dlfcn.h>

namespace NOX {

std::unique_ptr<Plugin> DynamicPlugin::create(std::string_view name, PluginFilenameCreationStrategy createFilename) {
    constexpr auto extension = "so";
    const auto filename = createFilename(name, extension);
    return std::make_unique<LinuxDynamicPlugin>(filename);
}

LinuxDynamicPlugin::~LinuxDynamicPlugin() {
    if (m_handle != nullptr) {
        auto result = dlclose(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

bool LinuxDynamicPlugin::load() {
    m_handle = dlopen(m_filename.c_str(), RTLD_LAZY);
    if (m_handle == nullptr) {
        NOX_ASSERT(true);
        return false;
    }

    PluginVersionFunctionType pluginVersion = getFunction(pluginVersionProcedureName);
    if (pluginVersion == nullptr) {
        NOX_ASSERT(true);
        return false;
    }
    m_version = pluginVersion();

    PluginRegisterFunctionType pluginRegister = getFunction(pluginRegisterProcedureName);
    if (pluginRegister == nullptr) {
        NOX_ASSERT(true);
        return false;
    }
    pluginRegister();

    return true;
}

void *LinuxDynamicPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_ASSERT(procedureName.empty());

    auto *address = reinterpret_cast<void *>(dlsym(m_handle, procedureName.data()));
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
