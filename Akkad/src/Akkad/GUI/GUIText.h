#pragma once
#include "Font.h"

#include "Akkad/core.h"

#include <string>
#include <vector>

namespace Akkad {

	namespace GUI {

		class GUIText
		{
		public:
			void SetFont(std::string filepath);
			void SetFontSize(unsigned int sizePixels);
			void SetText(std::string text) { m_Text = text; };
			void SetBoundingBox(Graphics::Rect boundingBox);
			void SetColor(glm::vec3 color) { m_Color = color; };
			
			Graphics::Rect GetBoundingBox() { return m_BoundingBox; }
			SharedPtr<Font> GetFont() { return m_Font; }
			std::string GetText() { return m_Text; }
			glm::vec3 GetColor() { return m_Color; }
			bool IsValid();
		private:
			SharedPtr<Font> m_Font;
			std::string m_Text;
			Graphics::Rect m_BoundingBox;
			glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
		};
	}
}

