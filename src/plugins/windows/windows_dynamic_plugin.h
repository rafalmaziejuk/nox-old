#include "plugins/plugin.h"

#include <Windows.h>

namespace NOX {

class WindowsDynamicPlugin final : public Plugin {
  public:
    explicit WindowsDynamicPlugin(std::string_view filename);
    ~WindowsDynamicPlugin() override;

  private:
    HMODULE m_handle{nullptr};
};

} // namespace NOX
