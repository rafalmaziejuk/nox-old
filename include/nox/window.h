#pragma once

#include <nox/export.h>
#include <nox/vector.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

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

struct WindowDescriptor {
    std::string title;
    Vector2D<uint32_t> size;
    Vector2D<int32_t> position;

    bool isVisible;
    bool isResizable;
    bool isBorderless;
    bool isCentered;
};

class NOX_EXPORT Window {
  public:
    [[nodiscard]] static std::unique_ptr<Window> create(const WindowDescriptor &descriptor);

    bool shouldClose() const;

    void pushFrontEventDispatcher(const EventDispatcher &eventDispatcher);
    void pushBackEventDispatcher(const EventDispatcher &eventDispatcher);

    virtual void show() const = 0;

    virtual void processEvents() const = 0;

    virtual void *getNativeHandle() const = 0;

    virtual Vector2D<uint32_t> getSize() const = 0;

  protected:
    void onCloseEvent() const;
    void onResizeEvent(uint32_t width, uint32_t height) const;

  public:
    Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;
    virtual ~Window();

  private:
    struct Impl;
    Impl *m_impl{nullptr};
};

} // namespace NOX
