#include "Akkad/Graphics/RenderContext.h"
#include "WebRenderContext.h"

namespace Akkad {
	namespace Graphics {
		SharedPtr<RenderContext> RenderContext::Create() {
			return CreateSharedPtr<WebRenderContext>();
		}
		void WebRenderContext::Init(RenderAPI api)
		{
			AK_ASSERT(api != RenderAPI::OPENGLES, "Web render api must be set to OpenGL ES !!");
			EmscriptenWebGLContextAttributes attrs;
			emscripten_webgl_init_context_attributes(&attrs);
			attrs.alpha = false;
			attrs.majorVersion = 2;
			attrs.minorVersion = 0;
			attrs.explicitSwapControl = true;
			attrs.renderViaOffscreenBackBuffer = true;

			m_Context = emscripten_webgl_create_context("#canvas", &attrs);
			emscripten_webgl_make_context_current(m_Context);

			
		}

		void WebRenderContext::SwapWindowBuffers()
		{
			emscripten_webgl_commit_frame();
		}

		void WebRenderContext::SetVsync(bool status)
		{
		}
	}

}


