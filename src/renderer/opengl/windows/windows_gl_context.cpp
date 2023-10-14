#include "renderer/opengl/gl_context.h"
#include "renderer/opengl/gl_debug_message_callback.inl"
#include "window/windows/windows_window_helper.h"

#include <nox/swap_chain.h>
#include <nox/window.h>

#include <glad/wgl.h>

namespace NOX {

struct GLContext::Impl {
    void setContextPixelFormat(const PixelFormatDescriptor &descriptor) const {
        NOX_LOG_TRACE_DECLARE(OPENGL);

        constexpr uint8_t defaultAlphaBits = 8u;
        constexpr uint8_t defaultColorBits = 32u;

        PIXELFORMATDESCRIPTOR pixelFormatDescriptor{};
        pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
        pixelFormatDescriptor.nVersion = 1u;
        pixelFormatDescriptor.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE);
        pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
        pixelFormatDescriptor.cColorBits = static_cast<BYTE>(descriptor.colorBits);
        pixelFormatDescriptor.cAlphaBits = static_cast<BYTE>((descriptor.colorBits == defaultColorBits) ? defaultAlphaBits : 0u);
        pixelFormatDescriptor.cDepthBits = static_cast<BYTE>(descriptor.depthBits);
        pixelFormatDescriptor.cStencilBits = static_cast<BYTE>(descriptor.stencilBits);

        auto pixelFormat = ChoosePixelFormat(handleDeviceContext, &pixelFormatDescriptor);
        NOX_ASSERT_MSG(!pixelFormat, "Unable to choose pixel format for device context");

        auto result = SetPixelFormat(handleDeviceContext, pixelFormat, &pixelFormatDescriptor);
        NOX_ASSERT_MSG(!result, "Unable to set pixel format for device context");
    }

    HDC handleDeviceContext{nullptr};
    HGLRC handleRenderingContext{nullptr};
};

GLContext::GLContext() : m_impl{std::make_unique<Impl>()} {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    constexpr auto dummyWindowName = "__NOX_DUMMY_WINDOW_CLASS__";
    auto dummyWindowProcedure = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    };

    WNDCLASS attributes{};
    attributes.style = CS_OWNDC;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.lpfnWndProc = dummyWindowProcedure;
    attributes.lpszClassName = TEXT(dummyWindowName);
    WindowsWindowHelper::registerWindowClass(attributes);

    HWND dummyWindowHandle = CreateWindow(TEXT(dummyWindowName),
                                          TEXT(dummyWindowName),
                                          WS_POPUP | WS_CLIPCHILDREN,
                                          0,
                                          0,
                                          32u,
                                          32u,
                                          nullptr,
                                          nullptr,
                                          GetModuleHandle(nullptr),
                                          nullptr);
    NOX_ASSERT_MSG(dummyWindowHandle == nullptr, "Unable to create dummy window");

    m_impl->handleDeviceContext = GetDC(dummyWindowHandle);
    NOX_ASSERT(m_impl->handleDeviceContext == nullptr);

    const PixelFormatDescriptor defaultPixelFormatDescriptor{};
    m_impl->setContextPixelFormat(defaultPixelFormatDescriptor);

    m_impl->handleRenderingContext = wglCreateContext(m_impl->handleDeviceContext);
    NOX_ASSERT(m_impl->handleRenderingContext == nullptr);
    makeCurrent();

    gladLoaderLoadWGL(m_impl->handleDeviceContext);
    gladLoaderLoadGL();

    int32_t majorVersion{}, minorVersion{};
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    NOX_ASSERT_MSG((majorVersion != glMajorVersion) || (minorVersion != glMinorVersion), "This system doesn't support OpenGL 4.6");

    wglDeleteContext(m_impl->handleRenderingContext);
    m_impl->handleRenderingContext = nullptr;
    m_impl->handleDeviceContext = nullptr;

    auto result = DestroyWindow(dummyWindowHandle);
    NOX_ASSERT_MSG(!result, "Unable to destroy dummy window");
    WindowsWindowHelper::unregisterWindowClass(dummyWindowName);
}

GLContext::~GLContext() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    if (m_impl->handleRenderingContext != nullptr) {
        auto result = wglDeleteContext(m_impl->handleRenderingContext);
        NOX_ASSERT_MSG(!result, "Unable to delete OpenGL rendering context");
    }
}

void GLContext::makeCurrent() const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto result = wglMakeCurrent(m_impl->handleDeviceContext, m_impl->handleRenderingContext);
    NOX_ASSERT_MSG(!result, "Unable to make OpenGL rendering context current");
}

void GLContext::swapBuffers() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    auto result = SwapBuffers(m_impl->handleDeviceContext);
    NOX_ASSERT_MSG(!result, "Unable to swap buffers in OpenGL swap chain");
}

void GLContext::setSwapInterval(bool value) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto result = wglSwapIntervalEXT(static_cast<int32_t>(value));
    NOX_ASSERT_MSG(!result, "Unable to set swap interval for OpenGL swap chain");
}

void GLContext::createExtendedContext(const PixelFormatDescriptor &descriptor, const Window &window) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto *windowHandle = static_cast<HWND>(window.getNativeHandle());
    m_impl->handleDeviceContext = GetDC(windowHandle);
    NOX_ASSERT(m_impl->handleDeviceContext == nullptr);

    m_impl->setContextPixelFormat(descriptor);

    constexpr auto attributePairsCount = 3u;
    constexpr auto attributesArraySize = attributePairsCount * 2u + 1u;
    std::array<int32_t, attributesArraySize> attributes{WGL_CONTEXT_MAJOR_VERSION_ARB, glMajorVersion,
                                                        WGL_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
                                                        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                                        0};
    m_impl->handleRenderingContext = wglCreateContextAttribsARB(m_impl->handleDeviceContext, nullptr, attributes.data());
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
