#include "window/windows/windows_window.h"

#include <nox/config.h>

namespace NOX {

namespace {

DWORD getWindowStyle(const WindowDescriptor &descriptor) {
    DWORD result{};

    if (descriptor.isVisible) {
        result |= WS_VISIBLE;
    }

    if (descriptor.isBorderless) {
        result |= WS_POPUP;
    } else {
        result |= (WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION);
        if (descriptor.isResizable) {
            result |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
        }
    }

    return result;
}

RECT getWindowClientArea(const WindowDescriptor &descriptor, DWORD style) {
    RECT rectangle;
    rectangle.left = 0;
    rectangle.top = 0;
    rectangle.right = static_cast<LONG>(descriptor.size.x());
    rectangle.bottom = static_cast<LONG>(descriptor.size.y());

    AdjustWindowRect(&rectangle, style, FALSE);

    return rectangle;
}

Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, RECT clientArea) {
    if (!descriptor.isCentered) {
        return descriptor.position;
    }

    auto x = static_cast<int32_t>(GetSystemMetrics(SM_CXSCREEN) / 2 - descriptor.size.x() / 2);
    auto y = static_cast<int32_t>(GetSystemMetrics(SM_CYSCREEN) / 2 - descriptor.size.y() / 2);
    x += static_cast<int32_t>(clientArea.left);
    y += static_cast<int32_t>(clientArea.top);

    return {x, y};
}

Vector2D<uint32_t> getWindowSize(RECT clientArea) {
    auto width = static_cast<uint32_t>(clientArea.right - clientArea.left);
    auto height = static_cast<uint32_t>(clientArea.bottom - clientArea.top);
    return {width, height};
}

WindowsWindow *getUserData(HWND hwnd) {
    return reinterpret_cast<WindowsWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
}

LRESULT handleCloseMessage(HWND hwnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) {
    if (auto *window = getUserData(hwnd)) {
        window->postCloseEvent();
    }
    return 0;
}

LRESULT handleResizeMessage(HWND hwnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam) {
    if (auto *window = getUserData(hwnd)) {
        auto width = static_cast<uint32_t>(LOWORD(lParam));
        auto height = static_cast<uint32_t>(HIWORD(lParam));
        window->postResizeEvent(width, height);
    }
    return 0;
}

using MessageHandler = std::pair<UINT, LRESULT (*)(HWND, UINT, WPARAM, LPARAM)>;
constexpr std::array<MessageHandler, 2> messageHandlers{{
    {WM_CLOSE, handleCloseMessage},
    {WM_SIZE, handleResizeMessage},
}};

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto handler = std::find_if(messageHandlers.begin(), messageHandlers.end(),
                                [uMsg](const MessageHandler &handler) {
                                    return (handler.first == uMsg);
                                });
    if (handler != messageHandlers.end()) {
        return handler->second(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

} // namespace

std::unique_ptr<Window> Window::create(const WindowDescriptor &descriptor) {
    return std::make_unique<WindowsWindow>(descriptor);
}

WindowsWindow::WindowsWindow(const WindowDescriptor &descriptor) : m_descriptor{descriptor} {
    auto style = getWindowStyle(descriptor);
    auto clientArea = getWindowClientArea(descriptor, style);
    auto position = getWindowPosition(descriptor, clientArea);
    auto size = getWindowSize(clientArea);

    WNDCLASS attributes{};
    attributes.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    attributes.lpfnWndProc = windowProcedure;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    attributes.hCursor = LoadCursor(nullptr, IDC_ARROW);
    attributes.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    attributes.lpszClassName = TEXT(windowClassName);
    RegisterClass(&attributes);

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
    NOX_ASSERT_MSG(m_handle != nullptr, "Couldn't create window");

    SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

WindowsWindow::~WindowsWindow() {
    NOX_ASSERT(m_handle != nullptr);

    DestroyWindow(m_handle);
    m_handle = nullptr;

    UnregisterClass(TEXT(windowClassName), GetModuleHandle(nullptr));
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
