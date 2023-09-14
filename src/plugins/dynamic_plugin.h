#pragma once

#include "plugins/plugin.h"

#include <memory>

namespace NOX {

[[nodiscard]] std::string createPluginFilenameWithExtension(std::string_view name, std::string_view extension);
using PluginFilenameCreationStrategy = decltype(createPluginFilenameWithExtension);

class DynamicPlugin : public Plugin {
  public:
    using Plugin::Plugin;

    [[nodiscard]] static std::unique_ptr<Plugin> create(std::string_view name, PluginFilenameCreationStrategy createFilename);

  protected:
    inline static constexpr auto pluginVersionProcedureName = "pluginVersion";
    using PluginVersionFunctionType = uint8_t (*)();

    inline static constexpr auto pluginRegisterProcedureName = "pluginRegister";
    using PluginRegisterFunctionType = bool (*)();

    struct ConvertibleProcedureAddress {
        template <typename T>
        operator T *() const {
            static_assert(std::is_function_v<T>, "Function type required");
            return reinterpret_cast<T *>(address);
        }

        void *address{nullptr};
    };

    virtual void *getProcedureAddress([[maybe_unused]] std::string_view procedureName) const = 0;
    [[nodiscard]] ConvertibleProcedureAddress getFunction(std::string_view name) const {
        return {getProcedureAddress(name)};
    }
};

} // namespace NOX
