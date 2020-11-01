#pragma once
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class OpenGLPlatform : public RenderPlatform
		{
		public:
			virtual RenderAPI GetRenderAPI() override { return m_API; }
			virtual void Init() override;
		private:
			RenderAPI m_API = RenderAPI::OPENGL;
		};
	}
}


