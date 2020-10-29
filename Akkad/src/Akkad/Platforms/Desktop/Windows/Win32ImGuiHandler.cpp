#include "Akkad/PlatformMacros.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/ImGuiHandler.h"

#ifdef AK_PLATFORM_WINDOWS
	#include "backends/imgui_impl_win32.h"
#endif 

namespace Akkad {
	namespace Graphics {
		#ifdef AK_PLATFORM_WINDOWS
			void Graphics::ImGuiWindowHandler::Init() {
				ImGui_ImplWin32_Init(Application::GetInstance().GetWindow()->GetNativeWindow());
			}

			void Graphics::ImGuiWindowHandler::ShutDown() {
				ImGui_ImplWin32_Shutdown();
			}

			void Graphics::ImGuiWindowHandler::NewFrame() {
				ImGui_ImplWin32_NewFrame();
			}
		#endif
	}
}