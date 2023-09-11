#pragma once

#include <memory>
#include <string>

namespace NOX {

[[nodiscard]] std::string createPluginFilename(std::string_view name, std::string_view extension);
using PluginFilenameCreationStrategy = decltype(createPluginFilename);

struct ConvertibleProcedureAddress {
    template <typename T>
    operator T *() const {
        static_assert(std::is_function_v<T>, "Function type required");
        return reinterpret_cast<T *>(address);
    }

    void *address{nullptr};
};

class Plugin {
  public:
    Plugin(std::string_view name) : m_filename{name} {}
    virtual ~Plugin() = default;

    [[nodiscard]] static std::unique_ptr<Plugin> create(std::string_view name, PluginFilenameCreationStrategy createFilename);

    [[nodiscard]] virtual bool load() = 0;

    [[nodiscard]] std::string getFilename() const { return m_filename; }

  protected:
    virtual void *getProcedureAddress(std::string_view procedureName) const = 0;
    [[nodiscard]] ConvertibleProcedureAddress getFunction(std::string_view name) const {
        return {getProcedureAddress(name)};
    }

  public:
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

  protected:
    std::string m_filename;
};

} // namespace NOX
