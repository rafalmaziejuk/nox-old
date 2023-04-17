#include "renderer/opengl/gl_context.h"
#include "utilities/windows/windows_helper.h"

#include <glad/wgl.h>

namespace NOX {

struct GLContext::Impl {
    void setContextPixelFormat(const PixelFormatDescriptor &descriptor) const {
        constexpr uint8_t defaultAlphaBits = 8u;

        PIXELFORMATDESCRIPTOR pixelFormatDescriptor{};
        pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
        pixelFormatDescriptor.nVersion = 1u;
        pixelFormatDescriptor.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE);
        pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
        pixelFormatDescriptor.cColorBits = static_cast<BYTE>(descriptor.colorBits);
        pixelFormatDescriptor.cAlphaBits = static_cast<BYTE>((descriptor.colorBits == PixelFormatDescriptor::defaultColorBits) ? defaultAlphaBits : 0u);
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

GLContext::GLContext(const OpenGLRendererConfig &config) : m_impl{std::make_unique<Impl>()},
                                                           m_config{config} {
    constexpr auto dummyWindowName = "__NOX_DUMMY_WINDOW_CLASS__";
    auto dummyWindowProcedure = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    };

    WNDCLASS attributes{};
    attributes.style = CS_OWNDC;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.lpfnWndProc = dummyWindowProcedure;
    attributes.lpszClassName = TEXT(dummyWindowName);
    WindowsHelper::registerWindowClass(attributes);

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
    NOX_ASSERT(!wglCreateContextAttribsARB);
    NOX_ASSERT(!wglSwapIntervalEXT);
    NOX_ASSERT(!GLAD_WGL_EXT_swap_control);

    gladLoaderLoadGL();
    NOX_ASSERT(!GLAD_GL_EXT_direct_state_access);

    int32_t majorVersion{}, minorVersion{};
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    const auto queriedVersion = majorVersion * 10 + minorVersion;
    const auto configVersion = m_config.majorVersion * 10 + m_config.minorVersion;
    NOX_ASSERT_MSG(configVersion > queriedVersion, "This system supports OpenGL up to version [{}.{}]", majorVersion, minorVersion);

    if (configVersion == 0) {
        m_config.majorVersion = majorVersion;
        m_config.minorVersion = minorVersion;
    }

    auto result = DestroyWindow(dummyWindowHandle);
    NOX_ASSERT_MSG(!result, "Unable to destroy dummy window");
    WindowsHelper::unregisterWindowClass(dummyWindowName);
}

GLContext::~GLContext() {
    auto result = wglDeleteContext(m_impl->handleRenderingContext);
    NOX_ASSERT_MSG(!result, "Unable to delete OpenGL rendering context");
    NOX_LOG_TRACE(OPENGL, "Destroyed rendering context");
}

void GLContext::makeCurrent() const {
    auto result = wglMakeCurrent(m_impl->handleDeviceContext, m_impl->handleRenderingContext);
    NOX_ASSERT_MSG(!result, "Unable to make OpenGL rendering context current");
    NOX_LOG_TRACE(OPENGL, "Rendering context made current");
}

void GLContext::swapBuffers() const {
    auto result = SwapBuffers(m_impl->handleDeviceContext);
    NOX_ASSERT_MSG(!result, "Unable to swap buffers in OpenGL swap chain");
}

void GLContext::setSwapInterval(bool value) {
    auto result = wglSwapIntervalEXT(static_cast<int32_t>(value));
    NOX_ASSERT_MSG(!result, "Unable to set swap interval for OpenGL swap chain");
    NOX_LOG_TRACE(OPENGL, "Swap interval set to {}", value);
}

} // namespace NOX
