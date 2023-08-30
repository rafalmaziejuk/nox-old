#include "plugins/plugin.h"

#include <Windows.h>

namespace NOX {

class WindowsPlugin final : public Plugin {
  public:
    WindowsPlugin(const WindowsPlugin &) = delete;
    WindowsPlugin &operator=(const WindowsPlugin &) = delete;

  public:
    explicit WindowsPlugin(std::string_view name);
    ~WindowsPlugin() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  private:
    HMODULE m_handle{nullptr};
};

} // namespace NOX
