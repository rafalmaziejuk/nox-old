#include "plugins/dynamic_plugin.h"

#include <Windows.h>

namespace NOX {

class WindowsDynamicPlugin final : public DynamicPlugin {
  public:
    using DynamicPlugin::DynamicPlugin;
    ~WindowsDynamicPlugin() override;

    bool load() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    HMODULE m_handle{nullptr};
};

} // namespace NOX
