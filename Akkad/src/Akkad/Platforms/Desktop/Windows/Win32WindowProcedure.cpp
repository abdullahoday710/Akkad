#include "Win32WindowProcedure.h"
#include "Win32Window.h"
#include <iostream>
#include <tchar.h>
#include "Akkad/Input/KeyEvent.h"
#include "WindowsKeyCodes.h"

namespace Akkad {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
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
        case WM_SIZE:
            switch (wParam)
            {
            case SIZE_MINIMIZED:
                return 0;
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

