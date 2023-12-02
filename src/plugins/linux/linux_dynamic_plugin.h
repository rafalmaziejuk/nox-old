#include "plugins/plugin.h"

namespace NOX {

class LinuxDynamicPlugin final : public Plugin {
  public:
    ~LinuxDynamicPlugin() override;

    [[nodiscard]] bool load(std::string_view filename);

  private:
    void *m_handle{nullptr};
};

} // namespace NOX
