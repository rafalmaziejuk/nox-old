#include "window/windows/windows_window.h"
#include "window/windows/windows_window_procedure.h"

#define USER_DATA(handle) reinterpret_cast<NOX::WindowsWindow *>(GetWindowLongPtr(handle, GWLP_USERDATA))

#define HANDLER(name) \
    LRESULT name(HWND hwnd, [[maybe_unused]] UINT uMsg, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)

namespace NOX {

namespace {
using MessageHandler = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);
std::unordered_map<UINT, MessageHandler> messageHandlers;
} // namespace

HANDLER(handleCloseMessage) {
    if (auto *window = USER_DATA(hwnd)) {
        window->postCloseEvent();
    }
    return 0;
}

HANDLER(handleResizeMessage) {
    if (auto *window = USER_DATA(hwnd)) {
        auto width = static_cast<uint32_t>(LOWORD(lParam));
        auto height = static_cast<uint32_t>(HIWORD(lParam));
        window->postResizeEvent(width, height);
    }
    return 0;
}

void populateWindowMessageHandlers() {
    messageHandlers[WM_CLOSE] = handleCloseMessage;
    messageHandlers[WM_SIZE] = handleResizeMessage;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto handler = messageHandlers.find(uMsg);
    if (handler != messageHandlers.end()) {
        return handler->second(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

} // namespace NOX
