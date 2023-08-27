#include "window/linux/linux_window_helper.h"

#include <nox/window.h>

#include <X11/Xlib.h>

namespace NOX::LinuxWindowHelper {

Vector2D<int32_t> getWindowPosition(const WindowDescriptor &descriptor, Display *display) {
    if (!descriptor.isCentered) {
        return descriptor.position;
    }

    const int32_t screenId = DefaultScreen(display);

    if (Screen *screen = ScreenOfDisplay(display, screenId)) {
        auto x = static_cast<int32_t>(screen->width / 2 - descriptor.size.x() / 2);
        auto y = static_cast<int32_t>(screen->height / 2 - descriptor.size.y() / 2);
        return {x, y};
    }
    return {0, 0};
}
} // namespace NOX::LinuxWindowHelper
