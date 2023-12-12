#include "plugins/plugin.h"

namespace nox {

class LinuxDynamicPlugin final : public Plugin {
  public:
    ~LinuxDynamicPlugin() override;

    [[nodiscard]] bool load(std::string_view filename);

  private:
    void *m_handle{nullptr};
};

} // namespace nox
