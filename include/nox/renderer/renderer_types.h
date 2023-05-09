#pragma once

#include <cstdint>
#include <variant>

namespace NOX {

enum class RendererAPI : uint8_t {
    OPENGL = 0u,
    MAX
};

struct OpenGLRendererConfig {
    static constexpr auto index = static_cast<size_t>(RendererAPI::OPENGL);
    int32_t majorVersion;
    int32_t minorVersion;
};

class RendererConfig {
  public:
    RendererConfig() = default;

    template <typename... Args>
    RendererConfig(Args &&...args) : config{std::forward<Args>(args)...} {}

    template <typename T>
    T get() const {
        if (config.index() == T::index) {
            return std::get<T::index>(config);
        }
        return T{};
    }

  private:
    std::variant<OpenGLRendererConfig> config;
};

} // namespace NOX
