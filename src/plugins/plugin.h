#pragma once

#include <string_view>

namespace NOX {

struct ConvertibleProcedureAddress {
    template <typename T>
    operator T *() const {
        static_assert(std::is_function<T>::value);
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
    virtual ~Plugin() = default;

    ConvertibleProcedureAddress getFunction(std::string_view name) const {
        return {getProcedureAddress(name)};
    }

  protected:
    virtual void *getProcedureAddress(std::string_view procedureName) const = 0;
};

} // namespace NOX

#define NOX_DECLARE_PLUGIN_FUNCTION(name, returnType, argumentList) \
    using name = returnType (*)(argumentList)
