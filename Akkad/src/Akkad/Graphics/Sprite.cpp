#include "Sprite.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

namespace Akkad {
	namespace Graphics {
		void Sprite::SetMaterial(std::string filepath)
		{
			m_Material = Material::LoadFile(filepath);
		}

		void Sprite::UseSingleTexture(bool use)
		{
			m_IsSingleTexture = use;
			RecalculateTextureCoords();
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

		void Sprite::SetTileSize(glm::vec2 size)
		{
			m_TileWidth = size.x;
			m_TileHeight = size.y;
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

		void Sprite::RecalculateTextureCoords()
		{
			glm::vec2 mincoords;
			float atlasWidth = (float)m_Material->GetTexture("sprite")->GetDescriptor().Width;
			float atlasHeight = (float)m_Material->GetTexture("sprite")->GetDescriptor().Height;
			if (m_IsSingleTexture)
			{
				m_MinTextureCoords = { 0,0 };
				m_MaxTextureCoords = { 1,1 };
			}
			else
			{
				// coords of the bottom left
				m_MinTextureCoords.x = (m_TileRow * m_TileWidth) / atlasWidth;
				m_MinTextureCoords.y = (m_TileColoumn * m_TileHeight) / atlasHeight;

				// coords of the top right
				m_MaxTextureCoords.x = ((m_TileRow + 1) * m_TileWidth) / atlasWidth;
				m_MaxTextureCoords.y = ((m_TileColoumn + 1) * m_TileHeight) / atlasHeight;
			}
		}
	}
}