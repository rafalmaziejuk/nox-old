#include "opengl/linux/linux_gl_context.h"
#include "opengl/linux/x11_gl_context.h"

#include <nox/config.h>

#include <glad/egl.h>
#include <glad/gl.h>

#include <array>

namespace NOX {

std::shared_ptr<GLContext> GLContext::create(const SurfaceDescriptor &descriptor) {
    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    if (surfaceAttributesDescriptor == nullptr) {
        NOX_ASSERT(false);
        return nullptr;
    }

    if (!gladLoaderLoadEGL(nullptr)) {
        NOX_ASSERT_MSG(false, "Couldn't load EGL");
        return nullptr;
    }

    std::shared_ptr<LinuxGLContext> context{nullptr};
    const auto *x11SurfaceBackendDescriptor = std::get_if<X11SurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
    if (x11SurfaceBackendDescriptor != nullptr) {
        if (x11SurfaceBackendDescriptor->windowHandle == 0u) {
            NOX_ASSERT(false);
            return nullptr;
        }

        if (x11SurfaceBackendDescriptor->displayHandle == nullptr) {
            NOX_ASSERT(false);
            return nullptr;
        }

        context = std::make_shared<X11GLContext>(*x11SurfaceBackendDescriptor);
    }

    if (!context->initialize(*surfaceAttributesDescriptor)) {
        return nullptr;
    }

    return context;
}

LinuxGLContext::~LinuxGLContext() {
    NOX_ASSERT_MSG((m_handleDisplay == nullptr) && (m_handleSurface == nullptr) && (m_handleRenderingContext == nullptr),
                   "OpenGL surface should be destroyed via destroy() method");

    gladLoaderUnloadEGL();
    gladLoaderUnloadGL();
}

bool LinuxGLContext::initialize(const OpenGLSurfaceAttributesDescriptor &descriptor) {
    if (!setDisplayHandle()) {
        return false;
    }

    if (!eglInitialize(m_handleDisplay, nullptr, nullptr)) {
        NOX_ASSERT_MSG(false, "Couldn't initialize EGL");
        return false;
    }

    if (!gladLoaderLoadEGL(m_handleDisplay)) {
        NOX_ASSERT_MSG(false, "Couldn't load EGL");
        return false;
    }

    if (!eglBindAPI(EGL_OPENGL_API)) {
        NOX_ASSERT_MSG(false, "Couldn't bind EGL OpenGL API");
        return false;
    }

    std::array<int32_t, 13> framebufferConfigAttributes{EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                                        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                                                        EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
                                                        EGL_BUFFER_SIZE, descriptor.pixelFormatDescriptor.colorBits,
                                                        EGL_DEPTH_SIZE, descriptor.pixelFormatDescriptor.depthBits,
                                                        EGL_STENCIL_SIZE, descriptor.pixelFormatDescriptor.stencilBits,
                                                        EGL_NONE};
    EGLConfig framebufferConfig{};
    EGLint framebufferConfigsCount{};
    if (!eglChooseConfig(m_handleDisplay, framebufferConfigAttributes.data(), &framebufferConfig, 1, &framebufferConfigsCount)) {
        NOX_ASSERT_MSG(false, "Couldn't choose framebuffer config");
        return false;
    }
    NOX_ASSERT_MSG(framebufferConfigsCount == 1, "Couldn't choose only one framebuffer config");

    if (!setSurfaceHandle(framebufferConfig)) {
        return false;
    }

    constexpr std::array<int32_t, 7> contextAttributes{EGL_CONTEXT_MAJOR_VERSION, glMajorVersion,
                                                       EGL_CONTEXT_MINOR_VERSION, glMinorVersion,
                                                       EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
                                                       EGL_NONE};
    m_handleRenderingContext = eglCreateContext(m_handleDisplay, framebufferConfig, EGL_NO_CONTEXT, contextAttributes.data());
    if (m_handleRenderingContext == nullptr) {
        NOX_ASSERT_MSG(false, "Couldn't create OpenGL 4.6 context");
        return false;
    }

    if (!eglMakeCurrent(m_handleDisplay, m_handleSurface, m_handleSurface, m_handleRenderingContext)) {
        NOX_ASSERT_MSG(false, "Couldn't make OpenGL context current");
        return false;
    }

    if (!gladLoaderLoadGL()) {
        NOX_ASSERT_MSG(false, "Couldn't load OpenGL");
        return false;
    }

    return true;
}

bool LinuxGLContext::destroy() {
    eglMakeCurrent(m_handleDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (!eglDestroyContext(m_handleDisplay, m_handleRenderingContext)) {
        NOX_ASSERT_MSG(false, "Couldn't destroy OpenGL context");
        return false;
    }
    if (!eglDestroySurface(m_handleDisplay, m_handleSurface)) {
        NOX_ASSERT_MSG(false, "Couldn't destroy EGL surface");
        return false;
    }
    if (!eglTerminate(m_handleDisplay)) {
        NOX_ASSERT_MSG(false, "Couldn't destroy EGL display");
        return false;
    }

    m_handleDisplay = nullptr;
    m_handleSurface = nullptr;
    m_handleRenderingContext = nullptr;

    return true;
}

void LinuxGLContext::swapBuffers() const {
    eglSwapBuffers(m_handleDisplay, m_handleSurface);
}

void LinuxGLContext::setSwapInterval(bool value) const {
    eglSwapInterval(m_handleDisplay, static_cast<EGLint>(value));
}

} // namespace NOX
