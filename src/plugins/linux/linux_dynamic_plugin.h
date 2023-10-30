#include "plugins/plugin.h"

namespace NOX {

class LinuxDynamicPlugin final : public Plugin {
  public:
    explicit LinuxDynamicPlugin(std::string_view filename);
    ~LinuxDynamicPlugin() override;

  private:
    void *m_handle{nullptr};
};

} // namespace NOX
