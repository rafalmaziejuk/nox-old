#include "asserts.h"
#include "opengl/linux/linux_gl_context.h"
#include "opengl/linux/x11_gl_context.h"

#include <glad/egl.h>
#include <glad/gl.h>

#include <array>

namespace nox {

bool GLContext::validateInput(const SurfaceDescriptor &descriptor) {
    bool result = true;

    auto validateSurfaceBackendDescriptor = [&descriptor]() -> bool {
        const auto *x11SurfaceBackendDescriptor = std::get_if<X11SurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
        if (x11SurfaceBackendDescriptor != nullptr) {
            bool result = true;
            result &= (x11SurfaceBackendDescriptor->windowHandle != 0u);
            result &= (x11SurfaceBackendDescriptor->displayHandle != nullptr);
            return result;
        }

        return false;
    };
    result &= validateSurfaceBackendDescriptor();

    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    result &= (surfaceAttributesDescriptor != nullptr);
    if (result) {
        result &= (surfaceAttributesDescriptor->pixelFormatDescriptor.colorBits > 0u);
    }

    return result;
}

std::unique_ptr<GLContext> GLContext::create(const SurfaceDescriptor &descriptor) {
    NOX_ENSURE_RETURN_NULLPTR_MSG(gladLoaderLoadEGL(nullptr), "Couldn't preload EGL");

    std::unique_ptr<LinuxGLContext> context{nullptr};
    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    const auto *x11SurfaceBackendDescriptor = std::get_if<X11SurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
    if (x11SurfaceBackendDescriptor != nullptr) {
        context = std::make_unique<X11GLContext>(*x11SurfaceBackendDescriptor);
    }
    NOX_ENSURE_RETURN_NULLPTR(context != nullptr);

    if (!context->initialize(*surfaceAttributesDescriptor)) {
        return nullptr;
    }

    return context;
}

LinuxGLContext::~LinuxGLContext() {
    eglMakeCurrent(m_handleDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(m_handleDisplay, m_handleRenderingContext);
    eglDestroySurface(m_handleDisplay, m_handleSurface);
    eglTerminate(m_handleDisplay);

    m_handleDisplay = nullptr;
    m_handleSurface = nullptr;
    m_handleRenderingContext = nullptr;

    gladLoaderUnloadEGL();
    gladLoaderUnloadGL();
}

bool LinuxGLContext::initialize(const OpenGLSurfaceAttributesDescriptor &descriptor) {
    NOX_ENSURE_RETURN_FALSE_MSG(setDisplayHandle(), "Couldn't get EGL display");

    NOX_ENSURE_RETURN_FALSE_MSG(eglInitialize(m_handleDisplay, nullptr, nullptr), "Couldn't initialize EGL");
    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadEGL(m_handleDisplay), "Couldn't load EGL");

    eglBindAPI(EGL_OPENGL_API);

    std::array<int32_t, 15> framebufferConfigAttributes{EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                                        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                                                        EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
                                                        EGL_BUFFER_SIZE, descriptor.pixelFormatDescriptor.colorBits,
                                                        EGL_DEPTH_SIZE, descriptor.pixelFormatDescriptor.depthBits,
                                                        EGL_STENCIL_SIZE, descriptor.pixelFormatDescriptor.stencilBits,
                                                        EGL_ALPHA_SIZE, (descriptor.pixelFormatDescriptor.colorBits == 32u) ? 8 : 0,
                                                        EGL_NONE};
    EGLConfig framebufferConfig{};
    EGLint framebufferConfigsCount{};
    eglChooseConfig(m_handleDisplay,
                    framebufferConfigAttributes.data(),
                    &framebufferConfig,
                    1,
                    &framebufferConfigsCount);
    NOX_ENSURE_RETURN_FALSE_MSG(framebufferConfigsCount == 1, "Couldn't choose only one framebuffer config");

    constexpr std::array<int32_t, 7> contextAttributes{EGL_CONTEXT_MAJOR_VERSION, glMajorVersion,
                                                       EGL_CONTEXT_MINOR_VERSION, glMinorVersion,
                                                       EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
                                                       EGL_NONE};
    m_handleRenderingContext = eglCreateContext(m_handleDisplay, framebufferConfig, EGL_NO_CONTEXT, contextAttributes.data());
    NOX_ENSURE_RETURN_FALSE_MSG(m_handleRenderingContext != nullptr, "Couldn't create OpenGL 4.6 context");

    NOX_ENSURE_RETURN_FALSE_MSG(setSurfaceHandle(framebufferConfig), "Couldn't create EGL window surface");
    eglMakeCurrent(m_handleDisplay, m_handleSurface, m_handleSurface, m_handleRenderingContext);

    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadGL(), "Couldn't load OpenGL");

    return true;
}

void LinuxGLContext::swapBuffers() const {
    eglSwapBuffers(m_handleDisplay, m_handleSurface);
}

void LinuxGLContext::setSwapInterval(bool interval) const {
    eglSwapInterval(m_handleDisplay, static_cast<EGLint>(interval));
}

} // namespace nox
