#include "src/opengl/gl_context.h"
#include "src/opengl/gl_swapchain.h"

#include "tests/base/window.h"

#include <gtest/gtest.h>

using namespace nox;

struct GLSwapchainFixture : public ::testing::Test {
    SurfaceDescriptor createSurfaceDescriptor(const OpenGLSurfaceAttributesDescriptor &surfaceAttributesDescriptor) {
        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = window.surfaceBackendDescriptor;
        surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;

        return surfaceDescriptor;
    }

    tests::Window window{};
};

TEST_F(GLSwapchainFixture, GivenValidSwapchainDescriptorAndContextWhenCallingCreateSwapchainThenSwapchainIsSuccessfullyCreated) {
    const auto surfaceDescriptor = createSurfaceDescriptor(OpenGLSurfaceAttributesDescriptor{});
    auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.surfaceDescriptor = surfaceDescriptor;
    swapchainDescriptor.presentMode = OpenGLPresentMode{};
    swapchainDescriptor.size = window.windowSize;

    const auto swapchain = GLSwapchain::create(swapchainDescriptor,
                                               std::move(context));
    ASSERT_NE(nullptr, swapchain);

    EXPECT_EQ(window.windowSize, swapchain->getSize());
    EXPECT_EQ(ImageFormat::NONE, swapchain->getSurfaceFormat());
    EXPECT_EQ(0u, swapchain->getSwapchainTextures().size());
}
