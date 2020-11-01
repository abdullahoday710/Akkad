#pragma once
#include "Akkad/core.h"
#include "RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class RenderCommand
		{
		public:
			virtual void ClearColorBuffer() = 0;
			virtual void SetClearColor(float r, float g, float b) = 0;
			static SharedPtr<RenderCommand> Create(RenderAPI api);
		};
	}
}


