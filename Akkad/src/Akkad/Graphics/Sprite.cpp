#include "Sprite.h"

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
			return m_Material->GetTexture("sprite")->GetDescriptor().IsTilemap;
		}

		void Sprite::RecalculateTextureCoords()
		{
			float atlasWidth = (float)m_Material->GetTexture("sprite")->GetDescriptor().Width;
			float atlasHeight = (float)m_Material->GetTexture("sprite")->GetDescriptor().Height;
			if (!IsUsingTilemap())
			{
				m_MinTextureCoords = { 0,0 };
				m_MaxTextureCoords = { 1,1 };
			}
			else
			{
				auto desc = m_Material->GetTexture("sprite")->GetDescriptor();
				
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