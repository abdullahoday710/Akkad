#include "RenderCommand.h"
#include "API/OpenGL/GLRenderCommand.h"

namespace Akkad {
	namespace Graphics {
		SharedPtr<RenderCommand> RenderCommand::Create(RenderAPI api)
		{
			switch (api)
			{
			case Akkad::Graphics::RenderAPI::OPENGL:
				return CreateSharedPtr<GLRenderCommand>();
			}
		}
	}
}