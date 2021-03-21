#include "GLRenderCommand.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {

		unsigned int PrimitiveTypeToGLType(PrimitiveType type) {
			switch (type)
			{
			case PrimitiveType::TRIANGLE:
				return GL_TRIANGLES;
			case PrimitiveType::TRIANGLE_FAN:
				return GL_TRIANGLE_FAN;
			case PrimitiveType::POINTS:
				return GL_POINTS;
			case PrimitiveType::LINE:
				return GL_LINE;
			default:
				AK_ASSERT(false, "unkown primitive type");
			}
		}

		unsigned int PolygonModeToGLPolygonMode(PolygonMode mode) {
			switch (mode)
			{
			case PolygonMode::POINT:
				return GL_POINT;
			case PolygonMode::LINE:
				return GL_LINE;
			case PolygonMode::FILL:
				return GL_FILL;
			default:
				AK_ASSERT(false, "unkown polygon mode !");
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

		void GLRenderCommand::DrawArrays(PrimitiveType type, unsigned int count)
		{
			glDrawArrays(PrimitiveTypeToGLType(type), 0, count);
		}

		void GLRenderCommand::DrawIndexed(PrimitiveType type, unsigned int count)
		{
			glDrawElements(PrimitiveTypeToGLType(type), count, GL_UNSIGNED_INT, 0);
		}

		void GLRenderCommand::SetPolygonMode(PolygonMode mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, PolygonModeToGLPolygonMode(mode));
		}
	}
}