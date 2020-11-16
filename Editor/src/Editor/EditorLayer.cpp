#include "EditorLayer.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Akkad/ECS/Entity.h>
#include <Akkad/ECS/Components/TagComponent.h>

namespace Akkad {

	void EditorLayer::OnAttach()
	{
		auto platform = Application::GetRenderPlatform();
		auto shader = platform->CreateShader("res/shaders/test.glsl");;
		auto texture = platform->CreateTexture("res/textures/container.jpg");

		FrameBufferDescriptor desc;

		desc.width = Application::GetInstance().GetWindow()->GetWidth();
		desc.height = Application::GetInstance().GetWindow()->GetHeight();
		desc.hasColorAttachment = true;

		auto framebuffer = platform->CreateFrameBuffer(desc);
		m_FrameBuffer = framebuffer;

		m_Shader = shader;
		m_Texture = texture;
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate()
	{
		auto command = Application::GetRenderPlatform()->GetRenderCommand();

		command->Clear(); // clear the screen
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0, 0, 0.0f));
		trans = glm::rotate(trans, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Shader->Bind();
		m_Shader->SetMat4("transform", trans);

		m_FrameBuffer->Bind();
		command->Clear(); // clear the framebuffer
		Renderer2D::DrawQuad(m_Shader, m_Texture);
		m_FrameBuffer->Unbind();

		m_Scene->Update();
	}

	void EditorLayer::RenderImGui()
	{
		auto viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags host_window_flags = 0;
		host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
		host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		host_window_flags |= ImGuiWindowFlags_NoBackground;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor", NULL, host_window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Close"))
				{
					Application::Shutdown();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Create Entity"))
				{
					Entity e = m_Scene->AddEntity();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Hirearchy"))
				{
					SceneHierarchyPanel* panel = new SceneHierarchyPanel(m_Scene);
					PanelManager::AddPanel(panel);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_FrameBuffer->SetSize(viewportPanelSize.x, viewportPanelSize.y);
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentTexture(), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		for (auto panel : PanelManager::GetPanels())
		{
			if (panel)
			{
				panel->DrawImGui();
			}
			
		}

		ImGui::ShowDemoWindow();

		ImGui::End();
	}
}