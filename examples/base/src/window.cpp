#include "base/window.h"

#include <GLFW/glfw3.h>

#if defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace nox::base {

Window::~Window() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
    m_handle = nullptr;
}

void Window::initialize(std::string_view title, const Vector2D<int32_t> &size) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_handle = glfwCreateWindow(size.x(), size.y(), title.data(), nullptr, nullptr);
    m_size = size;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_handle);
}

void Window::processEvents() const {
    glfwPollEvents();
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

Vector2D<int32_t> Window::getSize() const {
    return m_size;
}

} // namespace nox::base
