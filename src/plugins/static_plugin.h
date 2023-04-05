#pragma once

#include "plugins/plugin.h"

namespace NOX {

class StaticPlugin final : public Plugin {
  public:
    StaticPlugin(std::string_view pluginName);
    ~StaticPlugin() override;

    void *loadProcedure(std::string_view name) const override;
};

} // namespace NOX
