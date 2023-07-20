#pragma once

#include "plugins/plugin.h"

namespace NOX {

class StaticPlugin final : public Plugin {
  public:
    StaticPlugin(std::string_view name);

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;
};

} // namespace NOX
