#pragma once

#include <cstdint>

namespace NOX {

struct PixelFormatDescriptor {
    static constexpr uint8_t defaultColorBits = 32u;
    static constexpr uint8_t defaultDepthBits = 24u;
    static constexpr uint8_t defaultStencilBits = 8u;

    uint8_t colorBits{defaultColorBits};
    uint8_t depthBits{defaultDepthBits};
    uint8_t stencilBits{defaultStencilBits};
};

} // namespace NOX
