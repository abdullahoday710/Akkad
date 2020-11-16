#pragma once
#include "Akkad/core.h"

namespace Akkad {
	namespace Graphics {

		enum class RenderAPI {
			OPENGL
		};

		class RenderContext
		{
		public:
			virtual void Init(RenderAPI api) = 0;
			virtual void SwapWindowBuffers() = 0;
			virtual void SetVsync(bool status) = 0;

			static SharedPtr<RenderContext> Create();
		};
	}
}


