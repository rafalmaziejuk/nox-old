#include "asserts.h"
#include "opengl/linux/linux_gl_context.h"
#include "opengl/linux/x11_gl_context.h"

#include <glad/egl.h>
#include <glad/gl.h>

#include <array>

namespace nox {

namespace {

std::unique_ptr<LinuxGLContext> createContext(const SurfaceBackendDescriptor &descriptor) {
    const auto *x11SurfaceBackendDescriptor = std::get_if<X11SurfaceBackendDescriptor>(&descriptor);
    if (x11SurfaceBackendDescriptor != nullptr) {
        return std::make_unique<X11GLContext>(*x11SurfaceBackendDescriptor);
    }

    NOX_ASSERT(false);
    return nullptr;
}

} // namespace

std::unique_ptr<GLContext> GLContext::create(const SurfaceDescriptor &descriptor) {
    auto context = createContext(descriptor.surfaceBackendDescriptor);
    NOX_ENSURE_RETURN_NULLPTR(context != nullptr);

    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    NOX_ENSURE_RETURN_NULLPTR(surfaceAttributesDescriptor != nullptr);
    NOX_ENSURE_RETURN_NULLPTR(context->initialize(*surfaceAttributesDescriptor));

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
    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadEGL(nullptr), "Couldn't preload EGL");
    NOX_ENSURE_RETURN_FALSE_MSG(setDisplayHandle(), "Couldn't get EGL display");
    NOX_ENSURE_RETURN_FALSE_MSG(eglInitialize(m_handleDisplay, nullptr, nullptr), "Couldn't initialize EGL");
    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadEGL(m_handleDisplay), "Couldn't load EGL");

    eglBindAPI(EGL_OPENGL_API);

    std::array<int32_t, 19> framebufferConfigAttributes{EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                                        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                                                        EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
                                                        EGL_RED_SIZE, descriptor.pixelFormatDescriptor.redBits,
                                                        EGL_GREEN_SIZE, descriptor.pixelFormatDescriptor.greenBits,
                                                        EGL_BLUE_SIZE, descriptor.pixelFormatDescriptor.blueBits,
                                                        EGL_ALPHA_SIZE, descriptor.pixelFormatDescriptor.alphaBits,
                                                        EGL_DEPTH_SIZE, descriptor.pixelFormatDescriptor.depthBits,
                                                        EGL_STENCIL_SIZE, descriptor.pixelFormatDescriptor.stencilBits,
                                                        EGL_NONE};
    EGLConfig framebufferConfig{};
    EGLint framebufferConfigsCount{};
    eglChooseConfig(m_handleDisplay,
                    framebufferConfigAttributes.data(),
                    &framebufferConfig,
                    1,
                    &framebufferConfigsCount);
    NOX_ENSURE_RETURN_FALSE_MSG(framebufferConfigsCount == 1, "Couldn't choose unique framebuffer config");

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
