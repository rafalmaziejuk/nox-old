#pragma once

#include <string>

namespace NOX {

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
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;

  public:
    Plugin() = default;
    explicit Plugin(std::string_view name) : m_name{name} {}
    virtual ~Plugin() = default;

    [[nodiscard]] static std::unique_ptr<Plugin> load(std::string_view name);
    [[nodiscard]] static std::string createFilename(std::string_view name, bool withPrefix, bool withPostfix);
    [[nodiscard]] static std::string createFilenameWithExtension(std::string_view name, std::string_view extension);

    [[nodiscard]] ConvertibleProcedureAddress operator[](std::string_view name) const {
        return {getProcedureAddress(name)};
    }

    std::string getFilename() { return m_name; }

  protected:
    virtual void *getProcedureAddress(std::string_view procedureName) const = 0;

  private:
    std::string m_name;
};

} // namespace NOX

#define NOX_DECLARE_PLUGIN_FUNCTION(name, returnType, ...) \
    using name = returnType (*)(__VA_ARGS__)
