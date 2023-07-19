#pragma once

#include "plugins/plugin.h"

#include <memory>

namespace NOX {

class DynamicPlugin final : public Plugin {
  public:
    DynamicPlugin(const DynamicPlugin &) = delete;
    DynamicPlugin &operator=(const DynamicPlugin &) = delete;

  public:
    explicit DynamicPlugin(std::string_view name);
    ~DynamicPlugin() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace NOX
