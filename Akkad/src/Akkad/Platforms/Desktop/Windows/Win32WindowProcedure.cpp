#include "Win32WindowProcedure.h"
#include "Win32Window.h"
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
            KeyPressEvent e(keyCodes[code]);
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
        case WM_SIZE:
        {
            if (window == nullptr)
            {
                return 0;
            }
            else {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                WindowResizeEvent e(width, height);
                window->m_EventCallback(e);
                return 0;
            }
        }

        case WM_DPICHANGED:
        {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return 0;
        }

        case WM_ERASEBKGND: 
        {
            break;
        }

        case WM_MOUSEMOVE:
        {
            if (!window->isCursorTracked)
            {
                TRACKMOUSEEVENT tme;
                ZeroMemory(&tme, sizeof(tme));
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = (HWND)window->GetNativeWindow();
                TrackMouseEvent(&tme);

                window->isCursorTracked = true;
            }
            return 0;
        }

        case WM_MOUSELEAVE:
        {
            window->isCursorTracked = false;
            return 0;
        }

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

