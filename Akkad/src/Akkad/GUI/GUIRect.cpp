#include "GUIRect.h"

namespace Akkad {
	namespace GUI {

		GUIRect::GUIRect()
		{
			GUI::Constraint width_const = { GUI::ConstraintType::RELATIVE_CONSTRAINT, 0.3f };
			GUI::Constraint height_const = { GUI::ConstraintType::ASPECT_CONSTRAINT, 1.0f };

			GUI::Constraint x_const = { GUI::ConstraintType::CENTER_CONSTRAINT, 0 };
			GUI::Constraint y_const = { GUI::ConstraintType::CENTER_CONSTRAINT, 0 };

			SetWidthConstraint(width_const);
			SetHeightConstraint(height_const);

			SetXConstraint(x_const);
			SetYConstraint(y_const);
		}

		void GUIRect::SetWidth(ConstraintType type, float width)
		{
			switch (type)
			{
			case ConstraintType::CENTER_CONSTRAINT:
				break;
			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				m_Rect.SetWidth(width * m_ParentSize.x);
				break;
			}
			case ConstraintType::PIXEL_CONSTRAINT:
			{
				m_Rect.SetWidth(width);
				break;
			}
			case ConstraintType::ASPECT_CONSTRAINT:
			{
				m_Rect.SetWidth(m_Rect.GetHeight() * width);
				break;
			}
			default:
				break;
			}
		}

		void GUIRect::SetHeight(ConstraintType type, float height)
		{
			switch (type)
			{
			case ConstraintType::CENTER_CONSTRAINT:
				break;
			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				m_Rect.SetHeight(height * m_ParentSize.y);
				break;
			}
			case ConstraintType::PIXEL_CONSTRAINT:
			{
				m_Rect.SetHeight(height);
				break;
			}
			case ConstraintType::ASPECT_CONSTRAINT:
			{
				m_Rect.SetHeight(m_Rect.GetWidth() * height);
				break;
			}
			default:
				break;
			}
		}

		void GUIRect::SetX(ConstraintType type, float xPos)
		{
			switch (type)
			{

			case ConstraintType::CENTER_CONSTRAINT:
			{
				m_Rect.SetX((m_ParentPos.x) + (m_ParentSize.x / 2));
				break;
			}

			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				m_Rect.SetX((m_ParentPos.x + xPos) + (m_Rect.GetWidth() / 2));
				break;
			}

			case ConstraintType::PIXEL_CONSTRAINT:
			{
				m_Rect.SetX((m_Rect.GetWidth() / 2) + xPos);
				break;
			}

			case ConstraintType::ASPECT_CONSTRAINT:
				break;
			default:
				break;
			}
		}

		void GUIRect::SetY(ConstraintType type, float yPos)
		{
			switch (type)
			{

			case ConstraintType::CENTER_CONSTRAINT:
			{
				m_Rect.SetY(m_ParentPos.y + (m_ParentSize.y / 2));
				break;
			}

			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				m_Rect.SetY((m_ParentPos.y + yPos) + (m_Rect.GetHeight() / 2));
				break;
			}

			case ConstraintType::PIXEL_CONSTRAINT:
			{
				m_Rect.SetY((m_Rect.GetHeight() / 2) + yPos);
				break;
			}

			case ConstraintType::ASPECT_CONSTRAINT:
				break;
			default:
				break;
			}
		}

		void GUIRect::SetWidthConstraint(Constraint constraint)
		{
			m_widthConstraint = constraint;
			RecalculateRect();
		}

		void GUIRect::SetHeightConstraint(Constraint constraint)
		{
			m_heightConstraint = constraint;
			RecalculateRect();
		}

		void GUIRect::SetXConstraint(Constraint constraint)
		{
			m_xConstraint = constraint;
			RecalculateRect();
		}

		void GUIRect::SetYConstraint(Constraint constraint)
		{
			m_yConstraint = constraint;
			RecalculateRect();
		}

		void GUIRect::RecalculateRect()
		{
			SetWidth(m_widthConstraint.type, m_widthConstraint.constraintValue);
			SetHeight(m_heightConstraint.type, m_heightConstraint.constraintValue);

			SetX(m_xConstraint.type, m_xConstraint.constraintValue);
			SetY(m_yConstraint.type, m_yConstraint.constraintValue);
		}

	}
}