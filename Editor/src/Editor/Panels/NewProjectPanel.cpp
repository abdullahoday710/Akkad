#include "NewProjectPanel.h"
#include "../ProjectSerializer.h"

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
				ProjectSerializer::NewProject(projectName, projectPath);
				showPanel = false;
			}
		}

		ImGui::End();
	}

	void NewProjectPanel::OnClose()
	{
		projectName = "";
		projectPath = "";
	}
}