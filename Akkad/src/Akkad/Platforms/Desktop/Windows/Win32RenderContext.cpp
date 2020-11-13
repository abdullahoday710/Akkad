#include "Win32RenderContext.h"
#include "Win32Window.h"
#include "Akkad/Application/Application.h"
#include "GLHelpers.h"

#include "wglext.h"
#include <glad/glad.h>
#include <Windows.h>
namespace Akkad {
	namespace Graphics {

		SharedPtr<RenderContext> RenderContext::Create() {
			return CreateSharedPtr<Win32RenderContext>();
		}

		void Win32RenderContext::Init(RenderAPI api)
		{
			switch (api)
			{

			case RenderAPI::OPENGL:
			{
				Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
				GLContextData data = CreateContextGL((HWND)window->GetNativeWindow());

				m_API = api;
				m_GLContext = data.rc;
				m_DeviceContext = data.dc;

				gladLoadGL();
				glViewport(0, 0, window->GetWidth(), window->GetHeight());
			}
				
			}
		}

		void Win32RenderContext::SwapWindowBuffers()
		{
			switch (m_API)
			{
				case RenderAPI::OPENGL:
				{
					SwapBuffers(m_DeviceContext);
					break;
				}
			}
		}
		void Win32RenderContext::SetVsync(bool status)
		{
			m_VsyncEnabled = status;

			if (m_API == Graphics::RenderAPI::OPENGL)
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
}