#include "plugins/plugin.h"

namespace nox {

bool Plugin::pluginRegister() const {
    return m_pluginRegisterFunction();
}

uint8_t Plugin::pluginVersion() const {
    return m_pluginVersionFunction();
}

} // namespace nox
