#pragma once

#include <nox/surface.h>
#include <nox/vector.h>

#include <gtest/gtest.h>

struct GLFWwindow;

namespace nox::tests {

struct RendererFixture : public ::testing::Test {
    void SetUp() override;
    void TearDown() override;

    GLFWwindow *windowHandle{nullptr};
    Vector2D<uint32_t> windowSize{800u, 600u};
    SurfaceBackendDescriptor surfaceBackendDescriptor;
};

} // namespace nox::tests
