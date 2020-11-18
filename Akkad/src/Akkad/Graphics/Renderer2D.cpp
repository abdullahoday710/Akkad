#include "Renderer2D.h"

#include "Akkad/Application/Application.h"

namespace Akkad {

	namespace Graphics {

		Renderer2D Renderer2D::s_Instance;

		void Renderer2D::InitImpl()
		{
			float vertices[] = {
				// positions             // texture coords
				 0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
				 0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
				-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
				-0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
			};

			unsigned int indices[] = {
				0, 1, 3,   // first triangle
				1, 2, 3    // second triangle
			};

			auto platform = Application::GetRenderPlatform();
			BufferLayout layout;

			layout.Push(ShaderDataType::FLOAT, 3); // positions
			layout.Push(ShaderDataType::FLOAT, 2); // texture coords

			auto vertexbuffer = platform->CreateVertexBuffer();
			auto indexbuffer = platform->CreateIndexBuffer();

			vertexbuffer->SetLayout(layout);
			vertexbuffer->SetData(vertices, sizeof(vertices));
			indexbuffer->SetData(indices, sizeof(indices));

			m_QuadVB = vertexbuffer;
			m_QuadIB = indexbuffer;
		}

		void Renderer2D::DrawQuadImpl(SharedPtr<Shader> shader, SharedPtr<Texture> texture)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			shader->Bind();
			texture->Bind(0);

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
		}

		void Renderer2D::DrawQuadImpl(SharedPtr<Shader> shader, glm::vec3 color)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			shader->Bind();
			shader->SetVec3("color", color);

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
		}
	}
}

