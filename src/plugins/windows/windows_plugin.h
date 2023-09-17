#include "plugins/plugin.h"

#include <Windows.h>

namespace NOX {

class WindowsPlugin final : public Plugin {
  public:
    using Plugin::Plugin;
    ~WindowsPlugin() override;

    bool load() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    HMODULE m_handle{nullptr};
};

} // namespace NOX
