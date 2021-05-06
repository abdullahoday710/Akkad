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
			GUIText() {}
			GUIText(SharedPtr<Font> font)
			{
				m_Font = font;
			}
			GUIText(std::string text, SharedPtr<Font> font) {
				m_Font = font;
				SetText(text);
			}

			void SetFont(std::string path);
			void SetFont(std::string path, unsigned int fontSize);
			void SetFont(SharedPtr<Font> font) { m_Font = font; }
			bool IsValid();
			SharedPtr<Font> GetFont() { return m_Font; }


			struct Line {
				std::string text = "";
				unsigned int size = 0; // the size of the text line (in pixels) relative to screen space
				float yOffset = 0.0f;
				Graphics::Rect boundingBox = Graphics::Rect();
				void CalculateBoundingBox(SharedPtr<Font> font, glm::vec2 textPosition);
			};
			void SetText(std::string text);
			void SetPosition(glm::vec2 position);
			glm::vec2 GetPosition() { return m_Position; }
			unsigned int m_MaxLineSize = 1000;

			std::string m_Text;
			std::vector<Line>& GetLines() { return m_Lines; };
		private:
			bool PushWord(std::string word);
			std::vector<Line> m_Lines;
			SharedPtr<Font> m_Font;
			glm::vec2 m_Position = {0, 0};
			float m_Scale = 1.0f;
		};
	}
}

