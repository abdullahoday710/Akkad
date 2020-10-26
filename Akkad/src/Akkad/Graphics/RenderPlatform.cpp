#include "RenderPlatform.h"
#include "API/OpenGL/OpenGLPlatform.h"

namespace Akkad {
	namespace Graphics {
		RenderPlatform* RenderPlatform::Create(RenderAPI api)
		{
			switch (api)
			{
			case RenderAPI::OPENGL:
				return new OpenGLPlatform();
			default:
				break;
			}
		}
	}
}