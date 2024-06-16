#pragma once

#include <nox/surface.h>
#include <nox/vector.h>

struct GLFWwindow;

namespace nox::tests {

struct Window {
    Window();
    ~Window();

    GLFWwindow *windowHandle{nullptr};
    Vector2D<uint32_t> windowSize{800u, 600u};
    SurfaceBackendDescriptor surfaceBackendDescriptor;
    static uint8_t m_sInstanceCounter;
};

} // namespace nox::tests
