#include "src/opengl/gl_context.h"

#include "tests/fixtures/assert_disabled_fixture.h"
#include "tests/fixtures/renderer_fixture.h"

#include <gtest/gtest.h>

using namespace nox;

using GLContextFixture = tests::RendererFixture;

TEST_F(GLContextFixture, GivenValidSurfaceDescriptorWhenCallingCreateContextThenContextIsSuccessfullyCreated) {
    OpenGLSurfaceAttributesDescriptor surfaceAttributesDescriptor{};
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = surfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);
}

TEST_F(GLContextFixture, GivenSurfaceDescriptorWhenCallingContextValidateInputThenCorrectValueIsReturned) {
    // Valid input
    {
        SurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
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

struct GLContextAssertDisabledFixture : public GLContextFixture,
                                        public tests::AssertDisabledFixture {};

TEST_F(GLContextAssertDisabledFixture, GivenInvalidSurfaceAttributesDescriptorWhenCallingCreateContextThenNullptrIsReturned) {
    OpenGLSurfaceAttributesDescriptor invalidSurfaceAttributesDescriptor;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.colorBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.depthBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.stencilBits = 64u;

    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = invalidSurfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    EXPECT_EQ(nullptr, context);
}
