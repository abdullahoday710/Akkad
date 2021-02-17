#pragma once
#include "Akkad/core.h"
#include "Buffer.h"
#include "UniformBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderCommand.h"
#include "FrameBuffer.h"
#include "RenderContext.h"

namespace Akkad {
	namespace Graphics {

		class RenderPlatform {
		public:
			virtual RenderAPI GetRenderAPI() = 0;
			virtual RenderCommand* GetRenderCommand() = 0;
			virtual SharedPtr<RenderContext> GetRenderContext() = 0;

			virtual void Init() = 0;
			virtual void OnWindowResize(unsigned int width, unsigned int height) = 0;

			virtual SharedPtr<VertexBuffer> CreateVertexBuffer() = 0;
			virtual SharedPtr<IndexBuffer> CreateIndexBuffer() = 0;
			virtual SharedPtr<Shader> CreateShader(const char* path) = 0;
			virtual SharedPtr<Texture> CreateTexture(const char* path) = 0;
			virtual SharedPtr<FrameBuffer> CreateFrameBuffer(FrameBufferDescriptor desc) = 0;
			virtual SharedPtr<UniformBuffer> CreateUniformBuffer(UniformBufferLayout layout) = 0;

			static SharedPtr<RenderPlatform> Create(RenderAPI api);
		};
	}
}
