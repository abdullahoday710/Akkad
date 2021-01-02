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
			for (auto& asset : project.projectData["project"]["Assets"].items())
			{
				std::string assetID = asset.key();
				std::string assetName = project.projectData["project"]["Assets"][assetID]["name"];
				std::string assetNameIcon = ICON_FK_FILE + std::string(" ") + assetName;
				ImGui::Button(assetNameIcon.c_str());
			}

		}
		
		ImGui::End();
	}

	void AssetBrowserPanel::OnClose()
	{

	}

}