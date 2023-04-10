#include <nox/window/event.h>
#include <nox/window/window.h>

#define FOREACH_EVENT_LISTENER(function, ...)                                    \
    for (const auto &eventDispatcher : m_impl->eventDispatchers) {               \
        if (eventDispatcher.function && eventDispatcher.function(__VA_ARGS__)) { \
            break;                                                               \
        }                                                                        \
    }

namespace NOX {

struct Window::Impl {
    bool shouldClose{false};
    std::vector<EventDispatcher> eventDispatchers;
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
    FOREACH_EVENT_LISTENER(closeEventCallback);
}

void Window::onResizeEvent(uint32_t width, uint32_t height) const {
    const ResizeEvent event{{width, height}};
    FOREACH_EVENT_LISTENER(resizeEventCallback, event);
}

} // namespace NOX
