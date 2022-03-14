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
        ResetKeyStates();
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
        GetWindowRect(m_WindowHandle, &windowrect);

        glm::vec2 min = { windowrect.left, windowrect.top};
        return min;
    }

    glm::vec2 Win32Window::GetWindowRectMax()
    {
        RECT windowrect;
        GetWindowRect(m_WindowHandle, &windowrect);

        glm::vec2 max = { windowrect.right, windowrect.bottom };
        return max;
    }

    void Win32Window::ToggleFullScreen()
    {
        if (!m_FullScreen)
        {
            SetWindowLongPtr(m_WindowHandle, GWL_STYLE,
                WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
            MoveWindow(m_WindowHandle, 0, 0, 1920, 1080, TRUE);

            DEVMODE dm;
            ZeroMemory(&dm, sizeof(dm));
            EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
            ChangeDisplaySettings(&dm, 0);
            m_FullScreen = true;
        }

        else
        {
            RECT rect;
            rect.left = 0;
            rect.top = 0;
            rect.right = 800;
            rect.bottom = 600;
            SetWindowLongPtr(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
            AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
            MoveWindow(m_WindowHandle, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);

            ChangeDisplaySettings(0, 0);
            m_FullScreen = false;
        }
        
    }

    void Win32Window::ResetKeyStates()
    {
        for (size_t i = 0; i < 3; i++)
        {
            m_MouseStates[i] = -1;
        }
    }

}

