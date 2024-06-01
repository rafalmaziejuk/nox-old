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

    const auto swapchain = GLSwapchain::create(swapchainDescriptor, std::move(context));
    ASSERT_NE(nullptr, swapchain);

    EXPECT_EQ(window.windowSize, swapchain->getSize());
    EXPECT_EQ(1u, swapchain->getSwapchainTextures().size());
}

TEST_F(GLSwapchainFixture, GivenPixelFormatDescriptorWith0AlphaBitsWhenCallingCreateSwapchainThenCorrectSurfaceFormatIsReturned) {
    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    surfaceAttributesDescriptor.pixelFormatDescriptor.alphaBits = 0u;
    const auto surfaceDescriptor = createSurfaceDescriptor(surfaceAttributesDescriptor);

    auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.surfaceDescriptor = surfaceDescriptor;
    swapchainDescriptor.presentMode = OpenGLPresentMode{};
    swapchainDescriptor.size = window.windowSize;

    const auto swapchain = GLSwapchain::create(swapchainDescriptor, std::move(context));
    ASSERT_NE(nullptr, swapchain);

    constexpr auto expectedSurfaceFormat = ImageFormat::RGB8_UNORM;
    EXPECT_EQ(expectedSurfaceFormat, swapchain->getSurfaceFormat());

    const auto swapchainTextures = swapchain->getSwapchainTextures();
    EXPECT_EQ(expectedSurfaceFormat, swapchainTextures[0]->getFormat());
}

TEST_F(GLSwapchainFixture, GivenPixelFormatDescriptorWith8AlphaBitsWhenCallingCreateSwapchainThenCorrectSurfaceFormatIsReturned) {
    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    surfaceAttributesDescriptor.pixelFormatDescriptor.alphaBits = 8u;
    const auto surfaceDescriptor = createSurfaceDescriptor(surfaceAttributesDescriptor);

    auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);

    SwapchainDescriptor swapchainDescriptor{};
    swapchainDescriptor.surfaceDescriptor = surfaceDescriptor;
    swapchainDescriptor.presentMode = OpenGLPresentMode{};
    swapchainDescriptor.size = window.windowSize;

    const auto swapchain = GLSwapchain::create(swapchainDescriptor, std::move(context));
    ASSERT_NE(nullptr, swapchain);

    constexpr auto expectedSurfaceFormat = ImageFormat::RGBA8_UNORM;
    EXPECT_EQ(expectedSurfaceFormat, swapchain->getSurfaceFormat());

    const auto swapchainTextures = swapchain->getSwapchainTextures();
    EXPECT_EQ(expectedSurfaceFormat, swapchainTextures[0]->getFormat());
}
