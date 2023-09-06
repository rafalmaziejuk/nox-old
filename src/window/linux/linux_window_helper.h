#pragma once

#include <nox/vector.h>

typedef struct _XDisplay Display; // NOLINT

namespace NOX {
struct WindowDescriptor;

namespace LinuxWindowHelper {

Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, Display *display);

} // namespace LinuxWindowHelper

} // namespace NOX
