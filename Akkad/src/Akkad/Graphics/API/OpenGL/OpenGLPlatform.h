#pragma once
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class OpenGLPlatform : public RenderPlatform
		{
		public:
			~OpenGLPlatform();

			virtual RenderAPI GetRenderAPI() override { return m_API; }
			virtual RenderCommand* GetRenderCommand() override { return m_Command; }

			virtual void Init() override;
			virtual void OnWindowResize(unsigned int width, unsigned int height) override;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() override;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() override;
			virtual SharedPtr<Shader> CreateShader(const char* path) override;
			virtual SharedPtr<Texture> CreateTexture(const char* path) override;

		private:
			RenderAPI m_API = RenderAPI::OPENGL;
			RenderCommand* m_Command;
		};
	}
}


