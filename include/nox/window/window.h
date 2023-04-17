#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/maths/vector2d.h>

#include <memory>
#include <string>

namespace NOX {

struct EventDispatcher;

struct WindowDescriptor {
    std::string title;
    Vector2D<uint32_t> size;
    Vector2D<int32_t> position;

    bool isVisible;
    bool isResizable;
    bool isBorderless;
    bool isCentered;
};

class NOX_EXPORT Window : public NonCopyable {
  public:
    Window();
    virtual ~Window();

    [[nodiscard]] static std::unique_ptr<Window> create(const WindowDescriptor &descriptor);

    bool shouldClose() const;

    void pushFrontEventDispatcher(const EventDispatcher &eventDispatcher);
    void pushBackEventDispatcher(const EventDispatcher &eventDispatcher);

    virtual void show() const = 0;

    virtual void processEvents() const = 0;

  protected:
    void onCloseEvent() const;
    void onResizeEvent(uint32_t width, uint32_t height) const;

  private:
    struct Impl;
    Impl *m_impl{nullptr};
};

} // namespace NOX
