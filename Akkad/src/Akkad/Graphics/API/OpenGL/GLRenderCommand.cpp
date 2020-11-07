#include "GLRenderCommand.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {

		void GLRenderCommand::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void GLRenderCommand::SetClearColor(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
		}
	}
}