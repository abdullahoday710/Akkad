#pragma once
#include "Akkad/Graphics/RenderCommand.h"

namespace Akkad {
	namespace Graphics {

		class GLRenderCommand : public RenderCommand
		{
		public:
			virtual void Clear() override;
			virtual void SetClearColor(float r, float g, float b) override;
			virtual void DrawIndexed(PrimitiveType type, unsigned int count) override;
		};
	}
}


