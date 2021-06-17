#include "AssetBrowserPanel.h"
#include "MaterialEditorPanel.h"
#include "TexturePreviewPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/MaterialSerializer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/PlatformUtils.h>
#include <Akkad/Random.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/Graphics/Material.h>

#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <IconsForkAwesome.h>

namespace Akkad {
	bool AssetBrowserPanel::showPanel;

	bool showNewAsset = false;
	std::string NewAssetType;

	namespace filesystem = std::filesystem;

	void AssetBrowserPanel::DrawImGui()
	{
		ImGui::Begin("Asset Browser", &showPanel);
		auto& project = EditorLayer::GetActiveProject();

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Shader"))
				{
					NewAssetType = "shader";
					showNewAsset = true;
				}
				if (ImGui::MenuItem("Material"))
				{
					NewAssetType = "material";
					showNewAsset = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Add asset"))
			{
				std::string assetPath = PlatformUtils::OpenFileDialog();
				std::string assetName = filesystem::path(assetPath).filename().replace_extension("").string();
				std::string fileExtension = filesystem::path(assetPath).extension().string();

				filesystem::path destnationAssetPath = project.GetAssetsPath().append(assetName + fileExtension);

				if (filesystem::exists(destnationAssetPath))
				{
					// TODO : show file already exists alert
				}
				
				else
				{
					filesystem::copy(assetPath, destnationAssetPath);
					std::string assetID = Random::GenerateRandomUUID();

					std::string assetType = AssetManager::AssetTypeToStr(AssetManager::GetAssetTypeFromFileExtension(fileExtension));

					project.projectData["project"]["Assets"][assetID]["path"] = "assets/" + assetName + fileExtension;
					project.projectData["project"]["Assets"][assetID]["name"] = assetName;
					project.projectData["project"]["Assets"][assetID]["type"] = assetType;

					AssetDescriptor descriptor;

					if (assetType == "shader")
					{
						project.projectData["project"]["Assets"][assetID]["shaderdescPath"] = "null";
						descriptor.absolutePath = "";
					}

					if (assetType == "texture")
					{
						SharedPtr<TextureAssetInfo> textureInfo = CreateSharedPtr<TextureAssetInfo>();
						textureInfo->isTilemap = false;
						textureInfo->tileWidth = 0;
						textureInfo->tileHeight = 0;
						descriptor.assetInfo = textureInfo;
						descriptor.absolutePath = destnationAssetPath.string();
						descriptor.assetName = assetName;
					}

					else
					{
						descriptor.absolutePath = destnationAssetPath.string();
						descriptor.assetName = assetName;
					}
		

					Application::GetAssetManager()->RegisterAsset(assetID, descriptor);
					EditorLayer::SaveActiveProject();

					ImGui::CloseCurrentPopup();
				}

			}
			ImGui::EndPopup();
		}

		if (showNewAsset)
		{
			std::string label = "New " + NewAssetType;
			static std::string assetName;
			
			ImGui::Begin(label.c_str(), &showNewAsset);

			ImGui::InputText("Asset name", &assetName);

			if (ImGui::Button(label.c_str()))
			{
				AssetDescriptor desc;
				desc.assetID = Random::GenerateRandomUUID();
				desc.SetAssetType(NewAssetType);
				
				std::string fileExtension;

				switch (desc.assetType)
				{
				case Akkad::AssetType::UNKNOWN:
					break;
				case Akkad::AssetType::SHADER:
					fileExtension = ".glsl";
					break;
				case Akkad::AssetType::MATERIAL:
					fileExtension = ".mat";
					break;
				}

				filesystem::path destPath = project.GetAssetsPath().append(assetName + fileExtension);
				if (NewAssetType == "material")
				{
					SharedPtr<Graphics::Material> mat = CreateSharedPtr<Graphics::Material>(assetName);
					MaterialSerializer::Serialize(mat, destPath.string());

				}

				else
				{
					std::ofstream file(destPath);
				}

				project.projectData["project"]["Assets"][desc.assetID]["path"] = "assets/" + assetName + fileExtension;
				project.projectData["project"]["Assets"][desc.assetID]["name"] = assetName;
				project.projectData["project"]["Assets"][desc.assetID]["type"] = NewAssetType;

				if (NewAssetType == "shader")
				{
					project.projectData["project"]["Assets"][desc.assetID]["shaderdescPath"] = "null";
					desc.absolutePath = "";
				}

				else
				{
					desc.absolutePath = destPath.string();
				}

				Application::GetAssetManager()->RegisterAsset(desc.assetID, desc);
				EditorLayer::SaveActiveProject();

				assetName = "";
				showNewAsset = false;
				
			}


			ImGui::End();
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
				std::string assetType = project.projectData["project"]["Assets"][assetID]["type"];
				std::string assetPath = project.projectData["project"]["Assets"][assetID]["path"];

				std::string assetAbsolutePath = project.GetProjectDirectory().string() + assetPath;



				if (ImGui::Button(assetNameIcon.c_str()))
				{
					if (assetType == "material")
					{
						auto material = Graphics::Material::LoadFile(assetAbsolutePath);
						PanelManager::AddPanel(new MaterialEditorPanel());
						MaterialEditorPanel::SetActiveMaterial(material, assetID);
					}

					if (assetType == "texture")
					{
						PanelManager::AddPanel(new TexturePreviewPanel());
						TexturePreviewPanel::SetActiveTexture(assetID);
					}
				}

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
		showPanel = false;
	}

}