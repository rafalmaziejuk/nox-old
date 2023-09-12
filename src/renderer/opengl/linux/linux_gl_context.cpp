#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_debug_message_callback.inl"
#include "renderer/opengl/gl_helper.h"
#include "window/linux/linux_window.h"

#include <nox/swap_chain.h>
#include <nox/window.h>

#include <glad/glx.h>

namespace NOX {

struct GLContext::Impl {
    void setContextPixelFormatAndVisual(const PixelFormatDescriptor &descriptor) {
        NOX_ASSERT_MSG((descriptor.colorBits % 4) != 0, "Invalid colorBits value");
        NOX_ASSERT_MSG(display == nullptr, "Not connected to X11 display");

        constexpr auto defaultAlphaBits = 8;
        constexpr auto defaultColorBits = 32;
        const int32_t colorChannelBits = descriptor.colorBits / 4;

        constexpr auto attributePairsCount = 11u;
        constexpr auto attributesArraySize = attributePairsCount * 2u + 1u;

        // glx attributes for modern context
        std::array<int32_t, attributesArraySize> glxAttributes = {
            GLX_X_RENDERABLE, True,
            GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
            GLX_RENDER_TYPE, GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
            GLX_RED_SIZE, colorChannelBits,
            GLX_GREEN_SIZE, colorChannelBits,
            GLX_BLUE_SIZE, colorChannelBits,
            GLX_ALPHA_SIZE, static_cast<int32_t>((descriptor.colorBits == defaultColorBits) ? defaultAlphaBits : 0),
            GLX_DEPTH_SIZE, static_cast<int32_t>(descriptor.depthBits),
            GLX_STENCIL_SIZE, static_cast<int32_t>(descriptor.stencilBits),
            GLX_DOUBLEBUFFER, True,
            None};

        // preparing compatible frame buffer and visual for modern glx context
        int32_t fbCount;
        GLXFBConfig *fbConfigArray = glXChooseFBConfig(display, DefaultScreen(display), glxAttributes.data(), &fbCount);
        NOX_ASSERT_MSG(fbConfigArray == nullptr, "Could not create any frame buffer compatible with modern glx context");

        frameBufferConfig = getBestFrameBufferConfig(fbConfigArray, fbCount);

        visual = glXGetVisualFromFBConfig(display, frameBufferConfig);
        NOX_ASSERT_MSG(visual == nullptr, "Could not create correct visual window");
    }

    GLXFBConfig getBestFrameBufferConfig(GLXFBConfig *&fbConfig, const GLint fbCount) {
        // picking the frame buffer config/visual with the most samples per pixel
        int16_t bestFbc{-1}, bestNumSamp{-1};

        for (int8_t i = 0; i < fbCount; ++i) {
            XVisualInfo *visual = glXGetVisualFromFBConfig(display, fbConfig[i]);

            if (visual != nullptr) {
                int32_t sampBuf, samples;
                glXGetFBConfigAttrib(display, fbConfig[i], GLX_SAMPLE_BUFFERS, &sampBuf);
                glXGetFBConfigAttrib(display, fbConfig[i], GLX_SAMPLES, &samples);

                if (bestFbc < 0 || (sampBuf && samples > bestNumSamp)) {
                    bestFbc = i;
                    bestNumSamp = samples;
                }
            }
            XFree(visual);
        }
        GLXFBConfig bestFbConfig = fbConfig[bestFbc];
        XFree(fbConfig);
        fbConfig = nullptr;

        return bestFbConfig;
    }

    static constexpr auto glxMajorVersion = 1u;
    static constexpr auto glxMinorVersion = 4u;

    ::Window window{0};
    XVisualInfo *visual{nullptr};
    Display *display{nullptr};
    Screen *screen{nullptr};
    GLXFBConfig frameBufferConfig{nullptr};
    GLXContext handleRenderingContext{nullptr};
    XID screenId;
};

GLContext::GLContext() : m_impl{std::make_unique<Impl>()} {
    m_impl->display = XOpenDisplay(static_cast<const char *>(nullptr));
    NOX_ASSERT_MSG(m_impl->display == nullptr, "Unable to connect to X11 display");

    m_impl->screen = DefaultScreenOfDisplay(m_impl->display); // NOLINT
    m_impl->screenId = DefaultScreen(m_impl->display);

    {
        gladLoaderLoadGLX(m_impl->display, m_impl->screenId);
        int32_t majorGLX{}, minorGLX{};
        glXQueryVersion(m_impl->display, &majorGLX, &minorGLX);
        NOX_ASSERT_MSG((majorGLX <= m_impl->glxMajorVersion) && (minorGLX < m_impl->glxMinorVersion), "This system doesn't support GLX 1.4");

        NOX_LOG_TRACE(OPENGL, "GLX client version: {}", glXGetClientString(m_impl->display, GLX_VERSION));
        NOX_LOG_TRACE(OPENGL, "GLX client vendor: {}", glXGetClientString(m_impl->display, GLX_VENDOR));
        NOX_LOG_TRACE(OPENGL, "GLX server version: {}", glXQueryServerString(m_impl->display, m_impl->screenId, GLX_VERSION));
        NOX_LOG_TRACE(OPENGL, "GLX server vendoe: {}", glXQueryServerString(m_impl->display, m_impl->screenId, GLX_VENDOR));
    }

    // creating dummy opengl context for early gl initialization
    const PixelFormatDescriptor defaultPixelFormatDescriptor{};
    m_impl->setContextPixelFormatAndVisual(defaultPixelFormatDescriptor);

    GLXContext dummyContext = glXCreateContext(m_impl->display, m_impl->visual, NULL, GL_TRUE);
    auto result = glXMakeCurrent(m_impl->display, 0, dummyContext);
    NOX_ASSERT_MSG(!result, "Unable to make OpenGL rendering dummy context current");

    {
        gladLoaderLoadGL();
        GLint majorVersion, minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
        NOX_ASSERT_MSG((majorVersion != glMajorVersion) || (minorVersion != glMinorVersion), "This system doesn't support OpenGL 4.6");
    }
    glXDestroyContext(m_impl->display, dummyContext);

    XCloseDisplay(m_impl->display);
    m_impl->display = nullptr;
    m_impl->frameBufferConfig = nullptr;
    m_impl->screen = nullptr;
    m_impl->visual = nullptr;
}

GLContext::~GLContext() {
    if (m_impl->display != nullptr && m_impl->handleRenderingContext != nullptr) {
        glXDestroyContext(m_impl->display, m_impl->handleRenderingContext);
        NOX_LOG_TRACE(OPENGL, "Destroyed rendering context");
    }
}

void GLContext::makeCurrent() const {
    auto result = glXMakeCurrent(m_impl->display, m_impl->window, m_impl->handleRenderingContext);
    NOX_ASSERT_MSG(!result, "Unable to make OpenGL rendering context current");
    NOX_LOG_TRACE(OPENGL, "Rendering context made current");
}

void GLContext::swapBuffers() const {
    glXSwapBuffers(m_impl->display, m_impl->window);
}

void GLContext::setSwapInterval(bool value) {
    glXSwapIntervalEXT(m_impl->display, m_impl->window, static_cast<int32_t>(value));
    NOX_LOG_TRACE(OPENGL, "Swap interval set to {}", value);
}

void GLContext::createExtendedContext(const PixelFormatDescriptor &descriptor, const Window &window) {
    m_impl->display = reinterpret_cast<Display *>(window.getNativeHandle());
    NOX_ASSERT(m_impl->display == nullptr);

    auto *linuxWindow = static_cast<const LinuxWindow *>(&window);
    NOX_ASSERT(linuxWindow == nullptr);

    m_impl->window = linuxWindow->getWindow();
    m_impl->screen = DefaultScreenOfDisplay(m_impl->display); // NOLINT
    m_impl->screenId = DefaultScreen(m_impl->display);

    m_impl->setContextPixelFormatAndVisual(descriptor);
    NOX_ASSERT_MSG(m_impl->screenId != m_impl->visual->screen, "Screen id {} does not match visual screen id {}", m_impl->screenId, m_impl->visual->screen);
    NOX_ASSERT_MSG(!GLHelper::isExtensionSupported(glXQueryExtensionsString(m_impl->display, m_impl->screenId), "GLX_ARB_create_context"), "GLX_ARB_create_context extension not supported, could not create extended opengl context");

    constexpr auto attributePairsCount = 3u;
    constexpr auto attributesArraySize = attributePairsCount * 2u + 1u;

    std::array<int32_t, attributesArraySize> contextAttributes = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, glMajorVersion,
        GLX_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None};

    m_impl->handleRenderingContext = glXCreateContextAttribsARB(m_impl->display,
                                                                m_impl->frameBufferConfig,
                                                                0,
                                                                true,
                                                                contextAttributes.data());
    XSync(m_impl->display, False);
    makeCurrent();

    NOX_LOG_INFO(OPENGL, "OpenGL context info");
    NOX_LOG_INFO(OPENGL, "Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    NOX_LOG_INFO(OPENGL, "Device: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    NOX_LOG_INFO(OPENGL, "Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

    if constexpr (debugEnabled) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugMessageCallback, nullptr);
    }
}

} // namespace NOX
