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

			auto& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			Graphics::ImGuiWindowHandler::Init();

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
		void GLImGuiHandler::UpdateRenderPlatforms()
		{
			ImGuiWindowHandler::UpdateRenderPlatforms();
		}
	}
}