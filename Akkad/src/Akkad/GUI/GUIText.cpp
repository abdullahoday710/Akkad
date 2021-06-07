#include "GUIText.h"

#include <glad/glad.h>

#include <sstream>
#include <iostream>
namespace Akkad {
	namespace GUI {

		void GUIText::SetFont(std::string filepath)
		{
			m_Font = CreateSharedPtr<Font>(filepath);
			m_FontFilePath = filepath;
		}

		void GUIText::SetFontSize(unsigned int sizePixels)
		{
			if (IsValid())
			{
				m_Font->SetFontPixelScaling(sizePixels);
				RecalculateTextPosition();
			}
		}

		void GUIText::SetText(std::string text)
		{
			if (m_Text != text)
			{
				m_Text = text;
				RecalculateTextPosition();
			}
		}

		void GUIText::SetBoundingBox(Graphics::Rect boundingBox)
		{
			if (boundingBox != m_BoundingBox)
			{
				m_BoundingBox = boundingBox;
				RecalculateTextPosition();
			}
		}

		bool GUIText::IsValid()
		{
			if (m_Font != nullptr)
			{
				return true;
			}
			return false;
		}

		void GUIText::SetOriginalFontSize(unsigned int OriginalFontSize)
		{
			m_OriginalFontSize = OriginalFontSize;
			SetFontSize(OriginalFontSize);
		}

		void GUIText::RecalculateTextPosition()
		{
			m_Lines.clear();
			TextLine firstLine;
			firstLine.yOffset = GetPosition().y + (m_Font->GetFontSize() / 1.5);
			m_Lines.push_back(firstLine);
			switch (m_FittingMode)
			{
			case GUIText::FittingMode::SCALE_TO_FIT:
			{
				PositionTextScaleToFit();
				break;
			}
			case GUIText::FittingMode::KEEP_FONT_SIZE:
			{
				PositionTextKeepFtSize();
				break;
			}
			default:
				break;
			}
		}

		void GUIText::PositionTextScaleToFit()
		{

			std::string::const_iterator c;

			auto& current_line = m_Lines.back();
			current_line.boundingBox.SetParent(m_BoundingBox);
			current_line.boundingBox.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
			current_line.boundingBox.SetYConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0 });

			current_line.boundingBox.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 1 });
			current_line.boundingBox.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.15 });

			float current_line_height = current_line.boundingBox.GetRect().GetHeight();
			if (current_line_height > 12)
			{
				m_Font->SetFontPixelScaling(current_line.boundingBox.GetRect().GetHeight());
			}

			glm::vec2 current_line_pos;
			current_line_pos.x = current_line.boundingBox.GetRect().GetMin().x;
			current_line_pos.y = m_Lines.back().yOffset;
			for (c = m_Text.begin(); c != m_Text.end(); c++)
			{
				auto ftchar = m_Font->GetASCIICharacter(*c, current_line_pos.x, current_line_pos.y);
				current_line.characters.push_back(ftchar);
			}
			
			current_line.ApplyAlignment(m_Alignment, m_Font);

		}

		void GUIText::PositionTextKeepFtSize()
		{
			std::string::const_iterator c;
			glm::vec2 current_line_pos;
			current_line_pos.x = GetPosition().x;
			current_line_pos.y = m_Lines.back().yOffset;

			m_Lines.back().boundingBox.SetParent(m_BoundingBox);
			m_Lines.back().boundingBox.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 1 });
			m_Lines.back().boundingBox.SetHeightConstraint({ ConstraintType::PIXEL_CONSTRAINT, (float)m_OriginalFontSize });
			m_Lines.back().boundingBox.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
			m_Lines.back().boundingBox.SetYConstraint({ ConstraintType::PIXEL_CONSTRAINT, current_line_pos.y - m_Lines.back().boundingBox.GetRect().GetHeight() /1.5f});


			for (c = m_Text.begin(); c != m_Text.end(); c++)
			{
				auto& current_line = m_Lines.back();

				auto ftchar = m_Font->GetASCIICharacter(*c, current_line_pos.x, current_line_pos.y);

				if (ftchar.CharacterRect.GetMax().x < m_BoundingBox.GetMax().x)
				{
					current_line.characters.push_back(ftchar);
					current_line.ApplyAlignment(m_Alignment, m_Font);
				}

				else
				{
					TextLine newline;
					newline.yOffset = current_line.yOffset + m_Font->GetFontSize();
					if (newline.yOffset < m_BoundingBox.GetMax().y)
					{
						current_line_pos.x = GetPosition().x;
						current_line_pos.y = newline.yOffset;

						newline.boundingBox.SetParent(m_BoundingBox);

						newline.boundingBox.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 1 });
						newline.boundingBox.SetHeightConstraint({ ConstraintType::PIXEL_CONSTRAINT, (float)m_OriginalFontSize });

						newline.boundingBox.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
						newline.boundingBox.SetYConstraint({ ConstraintType::PIXEL_CONSTRAINT, current_line_pos.y - newline.boundingBox.GetRect().GetHeight() / 1.5f });
						newline.characters.push_back(ftchar);

						newline.ApplyAlignment(m_Alignment, m_Font);
						m_Lines.push_back(newline);
					}
				
				}
			}
		}

		glm::vec2 GUIText::GetPosition()
		{
			return m_BoundingBox.GetMin();
		}

		void GUIText::TextLine::ApplyAlignment(Alignment alignment, SharedPtr<Font> font)
		{
			glm::vec2 current_line_pos{};
			
			switch (alignment)
			{
			case Akkad::GUI::GUIText::Alignment::LEFT:
			{
				current_line_pos.x = boundingBox.GetRect().GetMin().x;
				current_line_pos.y = yOffset;

				for (auto& character : characters)
				{
					character = font->GetASCIICharacter(character.character, current_line_pos.x, current_line_pos.y);
				}
				break;
			}
			case Akkad::GUI::GUIText::Alignment::CENTER:
			{
				float line_width = 0.0f;
				for (auto& ftchar : characters)
				{
					line_width += ftchar.CharacterRect.GetWidth();
				}

				current_line_pos.x = boundingBox.GetParentRect().GetPosition().x - (line_width / 2);
				current_line_pos.y = yOffset;

				for (auto& character : characters)
				{
					character = font->GetASCIICharacter(character.character, current_line_pos.x, current_line_pos.y);
				}
				break;
			}
			default:
				break;
			}
		}

}
}

