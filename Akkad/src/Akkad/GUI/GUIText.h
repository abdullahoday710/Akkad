#pragma once
#include "Font.h"

#include "Akkad/core.h"
#include "Akkad/Graphics/Rect.h"

#include <string>
#include <vector>

namespace Akkad {

	namespace GUI {

		class GUIText
		{
		public:
			struct Line {
				std::string text = "";
				unsigned int size = 0;
				float yOffset = 0.0f;
				Graphics::Rect boundingBox = Graphics::Rect();
				void CalculateBoundingBox(SharedPtr<Font> font, glm::vec2 textPosition);
			};

			GUIText() {}
			GUIText(SharedPtr<Font> font)
			{
				m_Font = font;
			}
			GUIText(std::string text, SharedPtr<Font> font) {
				m_Font = font;
			}

			void SetFont(std::string path);
			void SetFont(std::string path, unsigned int fontSize);
			void SetFont(SharedPtr<Font> font) { m_Font = font; }
			void SetBoundingBox(Graphics::Rect box) { m_BoundingBox = box; }

			void SetText(std::string text);
			void RecalculateText();

			SharedPtr<Font> GetFont() { return m_Font; }
			bool IsValid();

			glm::vec2 GetPosition() { return m_BoundingBox.GetMin(); }
			std::string GetText() { return m_Text; }
			std::vector<Line>& GetLines() { return m_Lines; };


		private:
			SharedPtr<Font> m_Font;
			Graphics::Rect m_BoundingBox;

			std::string m_Text = "";
			std::vector<Line> m_Lines;
		};
	}
}

