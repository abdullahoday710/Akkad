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

		void GUIRect::SetParent(Graphics::Rect parent)
		{
			if (m_ParentRect != parent)
			{
				m_ParentRect = parent;
			}
		}

		void GUIRect::SetPreviousChild(Graphics::Rect child)
		{
			if (m_PreviousChild != child)
			{
				m_PreviousChild = child;
			}
		}

		void GUIRect::SetWidth(ConstraintType type, float width)
		{
			switch (type)
			{
			case ConstraintType::CENTER_CONSTRAINT:
				break;
			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				m_Rect.SetWidth(width * m_ParentRect.GetWidth());
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
				m_Rect.SetHeight(height * m_ParentRect.GetHeight());
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
				m_Rect.SetX((m_ParentRect.GetMin().x) + (m_ParentRect.GetWidth() / 2));
				break;
			}

			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				glm::vec2 anchoredPosition = GetAnchoredPosition(true, true);
				m_Rect.SetX((anchoredPosition.x + xPos));
				break;
			}

			case ConstraintType::PIXEL_CONSTRAINT:
			{
				glm::vec2 anchoredPosition = GetAnchoredPosition(true, true);
				m_Rect.SetX(anchoredPosition.x + xPos);
				break;
			}
			case ConstraintType::PREVIOUS_CHILD_MAX_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetMax().x + (m_PreviousChild.GetWidth() / 2);
				m_Rect.SetX(previousChildPos + xPos);
				break;
			}

			case ConstraintType::PREVIOUS_CHILD_MIN_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetMin().x - (m_PreviousChild.GetWidth() / 2);
				m_Rect.SetX(previousChildPos + xPos);
				break;
			}

			case ConstraintType::PREVIOUS_CHILD_CENTER_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetPosition().x;
				m_Rect.SetX(previousChildPos + xPos);
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
				m_Rect.SetY(m_ParentRect.GetMin().y + (m_ParentRect.GetHeight() / 2));
				break;
			}

			case ConstraintType::RELATIVE_CONSTRAINT:
			{
				glm::vec2 anchoredPosition = GetAnchoredPosition(true, true);
				m_Rect.SetY((anchoredPosition.y + yPos));
				break;
			}

			case ConstraintType::PIXEL_CONSTRAINT:
			{
				glm::vec2 anchoredPosition = GetAnchoredPosition(true, true);
				m_Rect.SetY(anchoredPosition.y + yPos);
				break;
			}
			case ConstraintType::PREVIOUS_CHILD_MAX_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetMax().y + (m_PreviousChild.GetHeight() / 2);
				m_Rect.SetY(previousChildPos + yPos);
				break;
			}
			case ConstraintType::PREVIOUS_CHILD_MIN_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetMin().y + (m_PreviousChild.GetHeight() / 2);
				m_Rect.SetY(previousChildPos + yPos);
				break;
			}
			case ConstraintType::PREVIOUS_CHILD_CENTER_CONSTRAINT:
			{
				float previousChildPos = m_PreviousChild.GetPosition().y;
				m_Rect.SetY(previousChildPos + yPos);
				break;
			}
			case ConstraintType::ASPECT_CONSTRAINT:
				break;

			default:
				break;
			}
		}

		glm::vec2 GUIRect::GetAnchoredPosition(bool adjustXHeight, bool adjustYHeight)
		{
			glm::vec2 anchoredPos;
			switch (m_AnchorType)
			{
			case Akkad::GUI::AnchorType::TOP_LEFT:
			{
				if (adjustXHeight)
				{
					anchoredPos.x = m_ParentRect.GetMin().x + (m_Rect.GetWidth() / 2);
				}
				else
				{
					anchoredPos.x = m_ParentRect.GetMin().x;
				}
				if (adjustYHeight)
				{
					anchoredPos.y = m_ParentRect.GetMin().y + (m_Rect.GetHeight() / 2);
				}
				else
				{
					anchoredPos.y = m_ParentRect.GetMin().y;
				}
				break;
			}
			case Akkad::GUI::AnchorType::TOP_RIGHT:
			{
				if (adjustXHeight)
				{
					anchoredPos.x = m_ParentRect.GetMax().x - (m_Rect.GetWidth() / 2);
				}
				else
				{
					anchoredPos.x = m_ParentRect.GetMax().x;
				}
				if (adjustYHeight)
				{
					anchoredPos.y = m_ParentRect.GetMin().y + (m_Rect.GetHeight() / 2);
				}
				else
				{
					anchoredPos.y = m_ParentRect.GetMin().y;
				}
				
				break;
			}
			case Akkad::GUI::AnchorType::BOTTOM_LEFT:
			{
				if (adjustXHeight)
				{
					anchoredPos.x = m_ParentRect.GetMin().x + (m_Rect.GetWidth() / 2);
				}
				else
				{
					anchoredPos.x = m_ParentRect.GetMin().x;
				}
				if (adjustYHeight)
				{
					anchoredPos.y = m_ParentRect.GetMax().y - (m_Rect.GetHeight() / 2);
				}
				else
				{
					anchoredPos.y = m_ParentRect.GetMax().y;
				}
				break;
			}
			case Akkad::GUI::AnchorType::BOTTOM_RIGHT:
			{
				if (adjustXHeight)
				{
					anchoredPos.x = m_ParentRect.GetMax().x - (m_Rect.GetWidth() / 2);
				}
				else
				{
					anchoredPos.x = m_ParentRect.GetMax().x;
				}
				if (adjustYHeight)
				{
					anchoredPos.y = m_ParentRect.GetMax().y - (m_Rect.GetHeight() / 2);
				}
				else
				{
					anchoredPos.y = m_ParentRect.GetMax().y;
				}
				break;
			}
			default:
				break;
			}

			return anchoredPos;
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