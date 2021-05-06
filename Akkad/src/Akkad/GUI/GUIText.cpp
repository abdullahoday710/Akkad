#include "GUIText.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/RenderPlatform.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Camera.h"
#include "Akkad/Graphics/Renderer2D.h"

#include <glad/glad.h>

#include <sstream>
namespace Akkad {
	namespace GUI {
		bool GUIText::PushWord(std::string word)
		{
			if (m_Lines.empty())
			{
				Line line;
				line.yOffset = m_Position.y;
				m_Lines.push_back(line);
			}

			auto& currentLine = m_Lines.back();
			unsigned int wordSize = 0;

			std::string::const_iterator c;
			for (c = word.begin(); c != word.end(); c++)
			{
				Font::FontCharacter character = m_Font->GetCharacter(*c);
				wordSize += (character.Advance >> 6) * m_Scale;
			}
			if (m_MaxLineSize > currentLine.size)
			{
				currentLine.text += word;
				currentLine.size += wordSize;
				currentLine.CalculateBoundingBox(m_Font, m_Position);
			}

			else
			{
				Line newLine;
				newLine.size += wordSize;
				newLine.text += word;
				newLine.yOffset = currentLine.yOffset - (m_Font->GetLineSpacing()) * m_Scale;
				newLine.CalculateBoundingBox(m_Font, m_Position);
				m_Lines.push_back(newLine);
			}
			return true;
		}

		void GUIText::SetFont(std::string path)
		{
			m_Font = CreateSharedPtr<Font>(path);
		}

		void GUIText::SetFont(std::string path, unsigned int fontSize)
		{
			m_Font = CreateSharedPtr<Font>(path, fontSize);
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

		void GUIText::SetText(std::string text)
		{
			std::stringstream ss(text);
			std::istream_iterator<std::string> begin(ss);
			std::istream_iterator<std::string> end;
			std::vector<std::string> words(begin, end);
			std::copy(words.begin(), words.end(), std::ostream_iterator<std::string>(ss, "\n"));
			m_Lines.clear();
			for (std::string word : words)
			{
				PushWord(word + " ");
			}

			m_Text = text;
		}

		void GUIText::SetPosition(glm::vec2 position)
		{
			m_Position = position;
			SetText(m_Text);
		}

		void GUIText::Line::CalculateBoundingBox(SharedPtr<Font> font, glm::vec2 textPosition)
		{
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Font::FontCharacter ch = font->GetCharacter(*c);
				float xpos = textPosition.x + ch.Bearing.x;
				float ypos = yOffset - (ch.Size.y - ch.Bearing.y);
				float w = ch.Size.x;
				float h = ch.Size.y;

				if (c == text.begin())
				{
					glm::vec2 min;
					min.x = xpos;
					min.y = ypos + h;
					boundingBox.SetMin(min);
				}
				textPosition.x += (ch.Advance >> 6);
				if (std::next(c) == text.end())
				{
					glm::vec2 max;
					max.x = xpos;
					max.y = ypos + h;
					boundingBox.SetMax(max);
				}
			}
		}
	}
}
