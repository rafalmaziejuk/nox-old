#include "opengl/windows/windows_gl_context.h"

#include <nox/config.h>

#include <glad/wgl.h>

#include <array>

namespace NOX {

namespace {

[[nodiscard]] bool initializeExtensions(int32_t glMajorVersion, int32_t glMinorVersion) {
    constexpr auto dummyWindowName = "__NOX_DUMMY_WINDOW_CLASS__";

    WNDCLASS attributes{};
    attributes.style = CS_OWNDC;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.lpfnWndProc = DefWindowProcA;
    attributes.lpszClassName = TEXT(dummyWindowName);
    RegisterClass(&attributes);

    HWND dummyHWND = CreateWindow(attributes.lpszClassName,
                                  TEXT(dummyWindowName),
                                  0,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  nullptr,
                                  nullptr,
                                  attributes.hInstance,
                                  nullptr);
    if (dummyHWND == nullptr) {
        NOX_ASSERT_MSG(false, "Couldn't create dummy window");
        return false;
    }

    HDC dummyDC = GetDC(dummyHWND);
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor{};
    pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
    pixelFormatDescriptor.nVersion = 1u;
    pixelFormatDescriptor.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE);
    pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
    pixelFormatDescriptor.cColorBits = 32u;
    pixelFormatDescriptor.cAlphaBits = 8u;
    pixelFormatDescriptor.cDepthBits = 24u;
    pixelFormatDescriptor.cStencilBits = 8u;

    auto pixelFormat = ChoosePixelFormat(dummyDC, &pixelFormatDescriptor);
    SetPixelFormat(dummyDC, pixelFormat, &pixelFormatDescriptor);

    HGLRC dummyHGLRC = wglCreateContext(dummyDC);
    if (dummyHGLRC == nullptr) {
        NOX_ASSERT_MSG(false, "Couldn't create dummy OpenGL context");
        return false;
    }
    wglMakeCurrent(dummyDC, dummyHGLRC);

    if (!gladLoaderLoadWGL(dummyDC) || !gladLoaderLoadGL()) {
        NOX_ASSERT_MSG(false, "Couldn't load OpenGL");
        return false;
    }

    int32_t majorVersion{}, minorVersion{};
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    if ((majorVersion != glMajorVersion) || (minorVersion != glMinorVersion)) {
        NOX_ASSERT_MSG(false, "NOX currently supports only OpenGL 4.6");
        return false;
    }

    wglMakeCurrent(dummyDC, nullptr);
    wglDeleteContext(dummyHGLRC);
    ReleaseDC(dummyHWND, dummyDC);
    DestroyWindow(dummyHWND);
    UnregisterClass(TEXT(dummyWindowName), GetModuleHandle(nullptr));

    return true;
}

} // namespace

std::shared_ptr<GLContext> GLContext::create(const SurfaceDescriptor &descriptor) {
    const auto *surfaceBackendDescriptor = std::get_if<WindowsSurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
    if (surfaceBackendDescriptor == nullptr) {
        NOX_ASSERT(false);
        return nullptr;
    }

    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    if (surfaceAttributesDescriptor == nullptr) {
        NOX_ASSERT(false);
        return nullptr;
    }

    if (surfaceBackendDescriptor->windowHandle == nullptr) {
        NOX_ASSERT(false);
        return nullptr;
    }

    if (!initializeExtensions(glMajorVersion, glMinorVersion)) {
        return nullptr;
    }

    auto context = std::make_shared<WindowsGLContext>(*surfaceBackendDescriptor);
    if (!context->initialize(*surfaceAttributesDescriptor)) {
        return nullptr;
    }

    return context;
}

WindowsGLContext::WindowsGLContext(const WindowsSurfaceBackendDescriptor &descriptor) {
    m_handleWindow = static_cast<HWND>(descriptor.windowHandle);
    m_handleDeviceContext = GetDC(m_handleWindow);
    if (m_handleDeviceContext == nullptr) {
        NOX_ASSERT_MSG(false, "Couldn't get device context for given window handle");
    }
}

WindowsGLContext::~WindowsGLContext() {
    NOX_ASSERT_MSG((m_handleWindow == nullptr) && (m_handleDeviceContext == nullptr) && (m_handleRenderingContext == nullptr),
                   "OpenGL surface should be destroyed via destroy() method");
}

bool WindowsGLContext::initialize(const OpenGLSurfaceAttributesDescriptor &descriptor) {
    std::array<int32_t, 17> pixelFormatAttributes{WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                                                  WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                                                  WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                                                  WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                                                  WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                                                  WGL_COLOR_BITS_ARB, descriptor.pixelFormatDescriptor.colorBits,
                                                  WGL_DEPTH_BITS_ARB, descriptor.pixelFormatDescriptor.depthBits,
                                                  WGL_STENCIL_BITS_ARB, descriptor.pixelFormatDescriptor.stencilBits,
                                                  0};
    int32_t pixelFormat{};
    UINT formatsCount{};
    wglChoosePixelFormatARB(m_handleDeviceContext,
                            pixelFormatAttributes.data(),
                            nullptr,
                            1,
                            &pixelFormat,
                            &formatsCount);

    PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
    DescribePixelFormat(m_handleDeviceContext, pixelFormat, sizeof(pixelFormatDescriptor), &pixelFormatDescriptor);
    if (!SetPixelFormat(m_handleDeviceContext, pixelFormat, &pixelFormatDescriptor)) {
        NOX_ASSERT_MSG(false, "Couldn't set pixel format");
        return false;
    }

    constexpr std::array<int32_t, 7> contextAttributes{WGL_CONTEXT_MAJOR_VERSION_ARB, glMajorVersion,
                                                       WGL_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
                                                       WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                                       0};
    m_handleRenderingContext = wglCreateContextAttribsARB(m_handleDeviceContext, nullptr, contextAttributes.data());
    if (m_handleRenderingContext == nullptr) {
        NOX_ASSERT_MSG(false, "Couldn't create OpenGL 4.6 context");
        return false;
    }

    wglMakeCurrent(m_handleDeviceContext, m_handleRenderingContext);

    return true;
}

bool WindowsGLContext::destroy() {
    if (!IsWindow(m_handleWindow)) {
        NOX_ASSERT_MSG(false, "The window handle associated with OpenGL context is invalid");
        return false;
    }

    wglMakeCurrent(nullptr, nullptr);
    if (!wglDeleteContext(m_handleRenderingContext)) {
        NOX_ASSERT_MSG(false, "Couldn't delete OpenGL context");
        return false;
    }
    if (!ReleaseDC(m_handleWindow, m_handleDeviceContext)) {
        NOX_ASSERT_MSG(false, "Couldn't release device context associated with OpenGL context");
        return false;
    }

    m_handleWindow = nullptr;
    m_handleDeviceContext = nullptr;
    m_handleRenderingContext = nullptr;

    return true;
}

void WindowsGLContext::swapBuffers() const {
    SwapBuffers(m_handleDeviceContext);
}

void WindowsGLContext::setSwapInterval(bool value) const {
    wglSwapIntervalEXT(static_cast<int32_t>(value));
}

} // namespace NOX