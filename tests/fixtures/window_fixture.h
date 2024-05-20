#pragma once

#include <nox/surface.h>
#include <nox/vector.h>

struct GLFWwindow;

namespace nox::tests {

struct WindowFixture {
    void setUp();
    void tearDown();

    GLFWwindow *windowHandle{nullptr};
    Vector2D<uint32_t> windowSize{800u, 600u};
    SurfaceBackendDescriptor surfaceBackendDescriptor;
};

} // namespace nox::tests
