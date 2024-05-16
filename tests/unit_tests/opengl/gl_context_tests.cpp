#include "src/opengl/gl_context.h"

#include "tests/utilities/utilities.h"
#include "tests/utilities/window.h"

#include <gtest/gtest.h>

using namespace nox;

TEST(GLContextTests, GivenValidSurfaceDescriptorWhenCallingCreateContextThenContextIsSuccessfullyCreated) {
    test::Window window{};
    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = window.getSurfaceBackendDescriptor();
    surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);
}

TEST(GLContextTests, GivenInvalidSurfaceAttributesDescriptorWhenCallingCreateContextThenNullptrIsReturned) {
    test::DisableAssert disableAssert{};

    test::Window window{};
    OpenGLSurfaceAttributesDescriptor invalidSurfaceAttributesDescriptor;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.colorBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.depthBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.stencilBits = 64u;

    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = window.getSurfaceBackendDescriptor();
    surfaceDescriptor.surfaceAttributesDescriptor = invalidSurfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    EXPECT_EQ(nullptr, context);
}

TEST(GLContextTests, GivenSurfaceDescriptorWhenCallingContextValidateInputThenCorrectValueIsReturned) {
    // Valid input
    {
        test::Window window{};
        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = window.getSurfaceBackendDescriptor();
        surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

        const auto result = GLContext::validateInput(surfaceDescriptor);
        EXPECT_TRUE(result);
    }

    // Invalid input
    {
        auto getInvalidSurfaceBackendDescriptor = []() {
#if defined(WIN32)
            return X11SurfaceBackendDescriptor{};
#elif defined(__linux__)
            return WindowsSurfaceBackendDescriptor{};
#endif
        };

        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = getInvalidSurfaceBackendDescriptor();
        surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

        const auto result = GLContext::validateInput(surfaceDescriptor);
        EXPECT_FALSE(result);
    }
}
