#pragma once

#include <nox/vec.h>

namespace NOX {

struct ResizeEvent {
    Vector2D<uint32_t> size;
};

struct EventDispatcher {
    using CloseEventCallback = std::function<bool()>;
    using ResizeEventCallback = std::function<bool(const ResizeEvent &event)>;

    CloseEventCallback closeEventCallback{};
    ResizeEventCallback resizeEventCallback{};
};

} // namespace NOX
