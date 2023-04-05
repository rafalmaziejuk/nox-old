#pragma once

#include <nox/non_copyable.h>

#include <string_view>

namespace NOX {

class Plugin : public NonCopyable {
  public:
    explicit Plugin(std::string_view pluginName);
    virtual ~Plugin() = default;

    virtual void *loadProcedure(std::string_view procedureName) const = 0;

  protected:
    std::string createPhysicalPluginName(const std::string &pluginName, const std::string &extension);
    const std::string &getPluginName() const { return m_pluginName; }

  private:
    std::string m_pluginName;
};

} // namespace NOX

#define NOX_DECLARE_PLUGIN_FUNCTION(name, returnType, argumentList) \
    typedef returnType (*name)(argumentList)
