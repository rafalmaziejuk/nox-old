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

struct PixelFormatDescriptor {
    static constexpr uint8_t defaultColorBits = 32u;
    static constexpr uint8_t defaultDepthBits = 24u;
    static constexpr uint8_t defaultStencilBits = 8u;

    uint8_t colorBits{defaultColorBits};
    uint8_t depthBits{defaultDepthBits};
    uint8_t stencilBits{defaultStencilBits};
};

} // namespace NOX
