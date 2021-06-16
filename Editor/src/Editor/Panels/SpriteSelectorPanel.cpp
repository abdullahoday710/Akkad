#include "SpriteSelectorPanel.h"

#include <Akkad/Graphics/Sprite.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Input/Input.h>
#include <Akkad/ECS/Components/SpriteRendererComponent.h>

#include <imgui.h>
namespace Akkad {
	bool SpriteSelectorPanel::showPanel;

	Entity SpriteSelectorPanel::m_ActiveEntity; // Initilize the reference with an empty sprite.

	SpriteSelectorPanel::SpriteSelectorPanel()
	{
	}

	void SpriteSelectorPanel::SetActiveEntity(Entity entity)
	{
		m_ActiveEntity = entity;
	}

	void SpriteSelectorPanel::DrawImGui()
	{
		if (ImGui::Begin("Sprite Selector", &showPanel))
		{
			if (m_ActiveEntity.IsValid())
			{
				auto& sprite = m_ActiveEntity.GetComponent<SpriteRendererComponent>();
				if (sprite.sprite.GetMaterial() != nullptr)
				{
					if (sprite.sprite.GetMaterial()->isValid())
					{
						glm::vec2 tilemapPreviewSize = { 400,400 };
						auto tilemap = sprite.sprite.GetMaterial()->GetTexture("sprite");
						ImGui::Image((void*)tilemap->GetID(), { tilemapPreviewSize.x, tilemapPreviewSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

						auto rectmin = ImGui::GetItemRectMin();
						auto rectmax = ImGui::GetItemRectMax();
						auto drawlist = ImGui::GetForegroundDrawList();
						drawlist->AddRect(rectmin, rectmax, IM_COL32(255, 255, 0, 255));

						unsigned int total_rows = tilemap->GetDescriptor().Width / tilemap->GetDescriptor().TileWidth;
						unsigned int total_cols = tilemap->GetDescriptor().Height / tilemap->GetDescriptor().TileHeight;

						float originalTileWidth = tilemap->GetDescriptor().TileWidth;
						float originalTileHeight = tilemap->GetDescriptor().TileHeight;

						glm::vec2 TileMapScale = { tilemapPreviewSize.x / tilemap->GetDescriptor().Width,  tilemapPreviewSize.y / tilemap->GetDescriptor().Height };

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
								drawlist->AddRect(currentTileMin, currentTileMax, IM_COL32(255, 255, 0, 255));
								auto input = Application::GetInputManager();

								if (input->GetMouseDown(MouseButtons::LEFT))
								{
									int mouseX = input->GetMouseX();
									int mouseY = input->GetMouseY();

									if (mouseX < currentTileMax.x && mouseY > currentTileMax.y)
									{
										if (mouseX > currentTileMin.x && mouseY < currentTileMin.y)
										{
											sprite.sprite.SetTileRow((float)current_row);
											sprite.sprite.SetTileColoumn((float)current_col);
										}
									}
								}
								currentTileMin.x += scaledTileWidth;
								currentTileMax.x += scaledTileWidth;
							}
						}

					}
				}
			}
		}
			ImGui::End();

	}

	void SpriteSelectorPanel::OnClose()
	{
		showPanel = false;
	}
}