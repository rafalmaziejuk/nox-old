#include "tests/base/opengl/gl_test_environment.h"

namespace nox::tests {

const GLTestEnvironment &GLTestEnvironment::instance() {
    static GLTestEnvironment instance{};
    return instance;
}

GLTestEnvironment::GLTestEnvironment() {
    window = std::make_unique<tests::Window>();

    SurfaceDescriptor surfaceDescriptor{};
    surfaceDescriptor.surfaceBackendDescriptor = window->surfaceBackendDescriptor;
    surfaceDescriptor.surfaceAttributesDescriptor = OpenGLSurfaceAttributesDescriptor{};
    context = GLContext::create(surfaceDescriptor);
}

} // namespace nox::tests
