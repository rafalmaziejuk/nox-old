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
    NOX_ASSERT_IF(m_impl->handle == nullptr, "Unable to load {{{0}}} dynamic plugin", physicalPluginName);

    NOX_DECLARE_PLUGIN_FUNCTION(GetAbiCompatibilityIdentifierFunction, uint32_t, void);
    auto *getAbiCompatibilityIdentifierProcedure = loadProcedure(getAbiCompatibilityIdentifierProcedureName);
    auto *getAbiCompatibilityIdentifierFunction = reinterpret_cast<GetAbiCompatibilityIdentifierFunction>(getAbiCompatibilityIdentifierProcedure);
    const auto abiCompatibilityIdentifier = getAbiCompatibilityIdentifierFunction();
    NOX_ASSERT_IF(abiCompatibilityIdentifier != NOX_ABI_COMPATIBILITY_IDENTIFIER, "{{{0}}} dynamic plugin ABI is not compatible", physicalPluginName);

    NOX_LOG(PLUGINS, DEBUG, "Loaded {{{0}}} dynamic plugin", physicalPluginName);
}

DynamicPlugin::~DynamicPlugin() {
    auto result = FreeLibrary(m_impl->handle);
    NOX_ASSERT_IF(result == 0, "Unable to unload {{{0}}} dynamic plugin", getPluginName());

    NOX_LOG(PLUGINS, DEBUG, "Unloaded {{{0}}} dynamic plugin", getPluginName());
}

void *DynamicPlugin::loadProcedure(std::string_view procedureName) const {
    auto address = GetProcAddress(m_impl->handle, procedureName.data());
    if (address != nullptr) {
        NOX_LOG(PLUGINS, TRACE, "Loaded {{{0}}} procedure from {{{1}}} dynamic plugin", procedureName, getPluginName());
        return reinterpret_cast<void *>(address);
    }

    NOX_ASSERT_IF(true, "Unable to load {{{0}}} procedure from {{{1}}} dynamic plugin", procedureName, getPluginName());
    return nullptr;
}

} // namespace NOX
