#include "RenderPlatform.h"
#include "API/OpenGL/OpenGLPlatform.h"
#include "API/OpenGLES/GLESPlatform.h"

#include "Akkad/PlatformMacros.h"
namespace Akkad {
	namespace Graphics {
		SharedPtr<RenderPlatform> RenderPlatform::Create(RenderAPI api)
		{
			switch (api)
			{
			case RenderAPI::OPENGL:
				#if defined(AK_PLATFORM_WINDOWS)
				return CreateSharedPtr<OpenGLPlatform>();
				#else
				return nullptr;
				#endif // AK_PLATFORM_WINDOWS
			case RenderAPI::OPENGLES:
				#if defined(AK_PLATFORM_WEB) 
				return CreateSharedPtr<GLESPlatform>();
				#else
				return nullptr;
				#endif // AK_PLATFORM_WEB


			default:
				break;
			}
		}
	}
}