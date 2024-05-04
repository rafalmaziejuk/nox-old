#pragma once

#include <nox/export.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>

namespace nox {

class NOX_EXPORT Plugin {
  public:
    [[nodiscard]] static std::unique_ptr<Plugin> create(const std::string &name);

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
inline constexpr auto usePluginPrefix = true;
#else
inline constexpr auto usePluginPrefix = false;
#endif

#if defined(NOX_USE_PLUGIN_POSTFIX)
inline constexpr auto usePluginPostfix = true;
#else
inline constexpr auto usePluginPostfix = false;
#endif

template <bool usePrefix = usePluginPrefix, bool usePostfix = usePluginPostfix>
[[nodiscard]] inline std::string createPluginName(std::string_view name) {
    constexpr auto prefix = (usePrefix ? "lib" : "");
    constexpr auto infix = "nox-";
    constexpr auto postfix = (usePostfix ? "-d" : "");
    auto toLower = [](std::string str) {
        std::transform(str.begin(), str.end(), str.begin(), [](auto c) {
            return static_cast<char>(std::tolower(c));
        });
        return str;
    };

    std::string result;
    result += prefix;
    result += infix;
    result += name.data();
    result += postfix;
    return toLower(result);
}

} // namespace nox
