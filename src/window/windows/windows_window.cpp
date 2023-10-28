#include "window/windows/windows_window.h"
#include "window/windows/windows_window_helper.h"
#include "window/windows/windows_window_procedure.inl"

namespace NOX {

std::unique_ptr<Window> Window::create(const WindowDescriptor &descriptor) {
    return std::make_unique<WindowsWindow>(descriptor);
}

WindowsWindow::WindowsWindow(const WindowDescriptor &descriptor) : m_descriptor{descriptor} {
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
    NOX_ASSERT(m_handle == nullptr);

    SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

WindowsWindow::~WindowsWindow() {
    auto result = DestroyWindow(m_handle);
    NOX_ASSERT(!result);

    WindowsWindowHelper::unregisterWindowClass(windowClassName);
}

void WindowsWindow::show() const {
    ShowWindow(m_handle, SW_NORMAL);
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
}

void WindowsWindow::postResizeEvent(uint32_t width, uint32_t height) {
    onResizeEvent(width, height);
}

void *WindowsWindow::getNativeHandle() const {
    return m_handle;
}

Vector2D<uint32_t> WindowsWindow::getSize() const {
    return m_descriptor.size;
}

} // namespace NOX
