#include "plugins/dynamic_plugin.h"

namespace NOX {

class LinuxDynamicPlugin final : public DynamicPlugin {
  public:
    using DynamicPlugin::DynamicPlugin;
    ~LinuxDynamicPlugin() override;

    bool load() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    void *m_handle{nullptr};
};

} // namespace NOX
