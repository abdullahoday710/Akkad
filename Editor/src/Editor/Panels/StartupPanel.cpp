#include "StartupPanel.h"
#include "SceneHierarchyPanel.h"
#include "AssetBrowserPanel.h"
#include "ViewPortPanel.h"
#include "GameViewPanel.h"
#include "NewProjectPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/ProjectSerializer.h"

#include <Akkad/PlatformUtils.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>

#include <imgui.h>
namespace Akkad
{
	bool StartupPanel::showPanel;

	void StartupPanel::DrawImGui()
	{
		ImGui::Begin("Welcome");
		if (ImGui::Button("Open a project"))
		{
			std::string projectPath = PlatformUtils::OpenFileDialog();

			if (!projectPath.empty())
			{
				auto project = ProjectSerializer::LoadProject(projectPath);
				EditorLayer::s_ActiveProject = project;

				PanelManager::AddPanel(new SceneHierarchyPanel());
				PanelManager::AddPanel(new AssetBrowserPanel());
				PanelManager::AddPanel(new ViewPortPanel());
				PanelManager::AddPanel(new GameViewPanel());

				showPanel = false;
			}
			
		}

		if (ImGui::Button("Create new project"))
		{
			PanelManager::AddPanel(new NewProjectPanel());
		}
		
		ImGui::End();
	}

	void StartupPanel::OnClose()
	{
		showPanel = false;
	}
}