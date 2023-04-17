#include "window/windows/windows_window_class.h"
#include "window/windows/windows_window_procedure.h"

namespace NOX {

WindowsWindowClass::WindowsWindowClass() {
    populateWindowMessageHandlers();

    WNDCLASS attributes{};
    attributes.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
    attributes.lpfnWndProc = windowProcedure;
    attributes.hInstance = GetModuleHandle(nullptr);
    attributes.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    attributes.hCursor = LoadCursor(nullptr, IDC_ARROW);
    attributes.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    attributes.lpszClassName = TEXT(className);

    auto result = RegisterClass(&attributes);
    NOX_ASSERT_MSG(!result, "Unable to register window class");
    NOX_LOG_TRACE(WINDOW, "Registered [{}] window class", className);
}

WindowsWindowClass::~WindowsWindowClass() {
    auto result = UnregisterClass(TEXT(className), GetModuleHandle(nullptr));
    NOX_ASSERT_MSG(!result, "Unable to unregister window class");
    NOX_LOG_TRACE(WINDOW, "Unregistered [{}] window class", className);
}

} // namespace NOX
