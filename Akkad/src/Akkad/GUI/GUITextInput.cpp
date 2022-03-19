#include "GUITextInput.h"
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
			if (m_uitext.GetFittingMode() != GUIText::FittingMode::KEEP_FONT_SIZE)
			{
				m_uitext.SetFittingMode(GUIText::FittingMode::KEEP_FONT_SIZE);
			}
			m_uitext.m_BoundingBox.SetParent(rect.GetRect());
			m_uitext.m_BoundingBox.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0.9 });
			m_uitext.m_BoundingBox.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 0.2 });
			m_uitext.m_BoundingBox.SetXConstraint({ ConstraintType::CENTER_CONSTRAINT, 0});
			m_uitext.m_BoundingBox.SetYConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });
			m_uitext.SetColor(m_TextColor);
			m_uitext.RecalculateTextPosition();
		}

		void GUITextInput::SetTextAlignment(GUIText::Alignment alignment)
		{
			m_uitext.SetAlignment(alignment);
		}

		void GUITextInput::AddCharacter(char characater)
		{
			m_TextValue += characater;
			m_uitext.SetText(m_TextValue);
		}

		void GUITextInput::RemoveCharacter()
		{
			m_TextValue = m_TextValue.substr(0, m_TextValue.size() - 1);
			m_uitext.SetText(m_TextValue);
		}
		GUIRect GUITextInput::GetTextInputRect()
		{
			return m_TextInputRect;
		}
	}
}

