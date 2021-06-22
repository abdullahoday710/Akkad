#include "Sprite.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

#include <json.hpp>
#include <fstream>

namespace Akkad {
	namespace Graphics {
		void Sprite::SetMaterial(std::string filepath)
		{
			m_Material = Material::LoadFile(filepath);
		}

		void Sprite::SetTileRow(float row)
		{
			m_TileRow = row;
			RecalculateTextureCoords();
		}

		void Sprite::SetTileColoumn(float coloumn)
		{
			m_TileColoumn = coloumn;
			RecalculateTextureCoords();
		}

		glm::vec2 Sprite::GetMinTextureCoords()
		{
			return m_MinTextureCoords;
		}

		glm::vec2 Sprite::GetMaxTextureCoords()
		{
			return m_MaxTextureCoords;
		}

		bool Sprite::IsUsingTilemap()
		{
			if (IsValid())
			{
				return m_Material->GetTexture("main_sprite_tex")->GetDescriptor().IsTilemap;
			}

			return false;
		}

		bool Sprite::IsValid()
		{
			if (m_Material != nullptr)
			{
				if (m_Material->isValid())
				{
					if (m_Material->HasTexture("main_sprite_tex"))
					{
						return true;
					}
					AK_WARNING("Sprite material must have a porperty named 'main_sprite_tex' for the main sprite texture !");
				}
			}

			return false;
		}
		void Sprite::RecalculateTextureCoords()
		{
			if (!IsUsingTilemap())
			{
				m_MinTextureCoords = { 0,0 };
				m_MaxTextureCoords = { 1,1 };
			}
			else
			{
				if (IsValid())
				{
					auto desc = m_Material->GetTexture("main_sprite_tex")->GetDescriptor();
					float atlasWidth = desc.Width;
					float atlasHeight = desc.Height;

					// coords of the bottom left
					m_MinTextureCoords.x = (m_TileRow * desc.TileWidth) / atlasWidth;
					m_MinTextureCoords.y = (m_TileColoumn * desc.TileHeight) / atlasHeight;

					// coords of the top right
					m_MaxTextureCoords.x = ((m_TileRow + 1) * desc.TileWidth) / atlasWidth;
					m_MaxTextureCoords.y = ((m_TileColoumn + 1) * desc.TileHeight) / atlasHeight;
				}
			}
		}

		void AnimatedSprite::SetMaterial(std::string filepath)
		{
			m_Material = Material::LoadFile(filepath);
		}

		SharedPtr<SpriteAnimation> AnimatedSprite::AddAnimation(std::string assetID)
		{
			auto desc = Application::GetAssetManager()->GetDescriptorByID(assetID);
			auto animation = Graphics::SpriteAnimation::LoadFile(desc.absolutePath);
			m_Animations[animation->name] = animation;

			return m_Animations[animation->name];
		}

		SharedPtr<SpriteAnimation> AnimatedSprite::GetAnimation(std::string AnimationName)
		{
			return m_Animations[AnimationName];
		}

		AnimationFrame AnimatedSprite::GetFrame(float deltaTime)
		{
			AnimationFrame frame;
			
			if (m_Animations.count(m_ActiveAnimation) > 0)
			{
				frame.isValid = true;
				
				auto& activeAnimation = m_Animations[m_ActiveAnimation];

				activeAnimation->currentTime += deltaTime;

				if (activeAnimation->currentTime >= (activeAnimation->interval / 1000))
				{
					activeAnimation->currentTime = 0;
					activeAnimation->currentRow += 1;
				}

				if (activeAnimation->currentRow >= activeAnimation->endRow)
				{
					activeAnimation->currentRow = activeAnimation->startingRow;
				}

				if (!activeAnimation->spriteSheetAssetID.empty())
				{
					m_Material->SetTexture("main_sprite_tex", activeAnimation->spriteSheetAssetID);
					auto desc = m_Material->GetTexture("main_sprite_tex")->GetDescriptor();

					float atlasWidth = desc.Width;
					float atlasHeight = desc.Height;

					// coords of the bottom left
					frame.minTextureCoords.x = (activeAnimation->currentRow * desc.TileWidth) / atlasWidth;
					frame.minTextureCoords.y = (activeAnimation->startingColumn * desc.TileHeight) / atlasHeight;

					// coords of the top right
					frame.maxTextureCoords.x = ((activeAnimation->currentRow + 1) * desc.TileWidth) / atlasWidth;
					frame.maxTextureCoords.y = ((activeAnimation->startingColumn + 1) * desc.TileHeight) / atlasHeight;
				}
			}

			return frame;
		}

		SharedPtr<SpriteAnimation> SpriteAnimation::LoadFile(std::string path)
		{
				std::ifstream file;
				file.open(path);
				nlohmann::json data;
				file >> data;

				SharedPtr<SpriteAnimation> animation = CreateSharedPtr<SpriteAnimation>();

				animation->spriteSheetAssetID = data["SpriteSheetID"];
				animation->name = data["Name"];
				animation->startingRow = data["StartingRow"];
				animation->startingColumn = data["StartingColumn"];
				animation->endRow = data["EndRow"];
				animation->interval = data["Interval"];

				return animation;
		}
}
}