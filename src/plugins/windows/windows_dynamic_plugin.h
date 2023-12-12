#include "plugins/plugin.h"

#include <windows.h>

namespace nox {

class WindowsDynamicPlugin final : public Plugin {
  public:
    ~WindowsDynamicPlugin() override;

    [[nodiscard]] bool load(std::string_view filename);

  private:
    HMODULE m_handle{nullptr};
};

} // namespace nox
