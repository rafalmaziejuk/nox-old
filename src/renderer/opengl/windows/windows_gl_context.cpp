#include "renderer/opengl/gl_context.h"

#include <nox/swap_chain.h>
#include <nox/window.h>

#include <glad/wgl.h>

namespace NOX {

struct GLContext::Impl {
    void setContextPixelFormat(const PixelFormatDescriptor &descriptor) const {
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
        SetPixelFormat(handleDeviceContext, pixelFormat, &pixelFormatDescriptor);
    }

    HDC handleDeviceContext{nullptr};
    HGLRC handleRenderingContext{nullptr};
};

GLContext::GLContext() : m_impl{std::make_unique<Impl>()} {
    constexpr auto dummyWindowName = "__NOX_DUMMY_WINDOW_CLASS__";
    auto dummyWindowProcedure = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    };

    WNDCLASS attributes{};
    attributes.style = CS_OWNDC;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.lpfnWndProc = dummyWindowProcedure;
    attributes.lpszClassName = TEXT(dummyWindowName);
    RegisterClass(&attributes);

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
    m_impl->handleDeviceContext = GetDC(dummyWindowHandle);

    const PixelFormatDescriptor defaultPixelFormatDescriptor{};
    m_impl->setContextPixelFormat(defaultPixelFormatDescriptor);

    m_impl->handleRenderingContext = wglCreateContext(m_impl->handleDeviceContext);
    makeCurrent();

    gladLoaderLoadWGL(m_impl->handleDeviceContext);
    gladLoaderLoadGL();

    int32_t majorVersion{}, minorVersion{};
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    wglDeleteContext(m_impl->handleRenderingContext);
    m_impl->handleRenderingContext = nullptr;
    m_impl->handleDeviceContext = nullptr;

    DestroyWindow(dummyWindowHandle);
    UnregisterClass(TEXT(dummyWindowName), GetModuleHandle(nullptr));
}

GLContext::~GLContext() {
    if (m_impl->handleRenderingContext != nullptr) {
        wglDeleteContext(m_impl->handleRenderingContext);
    }
}

void GLContext::makeCurrent() const {
    wglMakeCurrent(m_impl->handleDeviceContext, m_impl->handleRenderingContext);
}

void GLContext::swapBuffers() const {
    SwapBuffers(m_impl->handleDeviceContext);
}

void GLContext::setSwapInterval(bool value) {
    wglSwapIntervalEXT(static_cast<int32_t>(value));
}

void GLContext::createExtendedContext(const PixelFormatDescriptor &descriptor, const Window &window) {
    auto *windowHandle = static_cast<HWND>(window.getNativeHandle());
    m_impl->handleDeviceContext = GetDC(windowHandle);
    m_impl->setContextPixelFormat(descriptor);

    constexpr auto attributePairsCount = 3u;
    constexpr auto attributesArraySize = attributePairsCount * 2u + 1u;
    std::array<int32_t, attributesArraySize> attributes{WGL_CONTEXT_MAJOR_VERSION_ARB, glMajorVersion,
                                                        WGL_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
                                                        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                                        0};
    m_impl->handleRenderingContext = wglCreateContextAttribsARB(m_impl->handleDeviceContext, nullptr, attributes.data());
    makeCurrent();
}

} // namespace NOX
