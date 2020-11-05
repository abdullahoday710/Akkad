#include "Win32WindowProcedure.h"
#include "Win32Window.h"
#include <iostream>
#include <tchar.h>
#include "Akkad/Input/KeyEvent.h"
#include "WindowsKeyCodes.h"

// compile imgui for windows
#include "backends/imgui_impl_win32.cpp"

namespace Akkad {

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        #ifdef AK_ENABLE_IMGUI
            ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
        #endif

        Win32Window* window = (Win32Window*)GetProp(hwnd, L"windowclass");
        
        switch (uMsg)
        {
        case WM_DESTROY:
            
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN:
        {
            unsigned int code = MapVirtualKeyW(wParam, MAPVK_VK_TO_VSC);
            KeyPressEvent e(Win32KeyCodeToAkkadKeyCode(code));
            window->m_EventCallback(e);
            
            return 0;
        }
        case WM_SIZING:
        {
            RECT* rect;
            rect = (RECT*)lParam;

            unsigned int width = rect->right - rect->left;
            unsigned int height = rect->bottom - rect->top;

            WindowResizeEvent e(width, height);
            window->m_EventCallback(e);
            return 0;
        }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

