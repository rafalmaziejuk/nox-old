#include "window/windows/windows_window_helper.h"

#include <nox/window.h>

namespace NOX::WindowsWindowHelper {

void registerWindowClass(const WNDCLASS &attributes) {
    auto result = RegisterClass(&attributes);
    NOX_ASSERT(!result);
}

void unregisterWindowClass(std::string_view className) {
    auto result = UnregisterClass(TEXT(className.data()), GetModuleHandle(nullptr));
    NOX_ASSERT(!result);
}

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

    auto result = AdjustWindowRect(&rectangle, style, FALSE);
    NOX_ASSERT(!result);

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

} // namespace NOX::WindowsWindowHelper
