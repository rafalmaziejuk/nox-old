#pragma once

#include "plugins/plugin.h"

namespace NOX {

class StaticPlugin final : public Plugin {
  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;
};

} // namespace NOX
