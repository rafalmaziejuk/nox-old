#include "mocks/mock_plugin.h"

namespace nox {

std::unique_ptr<Plugin> Plugin::create(std::string_view name) {
    return std::make_unique<MockPlugin>();
}

MockPlugin::MockPlugin() {
    auto pluginRegister = []() -> bool {
        return expectedPluginRegisterOutput;
    };
    auto pluginVersion = []() -> uint8_t {
        return expectedPluginVersionOutput;
    };

    m_pluginRegisterFunction = pluginRegister;
    m_pluginVersionFunction = pluginVersion;
}

} // namespace nox
