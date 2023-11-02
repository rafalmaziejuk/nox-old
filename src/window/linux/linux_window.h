#pragma once

#include <nox/window.h>

#include <X11/Xlib.h>

namespace NOX {

class LinuxWindow final : public Window {
  public:
    explicit LinuxWindow(const WindowDescriptor &descriptor);
    ~LinuxWindow() override;

    void show() const override;
    void processEvents() const override;
    void *getNativeHandle() const override;
    Vector2D<uint32_t> getSize() const override;

    ::Window getWindow() const;

  private:
    void createWindow();
    void initializeWindowSpecification();

    void postEvent(XEvent const &event) const;
    void postCloseEvent() const;
    void postResizeEvent(uint32_t width, uint32_t height) const;

  private:
    WindowDescriptor m_descriptor;

    ::Display *m_display;
    ::Window m_window;

    mutable ::Atom m_closeWindowAtom;
};

} // namespace NOX
