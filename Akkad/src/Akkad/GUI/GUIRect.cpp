#include "GUIRect.h"

namespace Akkad {
	namespace GUI {

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
				m_Rect.SetWidth(m_Rect.GetHeight() / width);
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
				m_Rect.SetHeight(m_Rect.GetWidth() / height);
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

		void GUIRect::RecalculateRect()
		{
		}

	}
}