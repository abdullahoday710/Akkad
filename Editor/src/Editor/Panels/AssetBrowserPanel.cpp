#include "AssetBrowserPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/PlatformUtils.h>
#include <Akkad/Random.h>

#include <filesystem>
#include <imgui.h>
#include <IconsForkAwesome.h>

namespace Akkad {
	bool AssetBrowserPanel::showPanel;

	namespace filesystem = std::filesystem;

	void AssetBrowserPanel::DrawImGui()
	{
		ImGui::Begin("Asset Browser", &showPanel);
		auto& project = EditorLayer::GetActiveProject();

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Button("Add asset"))
			{
				std::string assetPath = PlatformUtils::OpenFileDialog();
				std::string assetName = filesystem::path(assetPath).filename().string();

				filesystem::path destnationAssetPath = project.GetAssetsPath().append(assetName);

				if (filesystem::exists(destnationAssetPath))
				{
					// TODO : show file already exists alert
				}
				
				else
				{
					filesystem::copy(assetPath, destnationAssetPath);
					std::string assetID = Random::GenerateRandomUUID();

					project.projectData["project"]["Assets"][assetID]["path"] = "assets/" + assetName;
					project.projectData["project"]["Assets"][assetID]["name"] = assetName;

					AssetDescriptor descriptor;
					descriptor.absolutePath = destnationAssetPath.string();

					Application::GetAssetManager()->RegisterAsset(assetID, descriptor);
					EditorLayer::SaveActiveProject();

					ImGui::CloseCurrentPopup();
				}

			}
			ImGui::EndPopup();
		}
		if (!project.projectData.is_null())
		{
			if (ImGui::TreeNode("Scenes"))
			{
				for (auto& scene : project.projectData["project"]["Scenes"].items())
				{
					std::string sceneName = scene.key();
					std::string sceneFileName = scene.value();
					std::string scenePath = project.GetAssetsPath().append("scenes/").append(sceneFileName).string();

					ImGui::Button(sceneName.c_str());
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
					{
						EditorLayer::LoadScene(scenePath);
					}
					
				}
				ImGui::TreePop();
			}
			
			for (auto& asset : project.projectData["project"]["Assets"].items())
			{
				std::string assetID = asset.key();
				std::string assetName = project.projectData["project"]["Assets"][assetID]["name"];
				std::string assetNameIcon = ICON_FK_FILE + std::string(" ") + assetName;
				ImGui::Button(assetNameIcon.c_str());

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::Text(assetName.c_str());
					const char* id = assetID.c_str();
					ImGui::SetDragDropPayload("ASSET_DRAG_DROP", id, strlen(id)+1);
					ImGui::EndDragDropSource();
				}
			}

		}
		
		ImGui::End();
	}

	void AssetBrowserPanel::OnClose()
	{

	}

}