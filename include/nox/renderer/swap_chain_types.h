#pragma once

#include <cstdint>

namespace NOX {

struct PixelFormatDescriptor {
    uint8_t colorBits{32u};
    uint8_t depthBits{24u};
    uint8_t stencilBits{8u};
};

} // namespace NOX
