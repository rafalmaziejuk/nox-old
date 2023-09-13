#include "plugins/plugin.h"

namespace NOX {

class LinuxPlugin final : public Plugin {
  public:
    using Plugin::Plugin;
    ~LinuxPlugin() override;

    bool load() override;

  protected:
    void *getProcedureAddress(std::string_view procedureName) const override;

  public:
    LinuxPlugin(const LinuxPlugin &) = delete;
    LinuxPlugin &operator=(const LinuxPlugin &) = delete;
    LinuxPlugin(LinuxPlugin &&) = delete;
    LinuxPlugin &operator=(LinuxPlugin &&) = delete;

  private:
    void *m_handle{nullptr};
};

} // namespace NOX
