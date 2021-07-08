#include "AssetBrowserPanel.h"
#include "MaterialEditorPanel.h"
#include "TexturePreviewPanel.h"
#include "SpriteAnimationPreviewPanel.h"
#include "InstantiableEntityPreviewPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/MaterialSerializer.h"
#include "Editor/Serializers/SpriteAnimationSerializer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/PlatformUtils.h>
#include <Akkad/Random.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/Graphics/Material.h>
#include <Akkad/Graphics/Sprite.h>
#include <Akkad/ECS/Serializers/InstantiableEntitySerializer.h>

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

	AssetBrowserPanel::AssetBrowserPanel()
	{
		auto platform = Application::GetRenderPlatform();

		m_FileIcon = platform->CreateTexture("res/icons/file-icon.png");
	}

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
				if (ImGui::MenuItem("Sprite animation"))
				{
					NewAssetType = "sprite_animation";
					showNewAsset = true;
				}

				if (ImGui::MenuItem("Instantiable entity"))
				{
					NewAssetType = "instantiable_entity";
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
				case AssetType::UNKNOWN:
					break;
				case AssetType::SHADER:
					fileExtension = ".glsl";
					break;
				case AssetType::MATERIAL:
					fileExtension = ".mat";
					break;
				case AssetType::SPRITE_ANIMATION:
					fileExtension = ".ak_sprite_anim";
					break;
				case AssetType::INSTANTIABLE_ENTITY:
					fileExtension = ".akentity";
					break;
				}

				filesystem::path destPath = project.GetAssetsPath().append(assetName + fileExtension);

				switch (desc.assetType)
				{
				case AssetType::UNKNOWN:
					break;
				case AssetType::MATERIAL:
				{
					SharedPtr<Graphics::Material> mat = CreateSharedPtr<Graphics::Material>(assetName);
					MaterialSerializer::Serialize(mat, destPath.string());
					break;
				}
				case AssetType::SPRITE_ANIMATION:
				{
					SharedPtr<Graphics::SpriteAnimation> animation = CreateSharedPtr<Graphics::SpriteAnimation>();
					SpriteAnimationSerializer::Serialize(animation, assetName, destPath.string());
					break;
				}
				case AssetType::INSTANTIABLE_ENTITY:
				{
					InstantiableEntitySerializer::SerializeEmpty(destPath.string());
					break;
				}
				default:
					std::ofstream file(destPath);
					break;
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

			float padding = 16.0f;
			float iconSize = 128.0f;
			float cellSize = padding + iconSize;
			auto panelSize = ImGui::GetContentRegionAvail();

			int coloumnCount = (int)(panelSize.x / cellSize);

			if (coloumnCount <= 0)
			{
				coloumnCount = 1;
			}
			ImGui::Columns(coloumnCount, 0 , false);
			int id = 1;
			for (auto& asset : project.projectData["project"]["Assets"].items())
			{
				std::string assetID = asset.key();
				std::string assetName = project.projectData["project"]["Assets"][assetID]["name"];
				std::string assetNameIcon = ICON_FK_FILE;
				std::string assetType = project.projectData["project"]["Assets"][assetID]["type"];
				std::string assetPath = project.projectData["project"]["Assets"][assetID]["path"];

				std::string assetAbsolutePath = project.GetProjectDirectory().string() + assetPath;
				ImGui::PushID(id);
				if (ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { iconSize,iconSize }, { 0, 1 }, { 1, 0 }))
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

					if (assetType == "sprite_animation")
					{
						auto animation = Graphics::SpriteAnimation::LoadFile(assetAbsolutePath);
						PanelManager::AddPanel(new SpriteAnimationPreviewPanel());
						SpriteAnimationPreviewPanel::SetActiveAnimation(animation, assetID);
					}

					if (assetType == "instantiable_entity")
					{
						PanelManager::AddPanel(new InstantiableEntityPreviewPanel());
						InstantiableEntityPreviewPanel::SetEntityFile(assetID);
					}
				}
				ImGui::PopID();
				id++;
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::Text(assetName.c_str());
					const char* id = assetID.c_str();
					ImGui::SetDragDropPayload("ASSET_DRAG_DROP", id, strlen(id) + 1);
					ImGui::EndDragDropSource();
				}
				ImGui::TextWrapped(assetName.c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns(1);

		}

		ImGui::End();

	}

	void AssetBrowserPanel::OnClose()
	{
		showPanel = false;
	}

}