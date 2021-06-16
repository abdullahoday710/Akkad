#include "Sprite.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

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
			if (m_Material->HasTexture("main_sprite_tex"))
			{
				return m_Material->GetTexture("main_sprite_tex")->GetDescriptor().IsTilemap;
			}
			
			AK_WARNING("Sprite material must have a porperty named 'main_sprite_tex' for the main sprite texture !");
		}

		bool Sprite::IsValid()
		{
			if (m_Material != nullptr)
			{
				if (m_Material->isValid())
				{
					return true;
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
				if (!m_Material->HasTexture("main_sprite_tex"))
				{
					AK_WARNING("Sprite material must have a porperty named 'main_sprite_tex' for the main sprite texture !");
					return;
				}
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
}