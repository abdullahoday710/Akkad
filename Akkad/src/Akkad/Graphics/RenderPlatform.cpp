#include "RenderPlatform.h"
#include "API/OpenGL/OpenGLPlatform.h"
#include "API/OpenGLES/GLESPlatform.h"

namespace Akkad {
	namespace Graphics {
		SharedPtr<RenderPlatform> RenderPlatform::Create(RenderAPI api)
		{
			switch (api)
			{
			case RenderAPI::OPENGL:
				//return CreateSharedPtr<OpenGLPlatform>();
			case RenderAPI::OPENGLES:
				return CreateSharedPtr<GLESPlatform>();
			default:
				break;
			}
		}
	}
}