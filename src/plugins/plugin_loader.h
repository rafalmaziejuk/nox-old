#pragma once

#include "core/singleton.h"

#include <memory>
#include <string_view>
#include <vector>

namespace NOX {

class Plugin;

class PluginLoader : public Singleton<PluginLoader> {
  public:
    [[nodiscard]] const Plugin *loadPlugin(std::string_view pluginName);
    void unloadPlugin(const Plugin *pluginToUnload);

  private:
    std::vector<std::unique_ptr<Plugin>> m_plugins;
};

} // namespace NOX
