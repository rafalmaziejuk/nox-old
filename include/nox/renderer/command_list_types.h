#pragma once

#include <nox/vector2d.h>

namespace NOX {

struct ClearFlag {
    enum {
        COLOR = (1 << 0),
        DEPTH = (1 << 1),
        STENCIL = (1 << 2),
        ALL = COLOR | DEPTH | STENCIL
    };
};

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

    Viewport(const Vector2D<uint32_t> &size) : Viewport{static_cast<float>(size.x), static_cast<float>(size.y)} {}

    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float nearClip{0.0f};
    float farClip{1.0f};
};

} // namespace NOX
