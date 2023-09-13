#pragma once

#include "core/singleton.h"
#include "plugins/plugin.h"

#include <memory>
#include <string_view>
#include <vector>

namespace NOX {

class Plugin;

class PluginManager : public Singleton<PluginManager> {
  public:
    [[nodiscard]] bool load(std::string_view name, PluginFilenameCreationStrategy createFilename);

  private:
    [[nodiscard]] bool isLoaded(std::string_view name) const;

  private:
    std::vector<std::unique_ptr<Plugin>> m_plugins;
};

} // namespace NOX
