#include "plugins/windows/windows_plugin.h"

namespace NOX {

Plugin *Plugin::load(std::string_view name) {
    return new WindowsPlugin(name);
}

WindowsPlugin::WindowsPlugin(std::string_view name) : Plugin{name} {
    NOX_ASSERT(name.empty());

    constexpr auto pluginExtension = "dll";
    const auto filename = Plugin::createFilenameWithExtension(name, pluginExtension);
    m_handle = LoadLibrary(filename.c_str());
    NOX_ASSERT(m_handle == nullptr);
}

WindowsPlugin::~WindowsPlugin() {
    auto result = FreeLibrary(m_handle);
    NOX_ASSERT(result == 0);
}

void *WindowsPlugin::getProcedureAddress(std::string_view procedureName) const {
    NOX_ASSERT(procedureName.empty());

    auto *address = reinterpret_cast<void *>(GetProcAddress(m_handle, procedureName.data()));
    NOX_ASSERT(address == nullptr);
    return address;
}

} // namespace NOX
