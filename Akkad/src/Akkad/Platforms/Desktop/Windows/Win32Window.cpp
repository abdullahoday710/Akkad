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

        // window pixel format
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
            1,                     // version number  
            PFD_DRAW_TO_WINDOW |   // support window  
            PFD_SUPPORT_OPENGL |   // support OpenGL  
            PFD_DOUBLEBUFFER,      // double buffered  
            PFD_TYPE_RGBA,         // RGBA type  
            24,                    // 24-bit color depth  
            0, 0, 0, 0, 0, 0,      // color bits ignored  
            0,                     // no alpha buffer  
            0,                     // shift bit ignored  
            0,                     // no accumulation buffer  
            0, 0, 0, 0,            // accum bits ignored  
            32,                    // 32-bit z-buffer      
            0,                     // no stencil buffer  
            0,                     // no auxiliary buffer  
            PFD_MAIN_PLANE,        // main layer  
            0,                     // reserved  
            0, 0, 0                // layer masks ignored  
        };

        m_DeviceContext = GetDC(hwnd);
        int pf = ChoosePixelFormat(m_DeviceContext, &pfd);
        SetPixelFormat(m_DeviceContext, pf, &pfd);

        ShowWindow(hwnd, 1);
        m_IsClosed = false;
        m_WindowHandle = hwnd;
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

    void Win32Window::CreateContext(Graphics::RenderAPI api)
    {
        switch (api)
        {
        case Graphics::RenderAPI::OPENGL:
        {
            HGLRC rc;
            rc = wglCreateContext(m_DeviceContext);
            wglMakeCurrent(m_DeviceContext, rc);
            m_RenderAPI = api;
            m_GLContext = rc;
            break;
        }

        }
    }

    void Win32Window::SwapWindowBuffers()
    {
        SwapBuffers(m_DeviceContext);
    }

    bool WGLExtensionSupported(const char* extension_name)
    {
        // this is pointer to function which returns pointer to string with list of all wgl extensions
        PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

        _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

        if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
        {
            // extension is not supported
            return false;
        }

        // extension is supported
        return true;
    }

    void Win32Window::SetVsync(bool status)
    {
        m_VsyncEnabled = status;

        if (m_RenderAPI == Graphics::RenderAPI::OPENGL)
        {
            
            if (WGLExtensionSupported("WGL_EXT_swap_control"))
            {
                PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
                wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
                wglSwapIntervalEXT(status);
            }
        }
    }

}

