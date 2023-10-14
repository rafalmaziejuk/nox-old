#include "plugins/windows/windows_dynamic_plugin.h"

namespace NOX {

std::unique_ptr<Plugin> DynamicPlugin::create(std::string_view name, PluginFilenameCreationStrategy createFilename) {
    NOX_LOG_TRACE_DECLARE(PLUGINS);

    constexpr auto extension = "dll";
    const auto filename = createFilename(name, extension);
    return std::make_unique<WindowsDynamicPlugin>(filename);
}

WindowsDynamicPlugin::~WindowsDynamicPlugin() {
    NOX_LOG_TRACE_DECLARE(PLUGINS);

    if (m_handle != nullptr) {
        auto result = FreeLibrary(m_handle);
        NOX_ASSERT(result == 0);
        m_handle = nullptr;
    }
}

bool WindowsDynamicPlugin::load() {
    NOX_LOG_TRACE_DECLARE(PLUGINS);

    m_handle = LoadLibrary(m_filename.c_str());
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

void *WindowsDynamicPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_LOG_TRACE_DECLARE(PLUGINS);
    NOX_ASSERT(procedureName.empty());

    auto *address = reinterpret_cast<void *>(GetProcAddress(m_handle, procedureName.data()));
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
