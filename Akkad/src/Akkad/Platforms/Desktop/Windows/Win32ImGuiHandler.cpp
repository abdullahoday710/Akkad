#include "Akkad/PlatformMacros.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/ImGuiHandler.h"

#ifdef AK_PLATFORM_WINDOWS
	#include "backends/imgui_impl_win32.h"
#endif 
#include "Win32Window.h"
#include <Windows.h>

namespace Akkad {
	namespace Graphics {
		#ifdef AK_PLATFORM_WINDOWS

			void Graphics::ImGuiWindowHandler::Init() {
				Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();
				ImGui_ImplWin32_EnableDpiAwareness();
				ImGui_ImplWin32_Init(window->GetNativeWindow(), window->m_GLContext);
			}

			void Graphics::ImGuiWindowHandler::ShutDown() {
				ImGui_ImplWin32_Shutdown();
			}

			void ImGuiWindowHandler::UpdateRenderPlatforms()
			{
				auto io = ImGui::GetIO();

				Win32Window* window = (Win32Window*)Application::GetInstance().GetWindow();

				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					// when moving around ImGui viewports, the OpenGL context gets
					// messed up in the process so we have to back it up before changing.
					if (window->m_RenderAPI == RenderAPI::OPENGL)
					{
						auto backUpDC = window->m_DeviceContext;
						ImGui::UpdatePlatformWindows();
						ImGui::RenderPlatformWindowsDefault();
						window->m_DeviceContext = backUpDC;
						wglMakeCurrent(window->m_DeviceContext, window->m_GLContext);
					}
					
				}
			}

			void Graphics::ImGuiWindowHandler::NewFrame() {
				ImGui_ImplWin32_NewFrame();
			}
		#endif
	}
}