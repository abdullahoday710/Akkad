#pragma once
#include "RenderPlatform.h"

namespace Akkad {
	namespace Graphics {

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


