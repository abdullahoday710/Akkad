#pragma once
#include "Material.h"

namespace Akkad {
	class AnimatedSpriteRendererComponentSerializer;
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
			bool IsValid();
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

		class SpriteAnimation
		{
		public:
			std::string spriteSheetAssetID = "";
			std::string name;
			unsigned int startingRow = 0;
			unsigned int startingColumn = 0;
			unsigned int endRow = 0;
			float interval = 25.0f;
			float currentTime = 0.0f;
			float currentRow = 0;

			static SharedPtr<SpriteAnimation> LoadFile(std::string path);
		};

		struct AnimationFrame
		{
			glm::vec2 minTextureCoords = { 0,0 };
			glm::vec2 maxTextureCoords = { 0,0 };
			bool isValid = false;
		};

		class AnimatedSprite {
		public:
			void SetMaterial(std::string filepath);
			void SetSortingLayer(std::string layer) { m_SortingLayer = layer; };
			void SetActiveAnimation(std::string AnimationName) { m_ActiveAnimation = AnimationName; }

			SharedPtr<SpriteAnimation> AddAnimation(std::string assetID);
			SharedPtr<SpriteAnimation> GetAnimation(std::string AnimationName);

			AnimationFrame GetFrame(float deltaTime);

			std::string GetSortingLayer() { return m_SortingLayer; };
			SharedPtr<Material> GetMaterial() { return m_Material; };

		private:
			std::map<std::string, SharedPtr<SpriteAnimation>> m_Animations;
			SharedPtr<Material> m_Material;
			std::string m_SortingLayer;
			std::string m_ActiveAnimation;

			friend class PropertyEditorPanel;
			friend class SpriteAnimationPreviewPanel;
			friend class ::Akkad::AnimatedSpriteRendererComponentSerializer;
		};
	}
}


