#pragma once

#include "core/singleton.h"
#include "plugins/plugin.h"

#include <memory>
#include <vector>

namespace NOX {

class Plugin;

class PluginManager : public Singleton<PluginManager> {
  public:
    [[nodiscard]] bool registerPlugin(std::unique_ptr<Plugin> &&plugin);

  private:
    [[nodiscard]] bool isLoaded(std::string_view name) const;

  private:
    std::vector<std::unique_ptr<Plugin>> m_plugins;
};

} // namespace NOX
