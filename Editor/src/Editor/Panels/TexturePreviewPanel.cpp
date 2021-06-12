#include "TexturePreviewPanel.h"

#include "Editor/EditorLayer.h"

#include "Akkad/Graphics/Texture.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include "Akkad/ECS/Components/SpriteRendererComponent.h"
#include <imgui.h>

namespace Akkad {
	bool TexturePreviewPanel::showPanel;
	std::string TexturePreviewPanel::m_TextureAssetID;
	SharedPtr<Graphics::Texture> TexturePreviewPanel::m_Texture;

	void TexturePreviewPanel::DrawImGui()
	{
		ImGui::Begin("Texture Preview", &showPanel);

		ImGui::Image((void*)m_Texture->GetID(), { 200, 200 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto& project = EditorLayer::GetActiveProject();

		bool isAtlas;
		if (project.projectData["project"]["Assets"][m_TextureAssetID]["IsAtlas"].is_null())
		{
			isAtlas = false;
		}

		else
		{
			isAtlas = project.projectData["project"]["Assets"][m_TextureAssetID]["IsAtlas"];
		}

		if (ImGui::Checkbox("Is Atlas", &isAtlas))
		{
			project.projectData["project"]["Assets"][m_TextureAssetID]["IsAtlas"] = isAtlas;
		}

		glm::vec2 atlasTileSize = {0,0};
		if (isAtlas)
		{
			if (project.projectData["project"]["Assets"][m_TextureAssetID]["AtlasTileSize"].is_null())
			{
				atlasTileSize = { 0,0 };
			}

			else
			{
				atlasTileSize.x = project.projectData["project"]["Assets"][m_TextureAssetID]["AtlasTileSize"][0];
				atlasTileSize.y = project.projectData["project"]["Assets"][m_TextureAssetID]["AtlasTileSize"][1];
			}

			if (ImGui::InputFloat2("Atlas tile size", glm::value_ptr(atlasTileSize)))
			{
				project.projectData["project"]["Assets"][m_TextureAssetID]["AtlasTileSize"][0] = atlasTileSize.x;
				project.projectData["project"]["Assets"][m_TextureAssetID]["AtlasTileSize"][1] = atlasTileSize.y;
			}
		}

		if (ImGui::Button("Save Texture"))
		{
			auto assetmanager = Application::GetAssetManager();
			auto desc = assetmanager->GetDescriptorByID(m_TextureAssetID);

			SharedPtr<TextureAssetInfo> info = DynamicCastPtr<TextureAssetInfo>(desc.assetInfo);
			info->isTilemap = isAtlas;
			info->tileWidth = atlasTileSize.x;
			info->tileHeight = atlasTileSize.y;

			assetmanager->ReloadTexture(m_TextureAssetID);
			EditorLayer::SaveActiveProject();

			auto scene = EditorLayer::GetActiveScene();
			auto view = scene->m_Registry.view<SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto& sprite = view.get<SpriteRendererComponent>(entity);

				sprite.sprite.RecalculateTextureCoords();
			}


		}

		ImGui::End();
	}
	void TexturePreviewPanel::OnClose()
	{
		showPanel = false;
	}

	void TexturePreviewPanel::SetActiveTexture(std::string assetID)
	{
		m_TextureAssetID = assetID;
		auto desc = Application::GetAssetManager()->GetDescriptorByID(assetID);
		m_Texture = Application::GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str());
	}
}