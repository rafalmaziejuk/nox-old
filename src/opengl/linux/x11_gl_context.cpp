#include "nox_assert.h"
#include "opengl/linux/x11_gl_context.h"

#define USE_X11
#include <glad/egl.h>

namespace nox {

X11GLContext::X11GLContext(const X11SurfaceBackendDescriptor &descriptor) {
    m_handleDisplayX11 = static_cast<Display *>(descriptor.displayHandle);
    m_handleWindowX11 = static_cast<Window>(descriptor.windowHandle);
}

X11GLContext::~X11GLContext() {
    [[maybe_unused]] XWindowAttributes windowAttributes; //NOLINT
    NOX_ASSERT_MSG(XGetWindowAttributes(m_handleDisplayX11, m_handleWindowX11, &windowAttributes),
                   "The window handle associated with OpenGL context is invalid");

    m_handleDisplayX11 = nullptr;
    m_handleWindowX11 = 0u;
}

bool X11GLContext::setDisplayHandle() {
    m_handleDisplay = eglGetDisplay(static_cast<EGLNativeDisplayType>(m_handleDisplayX11));
    return (m_handleDisplay != EGL_NO_DISPLAY);
}

bool X11GLContext::setSurfaceHandle(EGLConfig framebufferConfig) {
    m_handleSurface = eglCreateWindowSurface(m_handleDisplay, framebufferConfig, m_handleWindowX11, nullptr);
    return (m_handleSurface != EGL_NO_SURFACE);
}

} // namespace nox
