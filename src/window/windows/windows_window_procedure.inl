#define USER_DATA(handle) reinterpret_cast<WindowsWindow *>(GetWindowLongPtr(handle, GWLP_USERDATA))

namespace NOX {

LRESULT handleCloseMessage(HWND hwnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/) {
    if (auto *window = USER_DATA(hwnd)) {
        window->postCloseEvent();
    }
    return 0;
}

LRESULT handleResizeMessage(HWND hwnd, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam) {
    if (auto *window = USER_DATA(hwnd)) {
        auto width = static_cast<uint32_t>(LOWORD(lParam));
        auto height = static_cast<uint32_t>(HIWORD(lParam));
        window->postResizeEvent(width, height);
    }
    return 0;
}

namespace {
using MessageHandler = std::pair<UINT, LRESULT (*)(HWND, UINT, WPARAM, LPARAM)>;
constexpr std::array<MessageHandler, 2> messageHandlers{{
    {WM_CLOSE, handleCloseMessage},
    {WM_SIZE, handleResizeMessage},
}};
} // namespace

LRESULT CALLBACK windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto handler = std::find_if(messageHandlers.begin(), messageHandlers.end(),
                                [uMsg](const MessageHandler &handler) {
                                    return (handler.first == uMsg);
                                });
    if (handler != messageHandlers.end()) {
        return handler->second(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

} // namespace NOX
