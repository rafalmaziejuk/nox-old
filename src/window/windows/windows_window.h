#pragma once

#include "window/windows/windows_window_class.h"

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
    HWND m_handle{nullptr};
    WindowsWindowClass m_windowClass{};

    WindowDescriptor m_descriptor;
};

} // namespace NOX
