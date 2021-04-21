#pragma once
#include "Akkad/Graphics/RenderCommand.h"

namespace Akkad {
	namespace Graphics {

		class GLRenderCommand : public RenderCommand
		{
		public:
			virtual void Clear() override;
			virtual void SetClearColor(float r, float g, float b) override;
			virtual void DrawArrays(PrimitiveType type, unsigned int count);
			virtual void DrawIndexed(PrimitiveType type, unsigned int count) override;
			virtual void SetPolygonMode(PolygonMode mode) override;
			virtual void EnableBlending() override;
			virtual void DisableBlending() override;
			virtual void SetBlendState(BlendSourceFactor sfactor, BlendDestFactor dfactor) override;
		};
	}
}


