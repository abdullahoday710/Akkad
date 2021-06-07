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

			m_SceneCameraViewProjection = viewProjection;
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

		void Renderer2D::DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color, bool filled)
		{
			auto platform = Application::GetRenderPlatform();
			auto command = platform->GetRenderCommand();

			float vertices[] = {
				// positions             // texture coords
				 max.x,  min.y, 0.0f,    1.0f, 1.0f,  // top right
				 max.x, max.y, 0.0f,     1.0f, 0.0f,  // bottom right
				min.x, max.y, 0.0f,      0.0f, 0.0f,  // bottom left
				 min.x,  min.y, 0.0f,    0.0f, 1.0f   // top left 
			};

			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);
			m_RectShaderProps->SetData("props_color", color);
			m_RectShader->Bind();
			m_RectVB->SetSubData(0, vertices, sizeof(vertices));
			m_RectVB->Bind();
			m_QuadIB->Bind();

			if (filled)
			{
				command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
			}

			else
			{
				command->SetPolygonMode(PolygonMode::LINE);
				command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
				command->SetPolygonMode(PolygonMode::FILL);
			}
		}

		void Renderer2D::DrawRectImpl(glm::vec2 min, glm::vec2 max, glm::vec3 color, bool filled, glm::mat4 projection)
		{
			auto platform = Application::GetRenderPlatform();
			auto command = platform->GetRenderCommand();

			float vertices[] = {
				// positions             // texture coords
				 max.x,  min.y, 0.0f,    1.0f, 1.0f,  // top right
				 max.x, max.y, 0.0f,     1.0f, 0.0f,  // bottom right
				min.x, max.y, 0.0f,      0.0f, 0.0f,  // bottom left
				 min.x,  min.y, 0.0f,    0.0f, 1.0f   // top left 
			};

			m_RectShaderProps->SetData("props_color", color);
			m_SceneProps->SetData("sys_viewProjection", projection);
			m_RectShader->Bind();
			m_RectVB->SetSubData(0, vertices, sizeof(vertices));
			m_RectVB->Bind();
			m_QuadIB->Bind();

			if (filled)
			{
				command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
			}
			else
			{
				command->SetPolygonMode(PolygonMode::LINE);
				command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
				command->SetPolygonMode(PolygonMode::FILL);
			}

			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);

		}

		void Renderer2D::DrawRectImpl(Rect rect, SharedPtr<Texture> texture, glm::mat4 projection)
		{
			auto platform = Application::GetRenderPlatform();
			auto command = platform->GetRenderCommand();
			auto min = rect.GetMin();
			auto max = rect.GetMax();

			auto mintex = rect.GetMinTextureCoords();
			auto maxtex = rect.GetMaxTextureCoords();
			
				
			float vertices[] = {
				// positions                // texture coords
				 max.x,  min.y, 0.0f,       maxtex.x, mintex.y,  // top right
				 max.x,  max.y, 0.0f,       maxtex.x, maxtex.y,  // bottom right
				 min.x,  max.y, 0.0f,       mintex.x, maxtex.y,  // bottom left
				 min.x,  min.y, 0.0f,       mintex.x, mintex.y,   // top left 
			};

			m_SceneProps->SetData("sys_viewProjection", projection);
			texture->Bind(0);
			m_TexturedRectShader->Bind();
			m_RectVB->SetSubData(0, vertices, sizeof(vertices));
			m_RectVB->Bind();
			m_QuadIB->Bind();

			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);

			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);
		}

		void Renderer2D::DrawRectImpl(Rect rect, glm::vec3 color, bool filled)
		{
			DrawRectImpl(rect.GetMin(), rect.GetMax(), color, filled);
		}

		void Renderer2D::DrawRectImpl(Rect rect, glm::vec3 color, bool filled, glm::mat4 projection)
		{
			DrawRectImpl(rect.GetMin(), rect.GetMax(), color, filled, projection);
		}


		void Renderer2D::DrawImpl(SharedPtr<VertexBuffer> vb, SharedPtr<Shader> shader, unsigned int vertexCount)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			vb->Bind();
			shader->Bind();
			command->DrawArrays(PrimitiveType::TRIANGLE_FAN, vertexCount);
		}

		void Renderer2D::RenderTextImpl(GUI::GUIText& uitext, glm::mat4 projection)
		{
			if (uitext.IsValid())
			{
				auto atlasSize = uitext.GetFont()->GetTextureAtlasSize();
				unsigned int has_tint = 1;
				m_TexturedRectShaderProps->SetData("tint_color", uitext.GetColor());
				m_TexturedRectShaderProps->SetData("has_tint", has_tint);

				for (auto& line : uitext.GetLines())
				{
					for (const auto& fontCharacter : line.characters)
					{
						auto command = Application::GetRenderPlatform()->GetRenderCommand();

						command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::INVERSE_SRC_ALPHA);
						command->EnableBlending();
						DrawRectImpl(fontCharacter.CharacterRect, uitext.GetFont()->GetTextureAtlas(), projection);
						command->DisableBlending();
					}

					if (m_DrawDebugGUIRects)
					{
						const Graphics::Rect rect = line.boundingBox.GetRect();
						DrawRectImpl(rect, { 1,0,0 }, false, projection);
					}
					
				}
				
			}
			
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
				auto rectShader = assetManager->GetShaderByName("r2d_rectShader");
				m_RectShader = platform->CreateShader(rectShader.absolutePath.c_str());

				UniformBufferLayout layout;
				layout.Push("props_color", ShaderDataType::FLOAT3);
				m_RectShaderProps = platform->CreateUniformBuffer(layout);
				m_RectShaderProps->SetName("shader_props");

				m_RectShader->SetUniformBuffer(m_RectShaderProps);
				m_RectShader->SetUniformBuffer(m_SceneProps);


				VertexBufferLayout vbLayout;
				vbLayout.isDynamic = true;
				vbLayout.Push(ShaderDataType::FLOAT, 3); // position
				vbLayout.Push(ShaderDataType::FLOAT, 2); // texture coords
				m_RectVB = platform->CreateVertexBuffer();
				m_RectVB->SetLayout(vbLayout);
				m_RectVB->SetData(0, 5 * GetSizeOfType(ShaderDataType::FLOAT3) * GetSizeOfType(ShaderDataType::FLOAT2));
			}

			{
				auto texturedRectShader = assetManager->GetShaderByName("r2d_texturedRect");
				m_TexturedRectShader = platform->CreateShader(texturedRectShader.absolutePath.c_str());

				UniformBufferLayout layout;
				layout.Push("tint_color", ShaderDataType::FLOAT3);
				layout.Push("has_tint", ShaderDataType::UNISGNED_INT);

				m_TexturedRectShaderProps = platform->CreateUniformBuffer(layout);
				m_TexturedRectShaderProps->SetName("shader_props");

				m_TexturedRectShader->SetUniformBuffer(m_TexturedRectShaderProps);
				m_TexturedRectShader->SetUniformBuffer(m_SceneProps);

			}

		}
	}
}

