#include "GLESPlatform.h"
#include "Akkad/Application/Application.h"
#include "GLESIndexBuffer.h"
#include "GLESVertexBuffer.h"
#include "GLESShader.h"
#include "GLESTexture.h"
#include "GLESRenderCommand.h"
#include "GLESFrameBuffer.h"
#include "GLESUniformBuffer.h"

#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		void GLESPlatform::Init()
		{
			auto context = RenderContext::Create();
			context->Init(m_API);

			m_RenderContext = context;

			RenderCommand* command = new GLESRenderCommand();
			m_Command = command;
		}

		void GLESPlatform::OnWindowResize(unsigned int width, unsigned int height)
		{
			glViewport(0, 0, width, height);
		}

		SharedPtr<VertexBuffer> GLESPlatform::CreateVertexBuffer()
		{
			return CreateSharedPtr<GLESVertexBuffer>();
		}

		SharedPtr<IndexBuffer> GLESPlatform::CreateIndexBuffer()
		{
			return CreateSharedPtr<GLESIndexBuffer>();
		}

		SharedPtr<Shader> GLESPlatform::CreateShader(const char* path)
		{
			return CreateSharedPtr<GLESShader>(path);
		}

		SharedPtr<Texture> GLESPlatform::CreateTexture(const char* path)
		{
			return CreateSharedPtr<GLESTexture>(path);
		}

		SharedPtr<Texture> GLESPlatform::CreateTexture(TextureDescriptor desc)
		{
			return CreateSharedPtr<GLESTexture>(desc);
		}

		SharedPtr<Texture> GLESPlatform::CreateTexture(const char* path, float tileWidth, float tileHeight)
		{
			return CreateSharedPtr<GLESTexture>(path, tileWidth, tileHeight);
		}

		SharedPtr<FrameBuffer> GLESPlatform::CreateFrameBuffer(FrameBufferDescriptor desc)
		{
			return CreateSharedPtr<GLESFrameBuffer>(desc);
		}

		SharedPtr<RenderContext> GLESPlatform::GetRenderContext()
		{
			return m_RenderContext;
		}

		SharedPtr<UniformBuffer> GLESPlatform::CreateUniformBuffer(UniformBufferLayout layout)
		{
			return CreateSharedPtr<GLESUniformBuffer>(layout);
		}
	}
}