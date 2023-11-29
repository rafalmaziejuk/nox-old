#include "opengl/linux/x11_gl_context.h"

#include <nox/config.h>

#define USE_X11
#include <glad/egl.h>

namespace NOX {

X11GLContext::X11GLContext(const X11SurfaceBackendDescriptor &descriptor) {
    m_handleDisplayX11 = static_cast<Display *>(descriptor.displayHandle);
    m_handleWindowX11 = static_cast<Window>(descriptor.windowHandle);
}

X11GLContext::~X11GLContext() {
    NOX_ASSERT((m_handleDisplayX11 == nullptr) && (m_handleWindowX11 == 0u));
}

bool X11GLContext::destroy() {
    XWindowAttributes windowAttributes;
    auto result = XGetWindowAttributes(m_handleDisplayX11, m_handleWindowX11, &windowAttributes);
    if (result == 0) {
        NOX_ASSERT_MSG(false, "The window handle associated with OpenGL context is invalid");
        return false;
    }

    m_handleDisplayX11 = nullptr;
    m_handleWindowX11 = 0u;

    return LinuxGLContext::destroy();
}

bool X11GLContext::setDisplayHandle() {
    m_handleDisplay = eglGetDisplay(static_cast<EGLNativeDisplayType>(m_handleDisplayX11));
    if (m_handleDisplay == EGL_NO_DISPLAY) {
        NOX_ASSERT_MSG(false, "Couldn't get EGL display");
        return false;
    }

    return true;
}

bool X11GLContext::setSurfaceHandle(EGLConfig framebufferConfig) {
    m_handleSurface = eglCreateWindowSurface(m_handleDisplay, framebufferConfig, m_handleWindowX11, nullptr);
    if (m_handleSurface == EGL_NO_SURFACE) {
        NOX_ASSERT_MSG(false, "Couldn't create EGL window surface");
        return false;
    }

    return true;
}

} // namespace NOX
