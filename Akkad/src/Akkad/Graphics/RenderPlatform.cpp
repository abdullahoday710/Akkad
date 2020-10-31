#include "RenderPlatform.h"
#include "API/OpenGL/OpenGLPlatform.h"

namespace Akkad {
	namespace Graphics {
		SharedPtr<RenderPlatform> RenderPlatform::Create(RenderAPI api)
		{
			switch (api)
			{
			case RenderAPI::OPENGL:
				return CreateSharedPtr<OpenGLPlatform>();
			default:
				break;
			}
		}
	}
}