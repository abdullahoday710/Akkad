#include "GUISlider.h"
#include "Akkad/Logging.h"
namespace Akkad {
	namespace GUI {
		void GUISlider::SetSliderRect(GUIRect rect)
		{
			m_SliderRect = rect;
			m_KnobRect.SetParent(m_SliderRect.GetRect());

			m_KnobRect.SetWidthConstraint({ConstraintType::RELATIVE_CONSTRAINT, 0.045});
			m_KnobRect.SetHeightConstraint({ ConstraintType::ASPECT_CONSTRAINT, 1.5 });

			m_KnobRect.SetXConstraint({ ConstraintType::RELATIVE_CONSTRAINT, m_KnobX });
			m_KnobRect.SetYConstraint({ ConstraintType::CENTER_CONSTRAINT, 0 });

			m_SliderValue = (m_KnobRect.GetRect().GetPosition().x - m_SliderRect.GetRect().GetMin().x) / m_SliderRect.GetRect().GetWidth();
			
		}

		void GUISlider::SetKnobX(float x)
		{
			m_KnobX = x;
		}
	}
}
