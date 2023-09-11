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

  public:
    WindowsPlugin(const WindowsPlugin &) = delete;
    WindowsPlugin &operator=(const WindowsPlugin &) = delete;
    WindowsPlugin(WindowsPlugin &&) = delete;
    WindowsPlugin &operator=(WindowsPlugin &&) = delete;

  private:
    HMODULE m_handle{nullptr};
};

} // namespace NOX
