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

}

