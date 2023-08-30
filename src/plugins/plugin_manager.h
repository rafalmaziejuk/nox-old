#pragma once

#include "core/singleton.h"

#include <memory>
#include <unordered_map>

namespace NOX {

class Plugin;

using PluginHandle = uint8_t;

class PluginManager : public Singleton<PluginManager> {
    using PluginsContainer = std::unordered_map<PluginHandle, std::unique_ptr<Plugin>>;

  public:
    [[nodiscard]] PluginHandle registerPlugin(std::unique_ptr<Plugin> &&plugin);
    void unregisterPlugin(PluginHandle handle);

    bool isPluginRegistered(std::string_view pluginName) const;
    [[nodiscard]] PluginHandle getPluginHandle(std::string_view pluginName) const;

    [[nodiscard]] Plugin &operator[](PluginHandle handle);
    [[nodiscard]] const Plugin &operator[](PluginHandle handle) const;

  private:
    PluginsContainer m_plugins;
};

} // namespace NOX
