#pragma once

#include <cstdint>
#include <string>

namespace NOX {

class Plugin {
  public:
    Plugin(std::string_view name) : m_filename{name} {}
    virtual ~Plugin() = default;

    [[nodiscard]] virtual bool load() = 0;

    [[nodiscard]] std::string getFilename() const { return m_filename; }
    [[nodiscard]] uint8_t getVersion() const { return m_version; }

  public:
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;
    Plugin(Plugin &&) = delete;
    Plugin &operator=(Plugin &&) = delete;

  protected:
    std::string m_filename;
    uint8_t m_version;
};

[[nodiscard]] std::string createPluginFilename(std::string_view name);

} // namespace NOX
