#include "GUIText.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/RenderPlatform.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Camera.h"
#include "Akkad/Graphics/Renderer2D.h"

#include <glad/glad.h>

#include <sstream>
#include <iostream>
namespace Akkad {
	namespace GUI {

		void GUIText::SetFont(std::string path)
		{
			m_Font = CreateSharedPtr<Font>(path);
		}

		void GUIText::SetFont(std::string path, unsigned int fontSize)
		{
			m_Font = CreateSharedPtr<Font>(path, fontSize);
		}

		void GUIText::SetText(std::string text)
		{
			m_Text = text;
			RecalculateText();
		}

		void GUIText::RecalculateText()
		{
			if (IsValid())
			{
				m_Lines.clear();
				auto maxLineSize = m_BoundingBox.GetWidth();

				if (m_Lines.empty())
				{
					Line first_line;

					// top left of the bounding box
					first_line.yOffset = m_BoundingBox.GetMin().y + m_Font->GetLineSpacing();

					m_Lines.push_back(first_line);
				}

				std::string::const_iterator c;
				for (c = m_Text.begin(); c != m_Text.end(); c++)
				{
					auto& currentLine = m_Lines.back();
					Font::FontCharacter ch = m_Font->GetCharacter(*c);
					float advance = (ch.Advance >> 6);
					if (maxLineSize > (currentLine.size + advance))
					{
						currentLine.text += *(c);
						currentLine.size += advance;
						currentLine.CalculateBoundingBox(m_Font, m_BoundingBox.GetMin());
					}

					else
					{
						Line newLine;
						newLine.yOffset = currentLine.yOffset + m_Font->GetLineSpacing();
						if (newLine.yOffset < m_BoundingBox.GetMax().y)
						{
							newLine.text += *(c);
							newLine.size += (ch.Advance >> 6);

							newLine.CalculateBoundingBox(m_Font, m_BoundingBox.GetMin());
							m_Lines.push_back(newLine);
						}

					}

				}
			}
			
		}

		bool GUIText::IsValid()
		{
			if (m_Font == nullptr)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		void GUIText::Line::CalculateBoundingBox(SharedPtr<Font> font, glm::vec2 textPosition)
		{
			std::string::const_iterator c;
			float line_height = 0.0f;
			for (c = text.begin(); c != text.end(); c++)
			{
				Font::FontCharacter ch = font->GetCharacter(*c);

				float xpos = textPosition.x + ch.Bearing.x;
				float ypos = yOffset + (ch.Size.y - ch.Bearing.y);
				float w = ch.Size.x;
				float h = ch.Size.y;

				if (h > line_height)
				{
					line_height = h;
				}
				if (c == text.begin())
				{
					glm::vec2 min;
					min.x = xpos;
					min.y = ypos;
					boundingBox.SetMin(min);
				}

				textPosition.x += (ch.Advance >> 6);
				if (std::next(c) == text.end())
				{
					glm::vec2 max;
					max.x = xpos + (ch.Advance >> 6);
					max.y = ypos - line_height;
					boundingBox.SetMax(max);
				}
			}
		}
	}
}

