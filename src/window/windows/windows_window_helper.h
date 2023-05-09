#pragma once

#include <nox/export.h>
#include <nox/maths/vector2d.h>

#include <Windows.h>

#include <string_view>

namespace NOX {

struct WindowDescriptor;

namespace WindowsWindowHelper {

NOX_EXPORT void registerWindowClass(const WNDCLASS &attributes);
NOX_EXPORT void unregisterWindowClass(std::string_view className);

DWORD getWindowStyle(const WindowDescriptor &descriptor);
RECT getWindowClientArea(const WindowDescriptor &descriptor, DWORD style);
Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, RECT clientArea);
Vector2D<uint32_t> getWindowSize(RECT clientArea);

} // namespace WindowsWindowHelper

} // namespace NOX
