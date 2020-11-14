#include "Akkad/PlatformMacros.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/ImGuiHandler.h"

#ifdef AK_PLATFORM_WINDOWS
	#include "backends/imgui_impl_win32.h"
#endif 
#include "Win32Window.h"
#include "Win32RenderContext.h"

#include <Windows.h>

namespace Akkad {
	namespace Graphics {
		#ifdef AK_PLATFORM_WINDOWS

			void Graphics::ImGuiWindowHandler::Init() {
				Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
				SharedPtr<RenderContext> context = Application::GetInstance().GetContext();
				SharedPtr<Win32RenderContext> win32context = std::dynamic_pointer_cast<Win32RenderContext>(context);
				ImGui_ImplWin32_EnableDpiAwareness();
				ImGui_ImplWin32_Init(window->GetNativeWindow(), win32context->m_GLContext);
			}

			void Graphics::ImGuiWindowHandler::ShutDown() {
				ImGui_ImplWin32_Shutdown();
			}

			void ImGuiWindowHandler::UpdateRenderPlatforms()
			{
				auto& io = ImGui::GetIO();

				Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
				SharedPtr<RenderContext> context = Application::GetInstance().GetContext();
				SharedPtr<Win32RenderContext> win32context = DynamicCastPtr<Win32RenderContext>(context);

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					// when moving around ImGui viewports, the OpenGL context gets
					// messed up in the process so we have to back it up before changing.
					if (win32context->m_API == RenderAPI::OPENGL)
					{
						auto backUpDC = win32context->m_DeviceContext;
						ImGui::UpdatePlatformWindows();
						ImGui::RenderPlatformWindowsDefault();
						win32context->m_DeviceContext = backUpDC;
						wglMakeCurrent(win32context->m_DeviceContext, win32context->m_GLContext);	
					}
					
				}
			}

			void Graphics::ImGuiWindowHandler::NewFrame() {
				ImGui_ImplWin32_NewFrame();
			}
		#endif
	}
}