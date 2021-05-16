#include "GUIContainer.h"

#include <glm/gtc/matrix_transform.hpp>
namespace Akkad {
	namespace GUI {

		void GUIContainer::SetScreenSize(glm::vec2 size)
		{
			m_Projection = glm::ortho(0.0f, size.x, size.y, 0.0f);
			m_ScreenSize = size;
		}
	}
}

