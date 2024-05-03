#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace nox {

class Plugin {
  public:
    [[nodiscard]] static std::unique_ptr<Plugin> create(std::string_view name);

    [[nodiscard]] bool pluginRegister() const;
    [[nodiscard]] uint8_t pluginVersion() const;

  public:
    Plugin() = default;
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;
    Plugin(Plugin &&) = delete;
    Plugin &operator=(Plugin &&) = delete;
    virtual ~Plugin() = default;

  protected:
    static constexpr auto pluginRegisterFunctionName = "pluginRegister";
    static constexpr auto pluginVersionFunctionName = "pluginVersion";
    using PluginRegisterFunctionType = bool (*)();
    using PluginVersionFunctionType = uint8_t (*)();

    PluginRegisterFunctionType m_pluginRegisterFunction{nullptr};
    PluginVersionFunctionType m_pluginVersionFunction{nullptr};
};

#if defined(NOX_USE_PLUGIN_PREFIX)
inline constexpr auto usePrefix = true;
#else
inline constexpr auto usePrefix = false;
#endif

#if defined(NOX_USE_PLUGIN_POSTFIX)
inline constexpr auto usePostfix = true;
#else
inline constexpr auto usePostfix = false;
#endif

template <bool usePrefix, bool usePostfix>
[[nodiscard]] std::string createPluginFilename(std::string_view name, std::string_view extension);

} // namespace nox
