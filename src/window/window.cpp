#include <nox/window.h>

#include <vector>

#define FOREACH_EVENT_DISPATCHER(function, ...)                                  \
    for (const auto &eventDispatcher : m_impl->eventDispatchers) {               \
        if (eventDispatcher.function && eventDispatcher.function(__VA_ARGS__)) { \
            break;                                                               \
        }                                                                        \
    }

namespace NOX {

struct Window::Impl {
    std::vector<EventDispatcher> eventDispatchers;
    bool shouldClose{false};
};

Window::Window() : m_impl{std::make_unique<Impl>()} {}

Window::~Window() = default;

bool Window::shouldClose() const {
    return m_impl->shouldClose;
}

void Window::pushFrontEventDispatcher(const EventDispatcher &eventDispatcher) {
    m_impl->eventDispatchers.insert(m_impl->eventDispatchers.begin(), eventDispatcher);
}

void Window::pushBackEventDispatcher(const EventDispatcher &eventDispatcher) {
    m_impl->eventDispatchers.push_back(eventDispatcher);
}

void Window::onCloseEvent() const {
    m_impl->shouldClose = true;
    FOREACH_EVENT_DISPATCHER(closeEventCallback);
}

void Window::onResizeEvent(uint32_t width, uint32_t height) const {
    const ResizeEvent event{{width, height}};
    FOREACH_EVENT_DISPATCHER(resizeEventCallback, event);
}

} // namespace NOX
