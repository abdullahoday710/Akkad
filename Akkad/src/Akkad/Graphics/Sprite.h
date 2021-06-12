#pragma once
#include "Material.h"

namespace Akkad {
	namespace Graphics {

		class Sprite
		{
		public:
			void SetMaterial(std::string filepath);
			void SetSortingLayer(std::string layer) { m_SortingLayer = layer; };

			void SetTileRow(float row);
			void SetTileColoumn(float coloumn);

			std::string GetSortingLayer() { return m_SortingLayer; };
			SharedPtr<Material> GetMaterial() { return m_Material; };

			glm::vec2 GetMinTextureCoords();
			glm::vec2 GetMaxTextureCoords();
			float GetTileRow() { return m_TileRow; };
			float GetTileColoumn() { return m_TileColoumn; };
			bool IsUsingTilemap();
		private:
			SharedPtr<Material> m_Material;
			std::string m_SortingLayer;

			float m_TileRow = 0;
			float m_TileColoumn = 0;

			void RecalculateTextureCoords();

			glm::vec2 m_MinTextureCoords;
			glm::vec2 m_MaxTextureCoords;
			friend class SpriteRendererComponentSerializer;
			friend class TexturePreviewPanel;
		};
	}
}


