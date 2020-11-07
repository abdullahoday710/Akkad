#include "GLImGuiHandler.h"
#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.cpp>

#include "Akkad/Application/Application.h"

namespace Akkad {
	namespace Graphics {

		void GLImGuiHandler::Init()
		{
			auto window = Application::GetInstance().GetWindow();
			ImGui::CreateContext();
			Graphics::ImGuiWindowHandler::Init();

			auto& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());

			ImGui_ImplOpenGL3_Init();
		}

		void GLImGuiHandler::NewFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGuiWindowHandler::NewFrame();
			ImGui::NewFrame();
		}

		void GLImGuiHandler::Render()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void GLImGuiHandler::ShutDown()
		{
			ImGui_ImplOpenGL3_Shutdown();
		}
	}
}