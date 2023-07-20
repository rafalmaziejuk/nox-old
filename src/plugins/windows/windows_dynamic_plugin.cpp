#include "plugins/dynamic_plugin.h"

#include <Windows.h>

namespace NOX {

struct DynamicPlugin::Impl {
    HMODULE handle{nullptr};
};

DynamicPlugin::DynamicPlugin(std::string_view name) : Plugin{name, ".dll"},
                                                      m_impl{std::make_unique<Impl>()} {
    m_impl->handle = LoadLibrary(m_name.c_str());
    NOX_ASSERT(m_impl->handle == nullptr);
}

DynamicPlugin::~DynamicPlugin() {
    auto result = FreeLibrary(m_impl->handle);
    NOX_ASSERT(result == 0);
}

void *DynamicPlugin::getProcedureAddress(std::string_view procedureName) const {
    auto address = GetProcAddress(m_impl->handle, procedureName.data());
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
