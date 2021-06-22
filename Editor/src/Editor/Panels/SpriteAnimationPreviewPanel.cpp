#pragma once
#include "SpriteAnimationPreviewPanel.h"

#include "Editor/Serializers/SpriteAnimationSerializer.h"
#include "Editor/EditorLayer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/Graphics/Sprite.h>
#include <Akkad/Logging.h>
#include <Akkad/Input/Input.h>
#include <Akkad/ECS/Components/SpriteRendererComponent.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>


namespace Akkad {
	bool SpriteAnimationPreviewPanel::showPanel;
	std::string SpriteAnimationPreviewPanel::m_AnimationAssetID;
	SharedPtr<Graphics::SpriteAnimation> SpriteAnimationPreviewPanel::m_Animation;
	SharedPtr<Graphics::Texture> SpriteAnimationPreviewPanel::m_AnimationSpriteSheet;

	void SpriteAnimationPreviewPanel::DrawImGui()
	{
		if (ImGui::Begin("Sprite Animation Preview", &showPanel))
		{
			std::string label = "Editing animation : " + m_Animation->name;
			ImGui::Text(label.c_str());

			if (m_Animation->spriteSheetAssetID.empty())
			{
				std::string buf;
				ImGui::InputText("Sprite sheet", &buf, ImGuiInputTextFlags_ReadOnly);
			}

			else
			{
				ImGui::Text("Left click on the starting tile of the animation, this is the first frame");
				ImGui::Text("Then, Right click on the end tile of the animation, this is the last frame");
				ImGui::Text("All frames in-between will be automatically selected");
				glm::vec2 tilemapPreviewSize = { 512, 512 };
				ImGui::Image((void*)m_AnimationSpriteSheet->GetID(), { tilemapPreviewSize.x,tilemapPreviewSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				auto rectmin = ImGui::GetItemRectMin();
				auto rectmax = ImGui::GetItemRectMax();
				auto drawlist = ImGui::GetForegroundDrawList();
				drawlist->AddRect(rectmin, rectmax, IM_COL32(255, 255, 0, 255));

				unsigned int total_rows = m_AnimationSpriteSheet->GetDescriptor().Width / m_AnimationSpriteSheet->GetDescriptor().TileWidth;
				unsigned int total_cols = m_AnimationSpriteSheet->GetDescriptor().Height / m_AnimationSpriteSheet->GetDescriptor().TileHeight;

				float originalTileWidth = m_AnimationSpriteSheet->GetDescriptor().TileWidth;
				float originalTileHeight = m_AnimationSpriteSheet->GetDescriptor().TileHeight;

				glm::vec2 TileMapScale = { tilemapPreviewSize.x / m_AnimationSpriteSheet->GetDescriptor().Width,  tilemapPreviewSize.y / m_AnimationSpriteSheet->GetDescriptor().Height };

				float scaledTileWidth = originalTileWidth * TileMapScale.x;
				float scaledTileHeight = originalTileHeight * TileMapScale.y;

				ImVec2 originalTileMin = { rectmin.x, rectmax.y };
				ImVec2 originalTileMax = { rectmin.x + scaledTileWidth, rectmax.y - scaledTileHeight };

				ImVec2 currentTileMin = originalTileMin;
				ImVec2 currentTileMax = originalTileMax;

				for (size_t current_col = 0; current_col < total_cols; current_col++)
				{
					if (current_col != 0)
					{
						currentTileMin.y -= scaledTileHeight;
						currentTileMax.y -= scaledTileHeight;
					}

					currentTileMin.x = originalTileMin.x;
					currentTileMax.x = originalTileMax.x;
					for (size_t current_row = 0; current_row < total_rows; current_row++)
					{
						if (current_row == m_Animation->startingRow && current_col == m_Animation->startingColumn)
						{
							drawlist->AddRect(currentTileMin, currentTileMax, IM_COL32(0, 255, 0, 255));
						}
						else if (current_row == m_Animation->endRow && current_col == m_Animation->startingColumn)
						{
							drawlist->AddRect(currentTileMin, currentTileMax, IM_COL32(255, 0, 0, 255));
						}
						else if (current_row > m_Animation->startingRow && current_row < m_Animation->endRow && current_col == m_Animation->startingColumn)
						{
							drawlist->AddRect(currentTileMin, currentTileMax, IM_COL32(0, 0, 255, 255));
						}
						else
						{
							drawlist->AddRect(currentTileMin, currentTileMax, IM_COL32(255, 255, 0, 255));
						}

						auto input = Application::GetInputManager();

						if (input->GetMouseDown(MouseButtons::LEFT))
						{
							int mouseX = input->GetMouseX();
							int mouseY = input->GetMouseY();

							if (mouseX < currentTileMax.x && mouseY > currentTileMax.y)
							{
								if (mouseX > currentTileMin.x && mouseY < currentTileMin.y)
								{
									m_Animation->startingRow = current_row;
									m_Animation->startingColumn = current_col;
								}
							}
						}

						if (input->GetMouseDown(MouseButtons::RIGHT))
						{
							int mouseX = input->GetMouseX();
							int mouseY = input->GetMouseY();

							if (mouseX < currentTileMax.x && mouseY > currentTileMax.y)
							{
								if (mouseX > currentTileMin.x && mouseY < currentTileMin.y)
								{
									if (current_col == m_Animation->startingColumn)
									{
										if (m_Animation->startingRow != current_row)
										{
											m_Animation->endRow = current_row;
										}
									}
								}
							}
						}

						currentTileMin.x += scaledTileWidth;
						currentTileMax.x += scaledTileWidth;
					}
				}
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::TEXTURE)
					{
						m_Animation->spriteSheetAssetID = desc.assetID;
						auto info = DynamicCastPtr<TextureAssetInfo>(desc.assetInfo);
						
						if (info->isTilemap)
						{
							m_AnimationSpriteSheet = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str(), info->tileWidth, info->tileHeight);

						}

						else
						{
							AK_ERROR("Can't set texture for animation because it is not set as a tile map !");
						}
					}
				}
				ImGui::EndDragDropTarget();
			}
			
			
			ImGui::InputFloat("Interval", &m_Animation->interval);

			if (ImGui::Button("Save animation"))
			{
				auto desc = Application::GetAssetManager()->GetDescriptorByID(m_AnimationAssetID);
				SpriteAnimationSerializer::Serialize(m_Animation, m_Animation->name, desc.absolutePath);

				auto scene = EditorLayer::GetActiveScene();
				auto view = scene->m_Registry.view<AnimatedSpriteRendererComponent>();

				for (auto entity : view)
				{
					auto& animatedSprite = view.get<AnimatedSpriteRendererComponent>(entity);
					
					if (animatedSprite.sprite.m_Animations.count(m_Animation->name) > 0)
					{
						animatedSprite.sprite.m_Animations[m_Animation->name] = CreateSharedPtr<Graphics::SpriteAnimation>(*m_Animation);
					}
				}
			}
		}
		ImGui::End();
	}

	void SpriteAnimationPreviewPanel::OnClose()
	{
	}

	void SpriteAnimationPreviewPanel::SetActiveAnimation(SharedPtr<Graphics::SpriteAnimation> animation, std::string assetID)
	{
		m_Animation = animation;
		m_AnimationAssetID = assetID;
		if (!animation->spriteSheetAssetID.empty())
		{
			auto desc = Application::GetAssetManager()->GetDescriptorByID(animation->spriteSheetAssetID);
			auto info = DynamicCastPtr<TextureAssetInfo>(desc.assetInfo);

			if (info->isTilemap)
			{
				m_AnimationSpriteSheet = Application::GetInstance().GetRenderPlatform()->CreateTexture(desc.absolutePath.c_str(), info->tileWidth, info->tileHeight);

			}

			else
			{
				AK_ERROR("Can't set texture for animation because it is not set as a tile map !");
			}
		}

		auto desc = Application::GetAssetManager()->GetDescriptorByID(assetID);
		
	}

}