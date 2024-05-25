#include "src/asserts.h"
#include "src/opengl/gl_context.h"

#include "tests/fixtures/window_fixture.h"

#include <gtest/gtest.h>

using namespace nox;

struct GLContextFixture : public tests::WindowFixture,
                          public ::testing::Test {
    void SetUp() override {
        tests::WindowFixture::setUp();
    }

    void TearDown() override {
        tests::WindowFixture::tearDown();
    }
};

TEST_F(GLContextFixture, GivenValidSurfaceDescriptorWhenCallingCreateContextThenContextIsSuccessfullyCreated) {
    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};

    const auto context = GLContext::create(surfaceDescriptor);
    ASSERT_NE(nullptr, context);
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
    surfaceDescriptor.surfaceBackendDescriptor = surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = invalidSurfaceAttributesDescriptor;

    const auto context = GLContext::create(surfaceDescriptor);
    EXPECT_EQ(nullptr, context);
}
