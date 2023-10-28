#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_helper.h"
#include "window/linux/linux_window.h"

#include <nox/swap_chain.h>
#include <nox/window.h>

#include <glad/glx.h>

namespace NOX {

struct GLContext::Impl {
    void setContextPixelFormatAndVisual(const PixelFormatDescriptor &descriptor) {
        NOX_ASSERT((descriptor.colorBits % 4) != 0);
        NOX_ASSERT(display == nullptr);

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
        NOX_ASSERT(fbConfigArray == nullptr);

        frameBufferConfig = getBestFrameBufferConfig(fbConfigArray, fbCount);

        visual = glXGetVisualFromFBConfig(display, frameBufferConfig);
        NOX_ASSERT(visual == nullptr);s
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
    NOX_ASSERT(m_impl->display == nullptr);

    m_impl->screen = DefaultScreenOfDisplay(m_impl->display); // NOLINT
    m_impl->screenId = DefaultScreen(m_impl->display);

    {
        gladLoaderLoadGLX(m_impl->display, m_impl->screenId);
        int32_t majorGLX{}, minorGLX{};
        glXQueryVersion(m_impl->display, &majorGLX, &minorGLX);
        NOX_ASSERT((majorGLX <= m_impl->glxMajorVersion) && (minorGLX < m_impl->glxMinorVersion));
    }

    // creating dummy opengl context for early gl initialization
    const PixelFormatDescriptor defaultPixelFormatDescriptor{};
    m_impl->setContextPixelFormatAndVisual(defaultPixelFormatDescriptor);

    GLXContext dummyContext = glXCreateContext(m_impl->display, m_impl->visual, NULL, GL_TRUE);
    auto result = glXMakeCurrent(m_impl->display, 0, dummyContext);
    NOX_ASSERT(!result);

    {
        gladLoaderLoadGL();
        GLint majorVersion, minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
        NOX_ASSERT((majorVersion != glMajorVersion) || (minorVersion != glMinorVersion));
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
    }
}

void GLContext::makeCurrent() const {
    auto result = glXMakeCurrent(m_impl->display, m_impl->window, m_impl->handleRenderingContext);
    NOX_ASSERT(!result);
}

void GLContext::swapBuffers() const {
    glXSwapBuffers(m_impl->display, m_impl->window);
}

void GLContext::setSwapInterval(bool value) {
    glXSwapIntervalEXT(m_impl->display, m_impl->window, static_cast<int32_t>(value));
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
    NOX_ASSERT(m_impl->screenId != m_impl->visual->screen);
    NOX_ASSERT(!GLHelper::isExtensionSupported(glXQueryExtensionsString(m_impl->display, m_impl->screenId), "GLX_ARB_create_context"));

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
}

} // namespace NOX
