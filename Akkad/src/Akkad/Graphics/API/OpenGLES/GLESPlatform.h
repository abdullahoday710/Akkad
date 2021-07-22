#pragma once
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class GLESPlatform : public RenderPlatform
		{
		public:
			~GLESPlatform();

			virtual RenderAPI GetRenderAPI() override { return m_API; }
			virtual RenderCommand* GetRenderCommand() override { return m_Command; }

			virtual void Init() override;
			virtual void OnWindowResize(unsigned int width, unsigned int height) override;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() override;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() override;
			virtual SharedPtr<Shader> CreateShader(const char* path) override;
			virtual SharedPtr<Texture> CreateTexture(const char* path) override;
			virtual SharedPtr<Texture> CreateTexture(TextureDescriptor desc) override;
			virtual SharedPtr<Texture> CreateTexture(const char* path, float tileWidth, float tileHeight) override;
			virtual SharedPtr<FrameBuffer> CreateFrameBuffer(FrameBufferDescriptor desc) override;
			virtual SharedPtr<RenderContext> GetRenderContext() override;
			virtual SharedPtr<UniformBuffer> CreateUniformBuffer(UniformBufferLayout layout) override;

		private:
			RenderAPI m_API = RenderAPI::OPENGL;
			SharedPtr<RenderContext> m_RenderContext;
			RenderCommand* m_Command;
		};
	}
}


