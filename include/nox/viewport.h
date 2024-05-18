#pragma once

#include <nox/vector.h>

#include <cstdint>

namespace nox {

struct Viewport {
    constexpr Viewport() = default;

    constexpr Viewport(float width, float height)
        : width{width},
          height{height} {}

    constexpr Viewport(float x, float y, float width, float height, float nearClip = 0.0f, float farClip = 1.0f)
        : x{x},
          y{y},
          width{width},
          height{height},
          nearClip{nearClip},
          farClip{farClip} {}

    constexpr Viewport(const Vector2D<float> &size)
        : Viewport{size.x(), size.y()} {}

    constexpr Viewport(const Vector2D<int32_t> &size)
        : Viewport{static_cast<float>(size.x()), static_cast<float>(size.y())} {}

    constexpr Viewport(const Vector2D<uint32_t> &size)
        : Viewport{static_cast<float>(size.x()), static_cast<float>(size.y())} {}

    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float nearClip{0.0f};
    float farClip{1.0f};
};

} // namespace nox
