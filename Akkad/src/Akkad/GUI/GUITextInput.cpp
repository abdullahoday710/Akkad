#include "GUITextInput.h"

#include "Akkad/Logging.h"
#include <ctype.h>

namespace Akkad {
	namespace GUI {

		void GUITextInput::SetTextInputColor(glm::vec3 color)
		{
			m_TextInputColor = color;
		}

		void GUITextInput::SetTextColor(glm::vec3 color)
		{
			m_TextColor = color;
			m_uitext.SetColor(color);
		}

		void GUITextInput::SetTextInputRect(GUIRect rect)
		{
			m_TextInputRect = rect;
			if (m_uitext.GetFittingMode() != GUIText::FittingMode::SCALE_TO_FIT)
			{
				m_uitext.SetFittingMode(GUIText::FittingMode::SCALE_TO_FIT);
			}
			m_uitext.m_BoundingBox.SetParent(rect.GetRect());
			m_uitext.m_BoundingBox.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0.9 });
			m_uitext.m_BoundingBox.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.1 });
			m_uitext.m_BoundingBox.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0});
			m_uitext.m_BoundingBox.SetYConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
			m_uitext.SetOriginalFontSize(64);
			m_uitext.SetColor(m_TextColor);
			SetTextAlignment(GUIText::Alignment::CENTER);
			m_uitext.RecalculateTextPosition();
		}

		void GUITextInput::SetTextAlignment(GUIText::Alignment alignment)
		{
			m_uitext.SetAlignment(alignment);
		}

		void GUITextInput::AddCharacter(char characater)
		{

			if (m_Flags & GUITextInputFlags::PasswordField)
			{
				m_TextValue += characater;
				std::string stars;
				for (size_t i = 0; i < m_TextValue.size(); i++)
				{
					stars += "*";
				}
				m_uitext.SetText(stars);
			}
			if (m_Flags & GUITextInputFlags::NumbersOnly)
			{
				if (characater == '-' && m_TextValue.size() == 0)
				{
					m_TextValue += characater;
					m_uitext.SetText(m_TextValue);
				}
				if (isdigit(characater))
				{
					m_TextValue += characater;
					m_uitext.SetText(m_TextValue);
				}
			}
			else
			{
				m_uitext.SetText(m_TextValue);
			}
		}

		void GUITextInput::SetText(std::string text)
		{
			m_TextValue = text;
			m_uitext.SetText(m_TextValue);
		}

		void GUITextInput::RemoveCharacter()
		{
			m_TextValue = m_TextValue.substr(0, m_TextValue.size() - 1);

			if (m_Flags & GUITextInputFlags::PasswordField)
			{
				std::string stars;
				for (size_t i = 0; i < m_TextValue.size(); i++)
				{
					stars += "*";
				}
				m_uitext.SetText(stars);
			}
			else
			{

				m_uitext.SetText(m_TextValue);
			}
		}
		int GUITextInput::GetNumber()
		{
			if (m_Flags & GUITextInputFlags::NumbersOnly)
			{
				return std::stoi(m_TextValue);
			}
			return 0;
		}
		GUIRect GUITextInput::GetTextInputRect()
		{
			return m_TextInputRect;
		}
	}
}

