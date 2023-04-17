#pragma once

#include <Windows.h>

namespace NOX {

void populateWindowMessageHandlers();
LRESULT CALLBACK windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

} // namespace NOX
