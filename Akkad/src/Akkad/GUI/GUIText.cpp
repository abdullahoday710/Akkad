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
			glm::vec2 current_line_pos;
			current_line_pos.x = GetPosition().x;
			current_line_pos.y = m_Lines.back().yOffset;
			Graphics::Rect current_line_bb;
			glm::vec2 bbmin;
			glm::vec2 bbmax;

			
			for (c = m_Text.begin(); c != m_Text.end(); c++)
			{
				auto& current_line = m_Lines.back();
				auto ftchar = m_Font->GetASCIICharacter(*c, current_line_pos.x, current_line_pos.y);
				
				if (c == m_Text.begin())
				{
					bbmin = ftchar.CharacterRect.GetMin();
				}

				// A terrible way to scale text, but it works lol
				if (std::next(c) == m_Text.end())
				{
					bbmax = ftchar.CharacterRect.GetMax();
					current_line_bb.SetBounds(bbmin, bbmax);
					static bool isScalingDown = false;
					if (current_line_bb.GetMax().x > m_BoundingBox.GetMax().x)
					{
						if (m_Font->GetFontSize() > 12)
						{
							isScalingDown = true;
							SetFontSize(m_Font->GetFontSize() - 1);
							isScalingDown = false;
							break;
						}
					}

					if (current_line_bb.GetMax().y > m_BoundingBox.GetMax().y)
					{
						if (m_Font->GetFontSize() > 12)
						{
							isScalingDown = true;
							SetFontSize(m_Font->GetFontSize() - 1);
							isScalingDown = false;
							break;
						}
					}

					if (current_line_bb.GetMax().x < m_BoundingBox.GetMax().x && !isScalingDown)
					{
						if (m_Font->GetFontSize() < m_OriginalFontSize)
						{
							SetFontSize(m_Font->GetFontSize() + 1);
							break;
						}
					}
				}
				current_line.characters.push_back(ftchar);
			}
		}

		void GUIText::PositionTextKeepFtSize()
		{
			
			std::string::const_iterator c;
			glm::vec2 current_line_pos;
			current_line_pos.x = GetPosition().x;
			current_line_pos.y = m_Lines.back().yOffset;

			for (c = m_Text.begin(); c != m_Text.end(); c++)
			{
				auto& current_line = m_Lines.back();

				auto ftchar = m_Font->GetASCIICharacter(*c, current_line_pos.x, current_line_pos.y);

				if (ftchar.CharacterRect.GetMax().x < m_BoundingBox.GetMax().x)
				{
					current_line.characters.push_back(ftchar);
				}

				else
				{
					TextLine newline;
					newline.yOffset = current_line.yOffset + m_Font->GetFontSize();
					newline.characters.push_back(ftchar);
					current_line_pos.x = GetPosition().x;
					current_line_pos.y = newline.yOffset;
					m_Lines.push_back(newline);
				}
			}
		}

		glm::vec2 GUIText::GetPosition()
		{
			return m_BoundingBox.GetMin();
		}

	}
}

