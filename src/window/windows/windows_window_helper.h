#pragma once

#include <nox/maths/vector2d.h>

#include <Windows.h>

namespace NOX {

struct WindowDescriptor;

DWORD getWindowStyle(const WindowDescriptor &descriptor);

RECT getWindowClientArea(const WindowDescriptor &descriptor, DWORD style);

Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, RECT clientArea);

Vector2D<uint32_t> getWindowSize(RECT clientArea);

} // namespace NOX
