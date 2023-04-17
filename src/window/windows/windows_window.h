#pragma once

#include <nox/window/window.h>

#include <Windows.h>

namespace NOX {

class WindowsWindow final : public Window {
  public:
    WindowsWindow(const WindowDescriptor &descriptor);
    ~WindowsWindow() override;

    void show() const override;

    void processEvents() const override;
    void postCloseEvent();
    void postResizeEvent(uint32_t width, uint32_t height);

  private:
    static constexpr auto windowClassName = "__NOX_WINDOW_CLASS__";
    HWND m_handle{nullptr};

    WindowDescriptor m_descriptor;
};

} // namespace NOX
