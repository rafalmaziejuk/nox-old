#include "plugins/plugin_interface.h"
#include "plugins/windows/windows_plugin.h"

namespace NOX {

std::unique_ptr<Plugin> Plugin::create(std::string_view name, PluginFilenameCreationStrategy createFilename) {
    constexpr auto extension = "dll";
    const auto filename = createFilename(name, extension);
    return std::make_unique<WindowsPlugin>(filename);
}

WindowsPlugin::~WindowsPlugin() {
    if (m_handle != nullptr) {
        auto result = FreeLibrary(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

bool WindowsPlugin::load() {
    m_handle = LoadLibrary(m_filename.c_str());
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

void *WindowsPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_ASSERT(procedureName.empty());

    auto *address = reinterpret_cast<void *>(GetProcAddress(m_handle, procedureName.data()));
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
