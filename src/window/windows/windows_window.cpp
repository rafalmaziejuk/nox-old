#include "utilities/windows/windows_helper.h"
#include "window/windows/windows_window.h"
#include "window/windows/windows_window_procedure.inl"

namespace NOX {

std::unique_ptr<Window> Window::create(const WindowDescriptor &descriptor) {
    return std::make_unique<WindowsWindow>(descriptor);
}

WindowsWindow::WindowsWindow(const WindowDescriptor &descriptor) : m_descriptor{descriptor} {
    auto style = WindowsHelper::getWindowStyle(descriptor);
    auto clientArea = WindowsHelper::getWindowClientArea(descriptor, style);
    auto position = WindowsHelper::getWindowPosition(descriptor, clientArea);
    auto size = WindowsHelper::getWindowSize(clientArea);

    WNDCLASS attributes{};
    attributes.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    attributes.lpfnWndProc = windowProcedure;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    attributes.hCursor = LoadCursor(nullptr, IDC_ARROW);
    attributes.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    attributes.lpszClassName = TEXT(windowClassName);
    WindowsHelper::registerWindowClass(attributes);
    populateWindowMessageHandlers();

    m_handle = CreateWindow(TEXT(windowClassName),
                            TEXT(descriptor.title.c_str()),
                            style,
                            position.x,
                            position.y,
                            size.x,
                            size.y,
                            nullptr,
                            nullptr,
                            GetModuleHandle(nullptr),
                            nullptr);
    NOX_ASSERT_MSG(m_handle == nullptr, "Unable to create [{}] window", descriptor.title);
    NOX_LOG_INFO(WINDOW, "Created window (title: [{}], size: [{} x {}], position: [{} x {}])", descriptor.title, size.x, size.y, position.x, position.y);

    SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

WindowsWindow::~WindowsWindow() {
    auto result = DestroyWindow(m_handle);
    NOX_ASSERT_MSG(!result, "Unable to destroy [{}] window", m_descriptor.title);
    NOX_LOG_TRACE(WINDOW, "Destroyed [{}] window", m_descriptor.title);

    WindowsHelper::unregisterWindowClass(windowClassName);
}

void WindowsWindow::show() const {
    ShowWindow(m_handle, SW_NORMAL);
    NOX_LOG_TRACE(WINDOW, "Shown window");
}

void WindowsWindow::processEvents() const {
    MSG message;
    while (PeekMessage(&message, m_handle, 0u, 0u, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void WindowsWindow::postCloseEvent() {
    onCloseEvent();
    NOX_LOG_TRACE(WINDOW, "Close event posted");
}

void WindowsWindow::postResizeEvent(uint32_t width, uint32_t height) {
    onResizeEvent(width, height);
    NOX_LOG_TRACE(WINDOW, "Resize event posted");
}

} // namespace NOX
