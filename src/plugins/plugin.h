#pragma once

#include <string_view>

namespace NOX {

class Plugin {
  public:
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;
    virtual ~Plugin() = default;

  public:
    explicit Plugin(std::string_view pluginName);
    Plugin(std::string_view pluginName, std::string_view extension);

    virtual void *loadProcedure(std::string_view procedureName) const = 0;

  protected:
    const std::string &getPluginName() const { return m_pluginName; }

  private:
    std::string m_pluginName;
};

} // namespace NOX

#define NOX_DECLARE_PLUGIN_FUNCTION(name, returnType, argumentList) \
    using name = returnType (*)(argumentList)
