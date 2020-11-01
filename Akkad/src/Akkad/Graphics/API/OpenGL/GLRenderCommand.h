#pragma once
#include "Akkad/Graphics/RenderCommand.h"

namespace Akkad {
	namespace Graphics {

		class GLRenderCommand : public RenderCommand
		{
		public:
			virtual void ClearColorBuffer() override;
			virtual void SetClearColor(float r, float g, float b) override;
		};
	}
}


