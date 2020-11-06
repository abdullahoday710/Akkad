#include "OpenGLPlatform.h"
#include "Akkad/Application/Application.h"
#include "GLBuffer.h"
#include "GLShader.h"
#include <glad/glad.h>

namespace Akkad {
	namespace Graphics {
		void OpenGLPlatform::Init()
		{
			auto window = Application::GetInstance().GetWindow();
			window->CreateContext(m_API);
			gladLoadGL();
			glViewport(0, 0, window->GetWidth(), window->GetHeight());
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
	}
}