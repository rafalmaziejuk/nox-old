#include "tests/base/window.h"

#include <GLFW/glfw3.h>

#if defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace nox::tests {

namespace {

SurfaceBackendDescriptor createSurfaceBackendDescriptor(GLFWwindow *windowHandle) {
#if defined(WIN32)
    WindowsSurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.windowHandle = static_cast<void *>(glfwGetWin32Window(windowHandle));
#elif defined(__linux__)
    X11SurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.displayHandle = static_cast<void *>(glfwGetX11Display());
    surfaceBackendDescriptor.windowHandle = static_cast<uint64_t>(glfwGetX11Window(windowHandle));
#endif

    return surfaceBackendDescriptor;
}

} // namespace

Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    windowHandle = glfwCreateWindow(windowSize.x(), windowSize.y(), "", nullptr, nullptr);
    surfaceBackendDescriptor = createSurfaceBackendDescriptor(windowHandle);
}

Window::~Window() {
    glfwDestroyWindow(windowHandle);
    glfwTerminate();
    windowHandle = nullptr;
}

} // namespace nox::tests
