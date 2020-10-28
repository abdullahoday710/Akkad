#include "GLImGuiHandler.h"
#include "glad/glad.h"
#include "backends/imgui_impl_opengl3.cpp"
namespace Akkad {
	namespace Graphics {

		void GLImGuiHandler::Init()
		{
			ImGui_ImplOpenGL3_Init();
		}

		void GLImGuiHandler::NewFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
		}

		void GLImGuiHandler::Render()
		{
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void GLImGuiHandler::ShutDown()
		{
			ImGui_ImplOpenGL3_Shutdown();
		}
	}
}