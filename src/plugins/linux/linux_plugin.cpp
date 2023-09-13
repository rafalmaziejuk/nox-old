#include "plugins/plugin_interface.h"
#include "plugins/linux/linux_plugin.h"

#include <dlfcn.h>

namespace NOX {

std::unique_ptr<Plugin> Plugin::create(std::string_view name, PluginFilenameCreationStrategy createFilename) {
    constexpr auto extension = "so";
    const auto filename = createFilename(name, extension);
    return std::make_unique<LinuxPlugin>(filename);
}

LinuxPlugin::~LinuxPlugin() {
    if (m_handle != nullptr) {
        auto result = dlclose(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

bool LinuxPlugin::load() {
    m_handle = dlopen(m_filename.c_str(), RTLD_LAZY);
    if (m_handle == nullptr) {
        NOX_ASSERT(true);
        return false;
    }

    PluginRegisterFunctionType registerPlugin = getFunction(pluginRegisterProcedureName);
    if (registerPlugin == nullptr) {
        NOX_ASSERT(true);
        return false;
    }
    registerPlugin();

    return true;
}

void *LinuxPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_ASSERT(procedureName.empty());

    auto *address = reinterpret_cast<void *>(dlsym(m_handle, procedureName.data()));
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
