#include "plugins/plugin.h"

namespace NOX {

class LinuxPlugin final : public Plugin {
  public:
    using Plugin::Plugin;
    ~LinuxPlugin() override;

    bool load() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    void *m_handle{nullptr};
};

} // namespace NOX
