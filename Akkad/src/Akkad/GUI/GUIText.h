#pragma once
#include "Font.h"
#include "GUIRect.h"

#include "Akkad/core.h"

#include <string>
#include <vector>

namespace Akkad {

	namespace GUI {

		class GUIText
		{
		public:
			enum class FittingMode {
				SCALE_TO_FIT, KEEP_FONT_SIZE
			};

			enum class Alignment {
				LEFT, CENTER
			};

			struct TextLine {
				float yOffset;
				std::vector<Font::FontCharacter> characters;
				GUIRect boundingBox;
			};

			void SetFont(std::string filepath);
			void SetText(std::string text);
			void SetBoundingBox(Graphics::Rect boundingBox);
			void SetColor(glm::vec3 color) { m_Color = color; };
			
			Graphics::Rect GetBoundingBox() { return m_BoundingBox; }
			SharedPtr<Font> GetFont() { return m_Font; }
			std::string GetText() { return m_Text; }
			std::vector<TextLine> GetLines() { return m_Lines; }
			glm::vec3 GetColor() { return m_Color; }
			glm::vec2 GetPosition();
			bool IsValid();

			unsigned int GetOriginalFontSize() { return m_OriginalFontSize; }
			void SetOriginalFontSize(unsigned int OriginalFontSize);
		private:
			void RecalculateTextPosition();
			void PositionTextScaleToFit();
			void PositionTextKeepFtSize();
			void SetFontSize(unsigned int sizePixels);

			FittingMode m_FittingMode = FittingMode::KEEP_FONT_SIZE;
			Alignment m_Alignment = Alignment::LEFT;
			unsigned int m_OriginalFontSize = 64;
			SharedPtr<Font> m_Font;
			std::string m_Text;
			std::string m_FontFilePath;
			std::vector<TextLine> m_Lines;
			Graphics::Rect m_BoundingBox;
			
			glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };

			friend class Scene;

		};
	}
}

