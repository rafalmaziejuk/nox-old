#include "plugins/dynamic_plugin.h"
#include "plugins/plugin_abi_compatibility_identifier.h"
#include "plugins/plugin_procedure_names.h"

#include <Windows.h>

namespace NOX {

struct DynamicPlugin::Impl {
    HMODULE handle{nullptr};
};

DynamicPlugin::DynamicPlugin(std::string_view pluginName) : Plugin{pluginName, ".dll"},
                                                            m_impl{std::make_unique<Impl>()} {
    const auto &physicalPluginName = getPluginName();
    m_impl->handle = LoadLibrary(physicalPluginName.c_str());
    NOX_ASSERT_MSG(m_impl->handle == nullptr, "Unable to load [{}] dynamic plugin", physicalPluginName);

    NOX_DECLARE_PLUGIN_FUNCTION(GetAbiCompatibilityIdentifierFunction, uint32_t, void);
    auto *getAbiCompatibilityIdentifierProcedure = loadProcedure(getAbiCompatibilityIdentifierProcedureName);
    auto *getAbiCompatibilityIdentifierFunction = reinterpret_cast<GetAbiCompatibilityIdentifierFunction>(getAbiCompatibilityIdentifierProcedure);
    const auto abiCompatibilityIdentifier = getAbiCompatibilityIdentifierFunction();
    NOX_ASSERT_MSG(abiCompatibilityIdentifier != NOX_ABI_COMPATIBILITY_IDENTIFIER, "[{}] dynamic plugin ABI is not compatible", physicalPluginName);

    NOX_LOG_INFO(PLUGINS, "Loaded [{}] dynamic plugin", physicalPluginName);
}

DynamicPlugin::~DynamicPlugin() {
    auto result = FreeLibrary(m_impl->handle);
    NOX_ASSERT_MSG(result == 0, "Unable to unload [{}] dynamic plugin", getPluginName());
    NOX_LOG_TRACE(PLUGINS, "Unloaded [{}] dynamic plugin", getPluginName());
}

void *DynamicPlugin::loadProcedure(std::string_view procedureName) const {
    auto address = GetProcAddress(m_impl->handle, procedureName.data());
    if (address != nullptr) {
        NOX_LOG_TRACE(PLUGINS, "Loaded [{}] procedure from [{}] dynamic plugin", procedureName, getPluginName());
        return reinterpret_cast<void *>(address);
    }

    NOX_ASSERT_MSG(true, "Unable to load [{}] procedure from [{}] dynamic plugin", procedureName, getPluginName());
    return nullptr;
}

} // namespace NOX
