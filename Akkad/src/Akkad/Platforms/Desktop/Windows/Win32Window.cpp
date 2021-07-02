#include "Win32Window.h"
#include "Win32WindowProcedure.h"
#include "Win32Helpers.h"
#include "Akkad/Graphics/ImGuiHandler.h"
#include "imgui.h"
#include "WindowsKeyCodes.h"
#include "wglext.h"

namespace Akkad {
    int Win32Window::Init(WindowSettings settings)
    {
        m_Width = settings.width;
        m_Height = settings.height;
        HMODULE hInstance;
        hInstance = GetModuleHandle(NULL);

        // Register the window class.
        const wchar_t CLASS_NAME[] = L"Window Class";

        WNDCLASS wc = { };

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);
        // create the key codes table
        MakeWindowskeyCodes();
        // Create the window.
        RECT wr = { 0, 0, settings.width, settings.height };
        HWND hwnd = CreateWindow(CLASS_NAME, convertCharArrayToLPCWSTR(settings.title),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            0, 0,
            wr.right - wr.left,
            wr.bottom - wr.top,
            0, 0, hInstance, 0);

        if (hwnd == NULL)
        {
            return 0;
        }

        ShowWindow(hwnd, 1);
        m_IsClosed = false;
        m_WindowHandle = hwnd;
        SetProp(hwnd, L"windowclass", this);
        
        return 0;
    }

    void Win32Window::OnUpdate()
    {
        MSG msg = { };
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                WindowCloseEvent e;
                m_EventCallback(e);
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
    }

    glm::vec2 Win32Window::GetWindowRectMin()
    {
        RECT windowrect;
        GetClientRect(m_WindowHandle, &windowrect);
        /* Getting the window rect without the title bar */
        POINT winrectmin;
        winrectmin.x = windowrect.left;
        winrectmin.y = windowrect.top;
        ClientToScreen(m_WindowHandle, &winrectmin);

        glm::vec2 min = { winrectmin.x, winrectmin.y};
        return min;
    }

    glm::vec2 Win32Window::GetWindowRectMax()
    {
        RECT windowrect;
        GetClientRect(m_WindowHandle, &windowrect);
        /* Getting the window rect without the title bar */
        POINT winrectmin;
        winrectmin.x = windowrect.right;
        winrectmin.y = windowrect.bottom;
        ClientToScreen(m_WindowHandle, &winrectmin);

        glm::vec2 max = { winrectmin.x, winrectmin.y };
        return max;
    }

}

