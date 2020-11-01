#pragma once
#include "Akkad/core.h"

namespace Akkad {
	namespace Graphics {

		enum class RenderAPI {
			OPENGL
		};

		class RenderPlatform {
		public:
			virtual RenderAPI GetRenderAPI() = 0;
			virtual void Init() = 0;
			static SharedPtr<RenderPlatform> Create(RenderAPI api);
		};
	}
}
