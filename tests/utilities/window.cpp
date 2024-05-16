#include "tests/utilities/window.h"

#include <GLFW/glfw3.h>

#if defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace nox::test {

Window::Window() {
    constexpr auto title = "Tests window";
    m_size = {800, 600};

    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_handle = glfwCreateWindow(m_size.x(), m_size.y(), title, nullptr, nullptr);
}

Window::~Window() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
    m_handle = nullptr;
}

SurfaceBackendDescriptor Window::getSurfaceBackendDescriptor() const {
#if defined(WIN32)
    WindowsSurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.windowHandle = static_cast<void *>(glfwGetWin32Window(m_handle));
#elif defined(__linux__)
    X11SurfaceBackendDescriptor surfaceBackendDescriptor{};
    surfaceBackendDescriptor.displayHandle = static_cast<void *>(glfwGetX11Display());
    surfaceBackendDescriptor.windowHandle = static_cast<uint64_t>(glfwGetX11Window(m_handle));
#endif

    return surfaceBackendDescriptor;
}

Vector2D<uint32_t> Window::getSize() const {
    return m_size;
}

} // namespace nox::test
