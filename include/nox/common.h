#pragma once

#include <nox/vector.h>

#include <cstdint>
#include <vector>

namespace NOX {

struct ClearMask {
    enum {
        COLOR = (1 << 0),
        DEPTH = (1 << 1),
        STENCIL = (1 << 2),
        ALL = COLOR | DEPTH | STENCIL
    };
};

enum class Format : uint8_t {
    NONE,
    R8UI,
    R16UI,
    R32UI,
    RG8UI,
    RG16UI,
    RG32UI,
    RGB8UI,
    RGB16UI,
    RGB32UI,
    RGBA8UI,
    RGBA16UI,
    RGBA32UI,
    R8I,
    R16I,
    R32I,
    RG8I,
    RG16I,
    RG32I,
    RGB8I,
    RGB16I,
    RGB32I,
    RGBA8I,
    RGBA16I,
    RGBA32I,
    R8_UNORM,
    R16_UNORM,
    R32_UNORM,
    RG8_UNORM,
    RG16_UNORM,
    RG32_UNORM,
    RGB8_UNORM,
    RGB16_UNORM,
    RGB32_UNORM,
    RGBA8_UNORM,
    RGBA16_UNORM,
    RGBA32_UNORM,
    R8_SNORM,
    R16_SNORM,
    R32_SNORM,
    RG8_SNORM,
    RG16_SNORM,
    RG32_SNORM,
    RGB8_SNORM,
    RGB16_SNORM,
    RGB32_SNORM,
    RGBA8_SNORM,
    RGBA16_SNORM,
    RGBA32_SNORM,
    R16F,
    R32F,
    R64F,
    RG16F,
    RG32F,
    RG64F,
    RGB16F,
    RGB32F,
    RGB64F,
    RGBA16F,
    RGBA32F,
    RGBA64F,
    DEPTH16I,
    DEPTH24I,
    DEPTH32I,
    DEPTH32F,
    STENCIL8UI,
    DEPTH24I_STENCIL8UI,
    DEPTH32F_STENCIL8UI,
    MAX
};

using VertexFormat = std::vector<Format>;

struct Viewport {
    Viewport() = default;

    Viewport(float width, float height) : width{width},
                                          height{height} {}

    Viewport(float x, float y, float width, float height, float nearClip = 0.0f, float farClip = 1.0f) : x{x},
                                                                                                         y{y},
                                                                                                         width{width},
                                                                                                         height{height},
                                                                                                         nearClip{nearClip},
                                                                                                         farClip{farClip} {}

    Viewport(const Vector2D<uint32_t> &size) : Viewport{static_cast<float>(size.x()), static_cast<float>(size.y())} {}

    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float nearClip{0.0f};
    float farClip{1.0f};
};

} // namespace NOX
