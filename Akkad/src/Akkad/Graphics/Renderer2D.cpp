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
			VertexBufferLayout layout;

			layout.Push(ShaderDataType::FLOAT, 3); // positions
			layout.Push(ShaderDataType::FLOAT, 2); // texture coords

			UniformBufferLayout ubLayout;
			ubLayout.Push("floaty", ShaderDataType::FLOAT);
			ubLayout.Push("vecy", ShaderDataType::FLOAT3);
			ubLayout.Push("matty", ShaderDataType::MAT4);
			ubLayout.Push("floaty2", ShaderDataType::FLOAT4);
			ubLayout.Push("floaty3", ShaderDataType::FLOAT4);
			ubLayout.Push("floaty4", ShaderDataType::FLOAT4);


			ubLayout.Push("booly", ShaderDataType::BOOL);
			ubLayout.Push("int", ShaderDataType::UNISGNED_INT);

			auto vertexbuffer = platform->CreateVertexBuffer();
			auto indexbuffer = platform->CreateIndexBuffer();

			m_UniformBuffer = platform->CreateUniformBuffer(ubLayout);
			m_UniformBuffer->SetName("buffy");

			m_UniformBuffer->PushDataTest("floaty", 1.0f);
			m_UniformBuffer->ResetData();

			for (auto it : m_UniformBuffer->m_BufferData)
			{
				std::cout << "BYTE : " << it << std::endl;
			}

			vertexbuffer->SetLayout(layout);
			vertexbuffer->SetData(vertices, sizeof(vertices));
			indexbuffer->SetData(indices, sizeof(indices));

			m_QuadVB = vertexbuffer;
			m_QuadIB = indexbuffer;

			m_ColorShader = platform->CreateShader("res/shaders/colorShader.glsl");
			m_TextureShader = platform->CreateShader("res/shaders/textureShader.glsl");

			m_TextureShader->SetUniformBuffer(m_UniformBuffer);
		}

		void Renderer2D::BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform)
		{
			glm::mat4 view = glm::inverse(cameraTransform);
			glm::mat4 projection = camera.GetProjection();
			glm::mat4 viewProjection = projection * view;

			m_ColorShader->Bind();
			m_ColorShader->SetMat4("viewProjection", viewProjection);

			m_TextureShader->Bind();
			m_TextureShader->SetMat4("viewProjection", viewProjection);
		}

		void Renderer2D::DrawQuadImpl(SharedPtr<Texture> texture, glm::mat4& transform)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			m_TextureShader->Bind();
			//m_TextureShader->SetMat4("transform", transform);
			//texture->Bind(0);

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
		}

		void Renderer2D::DrawQuadImpl(glm::vec3 color, glm::mat4& transform)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			m_ColorShader->Bind();
			m_ColorShader->SetVec3("color", color);
			m_ColorShader->SetMat4("transform", transform);

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
		}
	}
}

