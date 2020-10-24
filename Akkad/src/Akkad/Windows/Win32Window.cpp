#include "Win32Window.h"
#include "Win32WindowProcedure.h"
#include "Win32Helpers.h"
namespace Akkad {


	
    int Win32Window::Init(WindowSettings settings)
    {
        HMODULE hInstance;
        hInstance = GetModuleHandle(NULL);

        // Register the window class.
        const wchar_t CLASS_NAME[] = L"Window Class";

        WNDCLASS wc = { };

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        // Create the window.
        HWND hwnd = CreateWindowEx(
            0,                              // Optional window styles.
            CLASS_NAME,                     // Window class
            convertCharArrayToLPCWSTR(settings.title),    // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Size and position
            0, 0, settings.width, settings.height,

            NULL,       // Parent window    
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL      // Additional application data
        );

        if (hwnd == NULL)
        {
            return 0;
        }

        ShowWindow(hwnd, 1);
        m_IsClosed = false;
        SetProp(hwnd, L"windowclass", this);
        return 0;
    }

    void Win32Window::OnUpdate()
    {
        MSG msg = { };
        GetMessage(&msg, NULL, 0, 0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            m_IsClosed = true;
        }


    }

    bool Win32Window::IsCloseRequested()
    {
        return m_IsClosed;
    }

}

