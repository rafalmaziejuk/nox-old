#include "window/windows/windows_window.h"
#include "window/windows/windows_window_helper.h"
#include "window/windows/windows_window_procedure.inl"

namespace NOX {

std::unique_ptr<Window> Window::create(const WindowDescriptor &descriptor) {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    return std::make_unique<WindowsWindow>(descriptor);
}

WindowsWindow::WindowsWindow(const WindowDescriptor &descriptor) : m_descriptor{descriptor} {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    auto style = WindowsWindowHelper::getWindowStyle(descriptor);
    auto clientArea = WindowsWindowHelper::getWindowClientArea(descriptor, style);
    auto position = WindowsWindowHelper::getWindowPosition(descriptor, clientArea);
    auto size = WindowsWindowHelper::getWindowSize(clientArea);

    WNDCLASS attributes{};
    attributes.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    attributes.lpfnWndProc = windowProcedure;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    attributes.hCursor = LoadCursor(nullptr, IDC_ARROW);
    attributes.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    attributes.lpszClassName = TEXT(windowClassName);
    WindowsWindowHelper::registerWindowClass(attributes);

    m_handle = CreateWindow(TEXT(windowClassName),
                            TEXT(descriptor.title.c_str()),
                            style,
                            position.x(),
                            position.y(),
                            size.x(),
                            size.y(),
                            nullptr,
                            nullptr,
                            GetModuleHandle(nullptr),
                            nullptr);
    NOX_ASSERT_MSG(m_handle == nullptr, "Unable to create [{}] window", descriptor.title);
    NOX_LOG_INFO(WINDOW, "Created window (title: [{}], size: [{} x {}], position: [{} x {}])", descriptor.title, size.x(), size.y(), position.x(), position.y());

    SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

WindowsWindow::~WindowsWindow() {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    auto result = DestroyWindow(m_handle);
    NOX_ASSERT_MSG(!result, "Unable to destroy [{}] window", m_descriptor.title);

    WindowsWindowHelper::unregisterWindowClass(windowClassName);
}

void WindowsWindow::show() const {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    ShowWindow(m_handle, SW_NORMAL);
}

void WindowsWindow::processEvents() const {
    NOX_LOG_TRACE_DECLARE_ONCE(WINDOW);

    MSG message;
    while (PeekMessage(&message, m_handle, 0u, 0u, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void WindowsWindow::postCloseEvent() {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    onCloseEvent();
}

void WindowsWindow::postResizeEvent(uint32_t width, uint32_t height) {
    NOX_LOG_TRACE_DECLARE_ONCE(WINDOW);

    onResizeEvent(width, height);
}

void *WindowsWindow::getNativeHandle() const {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    return m_handle;
}

Vector2D<uint32_t> WindowsWindow::getSize() const {
    NOX_LOG_TRACE_DECLARE(WINDOW);

    return m_descriptor.size;
}

} // namespace NOX
