#pragma once

#include <nox/vec.h>

#include <Windows.h>

#include <string_view>

namespace NOX {

struct WindowDescriptor;

namespace WindowsWindowHelper {

void registerWindowClass(const WNDCLASS &attributes);
void unregisterWindowClass(std::string_view className);

DWORD getWindowStyle(const WindowDescriptor &descriptor);
RECT getWindowClientArea(const WindowDescriptor &descriptor, DWORD style);
Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, RECT clientArea);
Vector2D<uint32_t> getWindowSize(RECT clientArea);

} // namespace WindowsWindowHelper

} // namespace NOX
