#pragma once
#include "Material.h"

namespace Akkad {
	namespace Graphics {

		class Sprite
		{
		public:
			void SetMaterial(std::string filepath);
			void SetSortingLayer(std::string layer) { m_SortingLayer = layer; };

			void UseSingleTexture(bool use);
			void SetTileRow(float row);
			void SetTileColoumn(float coloumn);
			void SetTileSize(glm::vec2 size);

			std::string GetSortingLayer() { return m_SortingLayer; };
			SharedPtr<Material> GetMaterial() { return m_Material; };

			glm::vec2 GetMinTextureCoords();
			glm::vec2 GetMaxTextureCoords();
			glm::vec2 GetTileSize() { return {m_TileWidth, m_TileHeight}; }
			float GetTileRow() { return m_TileRow; };
			float GetTileColoumn() { return m_TileColoumn; };
			bool IsSingleTexture() { return m_IsSingleTexture; };
		private:
			SharedPtr<Material> m_Material;
			std::string m_SortingLayer;

			bool m_IsSingleTexture = false;
			float m_TileRow = 0;
			float m_TileColoumn = 0;
			float m_TileWidth = 0.0f;
			float m_TileHeight = 0.0f;

			void RecalculateTextureCoords();

			glm::vec2 m_MinTextureCoords;
			glm::vec2 m_MaxTextureCoords;
		};
	}
}


