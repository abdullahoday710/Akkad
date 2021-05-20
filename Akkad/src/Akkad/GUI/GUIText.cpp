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

		void GUIText::SetBoundingBox(Graphics::Rect box)
		{
			if (box.GetMin() != m_BoundingBox.GetMin() && box.GetMax() != m_BoundingBox.GetMax())
			{
				m_BoundingBox = box;
				RecalculateText();
			}
		}

		void GUIText::SetAlignment(Alignment alignment)
		{
			m_Alignment = alignment;
			RecalculateText();
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
					first_line.yOffset = m_BoundingBox.GetMin().y;

					m_Lines.push_back(first_line);
				}

				std::string::const_iterator c;
				float line_height = 0.0f;
				for (c = m_Text.begin(); c != m_Text.end(); c++)
				{
					auto& currentLine = m_Lines.back();
					Font::FontCharacter ch = m_Font->GetCharacter(*c);
					float advance = (ch.Advance >> 6);

					if (ch.Size.y > line_height)
					{
						line_height = ch.Size.y;
					}

					if (maxLineSize > (currentLine.size + advance))
					{
						if (currentLine.yOffset == m_Lines.front().yOffset)
						{
							currentLine.yOffset = m_BoundingBox.GetMin().y + line_height;
						}

						currentLine.text += *(c);
						currentLine.size += advance;
						currentLine.CalculateBoundingBox(m_Font, GetPosition());

						if (m_Alignment == Alignment::CENTER)
						{
							currentLine.boundingBox.SetX(GetPosition().x);
						}

					}

					else
					{
						Line newLine;
						newLine.yOffset = currentLine.yOffset + m_Font->GetLineSpacing();

						newLine.text += *(c);
						newLine.size += (ch.Advance >> 6);

						newLine.CalculateBoundingBox(m_Font, GetPosition());

						if (m_Alignment == Alignment::CENTER)
						{
							currentLine.boundingBox.SetX(GetPosition().x);
						}

						if ((newLine.boundingBox.GetMax().y + ch.Size.y) < m_BoundingBox.GetMax().y)
						{
							m_Lines.push_back(newLine);
						}

						else
						{
							break;
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

		glm::vec2 GUIText::GetPosition()
		{
			switch (m_Alignment)
			{
			case GUIText::Alignment::LEFT:
				return m_BoundingBox.GetMin();
			case GUIText::Alignment::CENTER:
			{
				glm::vec2 position;
				position.x = m_BoundingBox.GetPosition().x;
				position.y = m_BoundingBox.GetMin().y;
				return position;
			}
			}
		}

		void GUIText::Line::CalculateBoundingBox(SharedPtr<Font> font, glm::vec2 textPosition)
		{
			float line_height = 0.0f;
			glm::vec2 min;
			glm::vec2 max;

			std::string::const_iterator c;
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
					min.x = xpos;
					min.y = ypos;
				}

				textPosition.x += (ch.Advance >> 6);
				if (std::next(c) == text.end())
				{
					
					max.x = xpos + (ch.Advance >> 6);
					max.y = ypos - line_height;
					boundingBox.SetBounds(min, max);
				}
			}
		}
	}
}

