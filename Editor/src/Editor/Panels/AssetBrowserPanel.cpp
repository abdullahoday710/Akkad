#include "AssetBrowserPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/PlatformUtils.h>
#include <Akkad/Random.h>

#include <filesystem>
#include <imgui.h>

namespace Akkad {
	bool AssetBrowserPanel::showPanel;

	namespace filesystem = std::filesystem;

	void AssetBrowserPanel::DrawImGui()
	{
		ImGui::Begin("Asset Browser", &showPanel);
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Button("Add asset"))
			{
				auto& project = EditorLayer::GetActiveProject();

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
					EditorLayer::SaveActiveProject();

					ImGui::CloseCurrentPopup();
				}

			}
			ImGui::EndPopup();
		}

		// TODO : iterate through the asset/ directory and draw icons of the assets
		ImGui::End();
	}

	void AssetBrowserPanel::OnClose()
	{

	}

}