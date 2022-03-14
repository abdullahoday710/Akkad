#include "GUICheckBox.h"

namespace Akkad {
	namespace GUI {
		void GUICheckBox::SetBoxUIRect(GUIRect rect)
		{
			m_BoxUIRect = rect;

			m_MarkUIRect.SetParent(m_BoxUIRect.GetRect());

			m_MarkUIRect.SetXConstraint({ConstraintType::CENTER_CONSTRAINT, 0});
			m_MarkUIRect.SetYConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });

			m_MarkUIRect.SetWidthConstraint({ ConstraintType::RELATIVE_CONSTRAINT, 0.75 });
			m_MarkUIRect.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 1.0 });
		}
	}
}
