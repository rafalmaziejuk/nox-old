#pragma once

#include <Windows.h>

namespace NOX {

void populateWindowMessageHandlers();
LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

} // namespace NOX
