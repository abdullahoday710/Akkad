#include "Renderer2D.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/GUI/GUIText.h"

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
			m_Camera = camera;
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

		void Renderer2D::RenderTextImpl(GUI::GUIText& text, float x, float y, float scale, glm::vec3 color, glm::mat4 projection)
		{
			using namespace GUI;
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			std::string::const_iterator c;


			m_GUITextShaderProps->SetData("projection", projection);
			m_GUITextShaderProps->SetData("text_color", color);

			command->EnableBlending();
			command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::INVERSE_SRC_ALPHA);
			m_GUITextShader->Bind();
			for (c = text.m_Text.begin(); c != text.m_Text.end(); c++)
			{
				Font::FontCharacter ch = text.GetFont()->GetCharacter(*c);

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;

				auto fontAtlasSize = text.GetFont()->GetTextureAtlasSize();

				float vertices[6][4] = {
					{ xpos,     ypos + h,   ch.xOffset / fontAtlasSize.x, 0.0f },
					{ xpos,     ypos,       ch.xOffset / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },
					{ xpos + w, ypos,       (ch.xOffset + ch.Size.x) / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },

					{ xpos,     ypos + h,   ch.xOffset / fontAtlasSize.x, 0.0f },
					{ xpos + w, ypos,       (ch.xOffset + ch.Size.x) / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },
					{ xpos + w, ypos + h,   (ch.xOffset + ch.Size.x) / fontAtlasSize.x, 0.0f }
				};

				m_GUITextVB->SetSubData(0, vertices, sizeof(vertices));
				m_GUITextVB->Bind();
				text.GetFont()->GetAtlas()->Bind(0);
				Application::GetRenderPlatform()->GetRenderCommand()->DrawArrays(PrimitiveType::TRIANGLE, 6);
				x += (ch.Advance >> 6) * scale;
			}

			command->DisableBlending();
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

			{
				static auto guiTextShader = assetManager->GetShaderByName("r2d_guitext");
				m_GUITextShader = assetManager->GetShader(guiTextShader.assetID);

				UniformBufferLayout layout;
				layout.Push("projection", ShaderDataType::MAT4);
				layout.Push("text_color", ShaderDataType::FLOAT3);

				m_GUITextShaderProps = platform->CreateUniformBuffer(layout);
				m_GUITextShaderProps->SetName("shader_props");

				m_GUITextShader->SetUniformBuffer(m_GUITextShaderProps);
				VertexBufferLayout vblayout;

				vblayout.isDynamic = true;
				vblayout.Push(ShaderDataType::FLOAT, 4);
				m_GUITextVB = platform->CreateVertexBuffer();
				m_GUITextVB->SetLayout(vblayout);
				
				m_GUITextVB->SetData(0, 7 * GetSizeOfType(ShaderDataType::FLOAT4));
			}

		}
	}
}

