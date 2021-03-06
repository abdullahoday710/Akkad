#include "NewProjectPanel.h"
#include "SceneHierarchyPanel.h"
#include "AssetBrowserPanel.h"
#include "ViewPortPanel.h"
#include "GameViewPanel.h"

#include "Editor/Serializers/ProjectSerializer.h"
#include "Editor/EditorLayer.h"

#include <Akkad/PlatformUtils.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string>
namespace Akkad
{
	bool NewProjectPanel::showPanel;

	std::string projectName;
	std::string projectPath;

	void NewProjectPanel::DrawImGui()
	{
		ImGui::Begin("New Project", &showPanel);

		ImGui::InputText("Project name", &projectName);
		ImGui::InputText("Project path", &projectPath);
		if (ImGui::Button("Select project path"))
		{
			projectPath = PlatformUtils::OpenDirectoryDialog("Please select the root directory of your new project");
		}

		if (ImGui::Button("Create project"))
		{
			if (!projectPath.empty())
			{
				auto project = ProjectSerializer::NewProject(projectName, projectPath);
				EditorLayer::SetActiveProject(project);

				PanelManager::AddPanel(new SceneHierarchyPanel());
				PanelManager::AddPanel(new AssetBrowserPanel());
				PanelManager::AddPanel(new ViewPortPanel());
				PanelManager::AddPanel(new GameViewPanel());

				Panel* startupPanel = PanelManager::GetPanel("StartupPanel");

				if (startupPanel != nullptr)
				{
					PanelManager::RemovePanel(startupPanel);
				}

				showPanel = false;
			}
		}

		ImGui::End();
	}

	void NewProjectPanel::OnClose()
	{
		projectName = "";
		projectPath = "";
		showPanel = false;
	}
}