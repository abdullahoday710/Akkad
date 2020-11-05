#pragma once
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class OpenGLPlatform : public RenderPlatform
		{
		public:
			virtual RenderAPI GetRenderAPI() override { return m_API; }
			virtual void Init() override;
			virtual void OnWindowResize(unsigned int width, unsigned int height) override;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() override;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() override;

		private:
			RenderAPI m_API = RenderAPI::OPENGL;
		};
	}
}


