#pragma once

#include "plugins/plugin.h"

namespace nox {

class MockPlugin : public Plugin {
  public:
    MockPlugin();
    ~MockPlugin() override = default;

    static constexpr auto expectedPluginRegisterOutput = true;
    static constexpr auto expectedPluginVersionOutput = 1u;
};

} // namespace nox
