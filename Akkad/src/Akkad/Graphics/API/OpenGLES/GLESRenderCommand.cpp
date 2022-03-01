#include "GLESRenderCommand.h"

#include <GLES3/gl3.h>

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
				return GL_LINES;
			default:
				AK_ASSERT(false, "unkown primitive type");
			}
		}

		unsigned int PolygonModeToGLPolygonMode(PolygonMode mode) {
			/*
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
			*/
		}

		unsigned int BlendSrcToGLSrc(BlendSourceFactor factor) {
			switch (factor)
			{
			case BlendSourceFactor::ALPHA:
				return GL_SRC_ALPHA;
			}
		}

		unsigned int BlendDestToGLDest(BlendDestFactor factor) {
			switch (factor)
			{
			case BlendDestFactor::INVERSE_SRC_ALPHA:
				return GL_ONE_MINUS_SRC_ALPHA;
			case BlendDestFactor::ONE:
				return GL_ONE;
			}
		}

		void GLESRenderCommand::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void GLESRenderCommand::SetClearColor(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
		}

		void GLESRenderCommand::DrawArrays(PrimitiveType type, unsigned int count)
		{
			glDrawArrays(PrimitiveTypeToGLType(type), 0, count);
		}

		void GLESRenderCommand::DrawIndexed(PrimitiveType type, unsigned int count)
		{
			glDrawElements(PrimitiveTypeToGLType(type), count, GL_UNSIGNED_INT, 0);
		}

		void GLESRenderCommand::SetPolygonMode(PolygonMode mode)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, PolygonModeToGLPolygonMode(mode));
		}

		void GLESRenderCommand::EnableBlending()
		{
			glEnable(GL_BLEND);
		}

		void GLESRenderCommand::DisableBlending()
		{
			glDisable(GL_BLEND);
		}

		void GLESRenderCommand::SetBlendState(BlendSourceFactor sfactor, BlendDestFactor dfactor)
		{
			unsigned int sourcefactor = BlendSrcToGLSrc(sfactor);
			unsigned int destfactor = BlendDestToGLDest(dfactor);
			glBlendFunc(sourcefactor, destfactor);
		}

		void GLESRenderCommand::DrawElementsInstanced(PrimitiveType type, unsigned int count, unsigned int amount)
		{
			glDrawElementsInstanced(PrimitiveTypeToGLType(type), count, GL_UNSIGNED_INT, 0, amount);
		}
	}
}