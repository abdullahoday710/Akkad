#include "EditorLayer.h"
#include "ProjectSerializer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertyEditorPanel.h"
#include "Panels/NewProjectPanel.h"

#include <Akkad/Logging.h>
#include <Akkad/PlatformUtils.h>
#include <Akkad/Input/Input.h>
#include <Akkad/Input/KeyCodes.h>
#include <Akkad/ECS/Entity.h>
#include <Akkad/ECS/Components/Components.h>
#include <Akkad/ECS/SceneSerializer.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.cpp>

namespace Akkad {
	float EditorLayer::s_AspectRatio;
	Scene* EditorLayer::s_ActiveScene;
	

	EditorLayer::EditorLayer()
		:m_EditorCamera()
	{
		s_ActiveScene = new Scene();

		SceneHierarchyPanel* panel = new SceneHierarchyPanel();
		PanelManager::AddPanel(panel);
	}
	void EditorLayer::NewScene(std::string& sceneName)
	{
		// when switching scenes property editor must be empty.
		//if its not, we will get an assertion failed from entt because the previous scene is a nullptr.
		PropertyEditorPanel::SetActiveEntity({}); 
		delete s_ActiveScene;
		s_ActiveScene = new Scene(sceneName);
	}
	void EditorLayer::LoadScene(std::string& filepath)
	{
		PropertyEditorPanel::SetActiveEntity({});
		delete s_ActiveScene;
		s_ActiveScene = SceneSerializer::Deserialize(filepath);
	}

	void EditorLayer::SaveActiveScene()
	{
		std::string path = "res/scenes/" + s_ActiveScene->m_Name + ".AKSCENE";
		SceneSerializer::Serialize(s_ActiveScene, path);
	}

	void EditorLayer::OnAttach()
	{
		auto platform = Application::GetRenderPlatform();

		FrameBufferDescriptor desc;

		desc.width = Application::GetInstance().GetWindow()->GetWidth();
		desc.height = Application::GetInstance().GetWindow()->GetHeight();
		desc.hasColorAttachment = true;

		auto framebuffer = platform->CreateFrameBuffer(desc);
		m_FrameBuffer = framebuffer;
		ApplyImGuiStyles();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnScenePlay()
	{
		m_IsPlaying = true;

		s_ActiveScene->Start();
	}

	void EditorLayer::OnSceneStop()
	{
		m_IsPlaying = false;
		s_ActiveScene->Stop();
	}

	void EditorLayer::OnUpdate()
	{
		auto desc = m_FrameBuffer->GetDescriptor();

		float ratio = desc.width / desc.height;
		s_AspectRatio = ratio;

		m_FrameBuffer->Bind();
	
		if (m_IsPlaying)
		{
			s_ActiveScene->Update();
		}

		else
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			command->Clear();
			auto view = s_ActiveScene->m_Registry.view<TransformComponent, SpriteRendererComponent>();
			m_EditorCamera.Update();
			Renderer2D::BeginScene(m_EditorCamera, m_EditorCamera.GetTransformMatrix());
			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(spriteRenderer.color, transform.GetTransformMatrix());
			}
		}
		
		m_FrameBuffer->Unbind();
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
		


		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor", NULL, host_window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		DrawMainMenuBar();
		DrawViewport();

		for (auto panel : PanelManager::GetPanels())
		{
			if (panel)
			{
				if (!panel->IsOpen())
				{
					PanelManager::RemovePanel(panel);
					continue;
				}
				panel->DrawImGui();
			}
			
		}

		ImGui::End();

		// Handle keyboard shortcuts

		if (Input::GetKeyDown(AK_KEY_LEFT_CONTROL) & Input::GetKeyDown(AK_KEY_S))
		{
			SaveActiveScene();
		}
	}

	void EditorLayer::DrawViewport()
	{
		ImGui::Begin("Viewport");
		if (!m_IsPlaying)
		{
			if (ImGui::Button("Play"))
			{
				OnScenePlay();
			}
		}
		else
		{
			if (ImGui::Button("Stop"))
			{
				OnSceneStop();
			}
		}
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_FrameBuffer->SetSize(viewportPanelSize.x, viewportPanelSize.y);
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentTexture(), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}

	void EditorLayer::DrawMainMenuBar()
	{
		bool NewScenepopup = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();
				if (ImGui::BeginMenu("New"))
				{
					if (ImGui::MenuItem("Scene"))
					{
						NewScenepopup = true;
					}

					if (ImGui::MenuItem("Project"))
					{
						PanelManager::AddPanel(new NewProjectPanel());
					}
					ImGui::EndMenu();
					
				}

				if (ImGui::MenuItem("Save", "ctrl + s"))
				{
					SaveActiveScene();
				}

				if (ImGui::MenuItem("Open"))
				{
					std::string scenePath = PlatformUtils::OpenFileDialog();

					if (!scenePath.empty())
					{
						LoadScene(scenePath);
					}

				}

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
					Entity e = s_ActiveScene->AddEntity();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Hirearchy"))
				{
					SceneHierarchyPanel* panel = new SceneHierarchyPanel();
					PanelManager::AddPanel(panel);
				}

				if (ImGui::MenuItem("Property Editor"))
				{
					PropertyEditorPanel* panel = new PropertyEditorPanel();
					PanelManager::AddPanel(panel);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();

			if (NewScenepopup)
			{
				ImGui::OpenPopup("New Scene");
			}

			if (ImGui::BeginPopupModal("New Scene"))
			{
				std::string sceneName = "";
				if (ImGui::InputText("Scene Name", &sceneName, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					NewScene(sceneName);
					NewScenepopup = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	}

	void EditorLayer::ApplyImGuiStyles() {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Medium.ttf", 16.0f);
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 2.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 13.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 4.0f;
	}
}