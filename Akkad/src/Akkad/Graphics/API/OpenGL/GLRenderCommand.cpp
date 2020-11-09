#include "GLRenderCommand.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {

		unsigned int PrimitiveTypeToGLType(PrimitiveType type) {
			switch (type)
			{
			case PrimitiveType::TRIANGLE:
				return GL_TRIANGLES;
			case PrimitiveType::POINTS:
				return GL_POINTS;
			default:
				AK_ASSERT(false, "unkown primitive type");
			}
		}
		void GLRenderCommand::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void GLRenderCommand::SetClearColor(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
		}

		void GLRenderCommand::DrawIndexed(PrimitiveType type, unsigned int count)
		{
			glDrawElements(PrimitiveTypeToGLType(type), count, GL_UNSIGNED_INT, 0);
		}
	}
}