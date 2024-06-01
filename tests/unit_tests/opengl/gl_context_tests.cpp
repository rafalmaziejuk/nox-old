#include "src/opengl/gl_context.h"

#include "tests/base/window.h"

#include <glad/gl.h>
#include <gtest/gtest.h>

using namespace nox;

struct GLContextFixture : public ::testing::Test {
    tests::Window window{};
};

TEST_F(GLContextFixture, GivenValidSurfaceDescriptorWhenCallingCreateContextThenContextIsSuccessfullyCreated) {
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = window.surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

    const auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);

    constexpr GLint expectedMajorVersion = 4;
    constexpr GLint expectedMinorVersion = 6;
    GLint majorVersion = 0;
    GLint minorVersion = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    EXPECT_EQ(expectedMajorVersion, majorVersion);
    EXPECT_EQ(expectedMinorVersion, minorVersion);
}

TEST_F(GLContextFixture, GivenInvalidSurfaceBackendDescriptorWhenCallingCreateContextThenNullptrIsReturned) {
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

    const auto context = GLContext::create(surfaceDescriptor);
    EXPECT_EQ(nullptr, context);
}

TEST_F(GLContextFixture, GivenInvalidSurfaceAttributesDescriptorWhenCallingCreateContextThenNullptrIsReturned) {
    OpenGLSurfaceAttributesDescriptor invalidSurfaceAttributesDescriptor;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.redBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.depthBits = 64u;
    invalidSurfaceAttributesDescriptor.pixelFormatDescriptor.stencilBits = 64u;

    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = window.surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = invalidSurfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    EXPECT_EQ(nullptr, context);
}
