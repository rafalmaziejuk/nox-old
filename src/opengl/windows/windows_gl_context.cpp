#include "asserts.h"
#include "opengl/windows/windows_gl_context.h"

#include <glad/wgl.h>

#include <array>

namespace nox {

namespace {

[[nodiscard]] bool loadOpenGL() {
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
    NOX_ENSURE_RETURN_FALSE_MSG(dummyHWND != nullptr, "Couldn't create dummy window");

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
    NOX_ENSURE_RETURN_FALSE_MSG(dummyHGLRC != nullptr, "Couldn't create dummy OpenGL context");

    wglMakeCurrent(dummyDC, dummyHGLRC);

    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadWGL(dummyDC), "Couldn't load WGL");
    NOX_ENSURE_RETURN_FALSE_MSG(gladLoaderLoadGL(), "Couldn't load OpenGL");

    wglMakeCurrent(dummyDC, nullptr);
    wglDeleteContext(dummyHGLRC);
    ReleaseDC(dummyHWND, dummyDC);
    DestroyWindow(dummyHWND);
    UnregisterClass(TEXT(dummyWindowName), GetModuleHandle(nullptr));

    return true;
}

} // namespace

bool GLContext::validateInput(const SurfaceDescriptor &descriptor) {
    bool result = true;

    const auto *surfaceBackendDescriptor = std::get_if<WindowsSurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
    result &= (surfaceBackendDescriptor != nullptr);
    if (result) {
        result &= (surfaceBackendDescriptor->windowHandle != nullptr);
    }

    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    result &= (surfaceAttributesDescriptor != nullptr);
    if (result) {
        result &= (surfaceAttributesDescriptor->pixelFormatDescriptor.colorBits > 0u);
    }

    return result;
}

std::unique_ptr<GLContext> GLContext::create(const SurfaceDescriptor &descriptor) {
    if (!loadOpenGL()) {
        return nullptr;
    }

    const auto *surfaceBackendDescriptor = std::get_if<WindowsSurfaceBackendDescriptor>(&descriptor.surfaceBackendDescriptor);
    const auto *surfaceAttributesDescriptor = std::get_if<OpenGLSurfaceAttributesDescriptor>(&descriptor.surfaceAttributesDescriptor);
    auto context = std::make_unique<WindowsGLContext>(*surfaceBackendDescriptor);
    if (!context->initialize(*surfaceAttributesDescriptor)) {
        return nullptr;
    }

    return context;
}

WindowsGLContext::WindowsGLContext(const WindowsSurfaceBackendDescriptor &descriptor) {
    m_handleWindow = static_cast<HWND>(descriptor.windowHandle);
    m_handleDeviceContext = GetDC(m_handleWindow);
    NOX_ASSERT_MSG(m_handleDeviceContext != nullptr, "Couldn't get device context for given window handle");
}

WindowsGLContext::~WindowsGLContext() {
    NOX_ASSERT_MSG(IsWindow(m_handleWindow), "The window handle associated with OpenGL context is invalid");

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(m_handleRenderingContext);
    ReleaseDC(m_handleWindow, m_handleDeviceContext);

    m_handleWindow = nullptr;
    m_handleDeviceContext = nullptr;
    m_handleRenderingContext = nullptr;

    gladLoaderUnloadGL();
}

bool WindowsGLContext::initialize(const OpenGLSurfaceAttributesDescriptor &descriptor) {
    std::array<int32_t, 19> pixelFormatAttributes{WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                                                  WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                                                  WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                                                  WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                                                  WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                                                  WGL_COLOR_BITS_ARB, descriptor.pixelFormatDescriptor.colorBits,
                                                  WGL_DEPTH_BITS_ARB, descriptor.pixelFormatDescriptor.depthBits,
                                                  WGL_STENCIL_BITS_ARB, descriptor.pixelFormatDescriptor.stencilBits,
                                                  WGL_ALPHA_BITS_ARB, (descriptor.pixelFormatDescriptor.colorBits == 32u) ? 8 : 0,
                                                  0};
    int32_t pixelFormat{};
    UINT formatsCount{};
    wglChoosePixelFormatARB(m_handleDeviceContext,
                            pixelFormatAttributes.data(),
                            nullptr,
                            1,
                            &pixelFormat,
                            &formatsCount);
    NOX_ENSURE_RETURN_FALSE_MSG(formatsCount == 1u, "Couldn't choose only one pixel format");

    PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
    DescribePixelFormat(m_handleDeviceContext, pixelFormat, sizeof(pixelFormatDescriptor), &pixelFormatDescriptor);
    SetPixelFormat(m_handleDeviceContext, pixelFormat, &pixelFormatDescriptor);

    constexpr std::array<int32_t, 7> contextAttributes{WGL_CONTEXT_MAJOR_VERSION_ARB, glMajorVersion,
                                                       WGL_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
                                                       WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                                       0};
    m_handleRenderingContext = wglCreateContextAttribsARB(m_handleDeviceContext, nullptr, contextAttributes.data());
    NOX_ENSURE_RETURN_FALSE_MSG(m_handleRenderingContext != nullptr, "Couldn't create OpenGL 4.6 context");

    wglMakeCurrent(m_handleDeviceContext, m_handleRenderingContext);

    return true;
}

void WindowsGLContext::swapBuffers() const {
    SwapBuffers(m_handleDeviceContext);
}

void WindowsGLContext::setSwapInterval(bool value) const {
    wglSwapIntervalEXT(static_cast<int32_t>(value));
}

} // namespace nox
