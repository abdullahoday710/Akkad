#pragma once
#include "Akkad/Graphics/RenderContext.h"

#include "Akkad/PlatformMacros.h"
#ifdef AK_PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif // AK_PLATFORM_WEB

namespace Akkad {
	namespace Graphics {
		class WebRenderContext : public RenderContext
		{
		public:
			virtual void Init(RenderAPI api);
			virtual void SwapWindowBuffers();
			virtual void SetVsync(bool status);
		private:
			EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_Context;
		};
	}
}