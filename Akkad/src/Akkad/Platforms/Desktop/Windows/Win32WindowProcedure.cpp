#include "Win32WindowProcedure.h"
#include "Win32Window.h"
#include "WindowsKeyCodes.h"

#include "Akkad/Input/KeyEvent.h"
#include "Akkad/Application/Application.h"

#include <tchar.h>

// compile imgui for windows
#include "backends/imgui_impl_win32.cpp"

namespace Akkad {

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (Application::IsImGuiEnabled())
        {
            ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
        }

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

        case WM_SIZE:
        {
            if (window == nullptr)
            {
                return 0;
            }
            else {
                RECT rect;
                GetWindowRect((HWND)window->GetNativeWindow(), &rect);
                unsigned int width = rect.right - rect.left;
                unsigned int height = rect.bottom - rect.top;
                WindowResizeEvent e(width, height);
                window->m_EventCallback(e);
                window->_SetSize(width, height);
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
        
        case WM_MOVE:
        {
            if (window != nullptr)
            {
                WindowResizeEvent e(window->GetWidth(), window->GetHeight());
                window->m_EventCallback(e);
            }
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            int button = -1;
            int buttonState = -1;

            if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
                button = static_cast<int>(MouseButtons::LEFT);
            else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
                button = static_cast<int>(MouseButtons::RIGHT);
            else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
                button = static_cast<int>(MouseButtons::MIDDLE);

            if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
                uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
            {
                buttonState = 0;
            }
            else
            {
                buttonState = 1;
            }

            if (button >= 0)
            {
                window->m_MouseStates[button] = buttonState;
            }

            return 0;
        }

        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

