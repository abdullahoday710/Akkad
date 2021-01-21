#include "NewScenePanel.h"
#include "PropertyEditorPanel.h"

#include "Editor/EditorLayer.h"

#include <Akkad/ECS/SceneSerializer.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Akkad {
	bool NewScenePanel::showPanel;

	std::string sceneName = "";

	void NewScenePanel::DrawImGui()
	{
		ImGui::Begin("New Scene", &showPanel);
		
		ImGui::InputText("Scene Name", &sceneName, ImGuiInputTextFlags_EnterReturnsTrue);
		
		if (ImGui::Button("Create"))
		{
			auto& project = EditorLayer::GetActiveProject();

			bool null = project.projectData["project"]["Scenes"][sceneName].is_null();
			if (null)
			{
				// NOTE: when switching scenes property editor must be empty.
				//if its not, we will get an assertion failed from entt because the previous scene is destroyed.
				PropertyEditorPanel::SetActiveEntity({});
				EditorLayer::s_ActiveScene.reset(new Scene(sceneName));

				std::string path = project.GetAssetsPath().append("scenes/").string() + EditorLayer::s_ActiveScene->GetName() + ".AKSCENE";

				SceneSerializer::Serialize(EditorLayer::s_ActiveScene, path);

				path = EditorLayer::s_ActiveScene->GetName() + ".AKSCENE";

				project.projectData["project"]["Scenes"][sceneName] = path;

				EditorLayer::SaveActiveProject();

				showPanel = false;
			}
		}

		ImGui::End();
	}

	void NewScenePanel::OnClose()
	{
		sceneName = "";
		showPanel = false;
	}
}