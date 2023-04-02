#pragma once

#include "plugins/plugin.h"

#include <memory>

namespace NOX {

class DynamicPlugin final : public Plugin {
  public:
    explicit DynamicPlugin(std::string_view pluginName);
    ~DynamicPlugin() override;

    void *loadProcedure(std::string_view procedureName) const override;

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace NOX
