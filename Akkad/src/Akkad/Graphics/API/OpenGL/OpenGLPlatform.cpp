#include "OpenGLPlatform.h"
#include "Akkad/Application/Application.h"
#include "GLIndexBuffer.h"
#include "GLVertexBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLRenderCommand.h"
#include "GLFrameBuffer.h"
#include "GLUniformBuffer.h"

#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		OpenGLPlatform::~OpenGLPlatform()
		{
			delete m_Command;
		}
		void OpenGLPlatform::Init()
		{
			auto context = RenderContext::Create();
			context->Init(m_API);

			m_RenderContext = context;

			RenderCommand* command = new GLRenderCommand();
			m_Command = command;
		}

		void OpenGLPlatform::OnWindowResize(unsigned int width, unsigned int height)
		{
			glViewport(0, 0, width, height);
		}

		SharedPtr<VertexBuffer> OpenGLPlatform::CreateVertexBuffer()
		{
			return CreateSharedPtr<GLVertexBuffer>();
		}

		SharedPtr<IndexBuffer> OpenGLPlatform::CreateIndexBuffer()
		{
			return CreateSharedPtr<GLIndexBuffer>();
		}

		SharedPtr<Shader> OpenGLPlatform::CreateShader(const char* path)
		{
			return CreateSharedPtr<GLShader>(path);
		}

		SharedPtr<Texture> OpenGLPlatform::CreateTexture(const char* path)
		{
			return CreateSharedPtr<GLTexture>(path);
		}

		SharedPtr<Texture> OpenGLPlatform::CreateTexture(TextureDescriptor desc)
		{
			return CreateSharedPtr<GLTexture>(desc);
		}

		SharedPtr<Texture> OpenGLPlatform::CreateTexture(const char* path, float tileWidth, float tileHeight)
		{
			return CreateSharedPtr<GLTexture>(path, tileWidth, tileHeight);
		}

		SharedPtr<FrameBuffer> OpenGLPlatform::CreateFrameBuffer(FrameBufferDescriptor desc)
		{
			return CreateSharedPtr<GLFrameBuffer>(desc);
		}

		SharedPtr<RenderContext> OpenGLPlatform::GetRenderContext()
		{
			return m_RenderContext;
		}

		SharedPtr<UniformBuffer> OpenGLPlatform::CreateUniformBuffer(UniformBufferLayout layout)
		{
			return CreateSharedPtr<GLUniformBuffer>(layout);
		}
	}
}