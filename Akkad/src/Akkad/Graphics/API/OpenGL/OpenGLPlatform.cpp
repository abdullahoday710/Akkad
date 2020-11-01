#include "OpenGLPlatform.h"
#include "Akkad/Application/Application.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		void OpenGLPlatform::Init()
		{
			auto window = Application::GetInstance().GetWindow();
			window->CreateContext(m_API);
			gladLoadGL();
			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}
	}
}