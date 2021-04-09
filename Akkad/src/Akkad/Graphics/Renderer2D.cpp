#include "Renderer2D.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"

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

			auto vertexbuffer = platform->CreateVertexBuffer();
			auto indexbuffer = platform->CreateIndexBuffer();

			vertexbuffer->SetLayout(layout);
			vertexbuffer->SetData(vertices, sizeof(vertices));
			indexbuffer->SetData(indices, sizeof(indices));

			m_QuadVB = vertexbuffer;
			m_QuadIB = indexbuffer;

			UniformBufferLayout scenePropsLayout;
			scenePropsLayout.Push("sys_transform", ShaderDataType::MAT4);
			scenePropsLayout.Push("sys_viewProjection", ShaderDataType::MAT4);

			m_SceneProps = platform->CreateUniformBuffer(scenePropsLayout);
		}

		void Renderer2D::BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform)
		{
			glm::mat4 view = glm::inverse(cameraTransform);
			glm::mat4 projection = camera.GetProjection();
			glm::mat4 viewProjection = projection * view;

			m_SceneProps->SetData("sys_viewProjection", viewProjection);
		}

		void Renderer2D::DrawQuadImpl(SharedPtr<Texture> texture, glm::mat4& transform)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			/*
			m_QuadVB->Bind();
			m_QuadIB->Bind();
			m_TextureShader->Bind();
			m_SceneProps->SetData("sys_transform", transform);
			texture->Bind(0);

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
			*/
		}

		void Renderer2D::DrawQuadImpl(Material& material, glm::mat4& transform)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			
			if (material.isValid())
			{
				material.BindShaders();
				material.BindTextures();
				m_SceneProps->SetData("sys_transform", transform);

				m_QuadVB->Bind();
				m_QuadIB->Bind();
				command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
			}

		}
		void Renderer2D::DrawQuadImpl(glm::vec3 color, glm::mat4& transform)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			m_ColorShader->Bind();
			m_ColorShaderProps->SetData("props_color", color);
			m_SceneProps->SetData("sys_transform", transform);

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
		}
		void Renderer2D::DrawImpl(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			vb->Bind();
			shader->Bind();
			command->DrawArrays(PrimitiveType::TRIANGLE_FAN, vertexCount);
		}

		void Renderer2D::InitShadersImpl()
		{
			auto assetManager = Application::GetAssetManager();
			auto platform = Application::GetRenderPlatform();

			{
				auto colorShader = assetManager->GetShaderByName("r2d_colorShader");
				m_ColorShader = platform->CreateShader(colorShader.absolutePath.c_str());

				UniformBufferLayout layout;
				layout.Push("props_color", ShaderDataType::FLOAT3);
				m_ColorShaderProps = platform->CreateUniformBuffer(layout);
				m_ColorShaderProps->SetName("shader_props");

				m_ColorShader->SetUniformBuffer(m_ColorShaderProps);
				m_ColorShader->SetUniformBuffer(m_SceneProps);

			}

		}
	}
}

