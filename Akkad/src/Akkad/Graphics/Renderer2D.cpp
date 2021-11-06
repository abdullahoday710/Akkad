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
			// setting up quad vertex buffer
			{
				VertexBufferLayout layout;
				layout.isDynamic = true;
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
			
			// setting up line vertex buffer
			{
				m_LineBatchData = new LineVertex[MAX_BATCH_VERTS];
				VertexBufferLayout layout;
				layout.isDynamic = true;
				layout.Push(ShaderDataType::FLOAT, 2); // positions
				layout.Push(ShaderDataType::FLOAT, 3); // colors
				auto vertexbuffer = platform->CreateVertexBuffer();
				vertexbuffer->SetLayout(layout);
				m_LineVB = vertexbuffer;
				m_LineVB->SetData(0, sizeof(LineVertex) * MAX_BATCH_VERTS);
			}

			UniformBufferLayout scenePropsLayout;
			scenePropsLayout.Push("sys_transform", ShaderDataType::MAT4);
			scenePropsLayout.Push("sys_viewProjection", ShaderDataType::MAT4);

			m_SceneProps = platform->CreateUniformBuffer(scenePropsLayout);
			m_SceneProps->SetName("sys_SceneProps");

			// setting up batch renderer
			{

				m_QuadBatchData = new QuadVertex[MAX_BATCH_VERTS];
				uint32_t* batchIndices = new uint32_t[MAX_BATCH_INDICES];
				uint32_t offset = 0;
				for (uint32_t i = 0; i < MAX_BATCH_INDICES; i += 6)
				{
					batchIndices[i + 0] = offset + 0;
					batchIndices[i + 1] = offset + 1;
					batchIndices[i + 2] = offset + 2;

					batchIndices[i + 3] = offset + 2;
					batchIndices[i + 4] = offset + 3;
					batchIndices[i + 5] = offset + 0;

					offset += 4;
				}

				VertexBufferLayout batchLayout;
				batchLayout.isDynamic = true;
				batchLayout.Push(ShaderDataType::FLOAT, 3);
				batchLayout.Push(ShaderDataType::FLOAT, 3);
				m_BatchVB = platform->CreateVertexBuffer();
				m_BatchVB->SetLayout(batchLayout);

				m_BatchVB->SetData(0, sizeof(QuadVertex) * MAX_BATCH_VERTS);
				
				m_BatchIB = platform->CreateIndexBuffer();
				m_BatchIB->SetData(batchIndices, MAX_BATCH_INDICES * sizeof(uint32_t));

				delete[] batchIndices;
				m_LastQuadVertexPtr = m_QuadBatchData;


				m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
				m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
				m_QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
				m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
			}
		}

		void Renderer2D::BeginSceneImpl(Camera& camera, glm::mat4& cameraTransform)
		{
			glm::mat4 view = glm::inverse(cameraTransform);
			glm::mat4 projection = camera.GetProjection();
			glm::mat4 viewProjection = projection * view;
			m_Camera = camera;
			m_SceneProps->SetData("sys_viewProjection", viewProjection);

			m_SceneCameraViewProjection = viewProjection;

			StartBatch();
			StartLineBatch();
		}

		void Renderer2D::EndSceneImpl()
		{
			FlushBatch();
			FlushLineBatch();
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
			if (m_QuadBatchIndexCount >= MAX_BATCH_INDICES)
			{
				NewBatch();
			}
			constexpr int quadVertexCount = 4;
			for (size_t i = 0; i < quadVertexCount; i++)
			{
				m_LastQuadVertexPtr->position = transform * m_QuadVertexPositions[i];
				m_LastQuadVertexPtr->color = color;
				m_LastQuadVertexPtr++;
			}

			m_QuadBatchIndexCount += 6;
		}

		void Renderer2D::DrawQuadImpl(glm::vec3 color, glm::mat4& transform, glm::mat4 projection)
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			m_ColorShader->Bind();
			m_ColorShaderProps->SetData("props_color", color);
			m_SceneProps->SetData("sys_viewProjection", projection);
			m_SceneProps->SetData("sys_transform", transform);

			m_QuadVB->Bind();
			m_QuadIB->Bind();
			command->DrawIndexed(PrimitiveType::TRIANGLE, 6);
			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);
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

		void Renderer2D::DrawSpriteImpl(Sprite& sprite, glm::mat4& transform)
		{
			if (sprite.GetMaterial() != nullptr)
			{
				if (sprite.GetMaterial()->isValid())
				{
					auto mintexcoords = sprite.GetMinTextureCoords();
					auto maxtexcoords = sprite.GetMaxTextureCoords();
					float vertices[] = {
						// positions             // texture coords
						 0.5f,  0.5f, 0.0f,      maxtexcoords.x, maxtexcoords.y,    // top right
						 0.5f, -0.5f, 0.0f,      maxtexcoords.x, mintexcoords.y,    // bottom right
						-0.5f, -0.5f, 0.0f,      mintexcoords.x, mintexcoords.y,    // bottom left
						-0.5f,  0.5f, 0.0f,      mintexcoords.x, maxtexcoords.y,    // top left 
					};

					m_QuadVB->SetSubData(0, &vertices, sizeof(vertices));
					auto command = Application::GetRenderPlatform()->GetRenderCommand();
					sprite.GetMaterial()->BindShaders();
					sprite.GetMaterial()->BindTextures();
					m_SceneProps->SetData("sys_transform", transform);

					m_QuadVB->Bind();
					m_QuadIB->Bind();
					
					unsigned int renderFlags = sprite.GetMaterial()->GetRenderFlags();
					if (renderFlags & Material::RenderFlags::BLEND_ENABLE)
					{
						command->EnableBlending();
						
						if (renderFlags & Material::RenderFlags::BLEND_INVERSE_ALPHA)
						{
							command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::INVERSE_SRC_ALPHA);
						}

						if (renderFlags & Material::RenderFlags::BLEND_ALPHA_ADD)
						{
							command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::ONE);
						}

					}

					command->DrawIndexed(PrimitiveType::TRIANGLE, 6);

					if (renderFlags & Material::RenderFlags::BLEND_ENABLE)
					{
						command->DisableBlending();
					}
				}
			}

		}

		void Renderer2D::DrawAnimatedSpriteImpl(AnimatedSprite& sprite, AnimationFrame& frame, glm::mat4& transform)
		{
			if (sprite.GetMaterial() != nullptr)
			{
				if (sprite.GetMaterial()->isValid())
				{
					auto mintexcoords = frame.minTextureCoords;
					auto maxtexcoords = frame.maxTextureCoords;
					float vertices[] = {
						// positions             // texture coords
						 0.5f,  0.5f, 0.0f,      maxtexcoords.x, maxtexcoords.y,    // top right
						 0.5f, -0.5f, 0.0f,      maxtexcoords.x, mintexcoords.y,    // bottom right
						-0.5f, -0.5f, 0.0f,      mintexcoords.x, mintexcoords.y,    // bottom left
						-0.5f,  0.5f, 0.0f,      mintexcoords.x, maxtexcoords.y,    // top left 
					};

					m_QuadVB->SetSubData(0, &vertices, sizeof(vertices));
					auto command = Application::GetRenderPlatform()->GetRenderCommand();
					sprite.GetMaterial()->BindShaders();
					sprite.GetMaterial()->BindTextures();
					m_SceneProps->SetData("sys_transform", transform);

					m_QuadVB->Bind();
					m_QuadIB->Bind();

					unsigned int renderFlags = sprite.GetMaterial()->GetRenderFlags();
					if (renderFlags & Material::RenderFlags::BLEND_ENABLE)
					{
						command->EnableBlending();

						if (renderFlags & Material::RenderFlags::BLEND_INVERSE_ALPHA)
						{
							command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::INVERSE_SRC_ALPHA);
						}

						if (renderFlags & Material::RenderFlags::BLEND_ALPHA_ADD)
						{
							command->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::ONE);
						}
					}

					command->DrawIndexed(PrimitiveType::TRIANGLE, 6);

					if (renderFlags & Material::RenderFlags::BLEND_ENABLE)
					{
						command->DisableBlending();
					}
				}
			}
		}

		void Renderer2D::DrawLineImpl(glm::vec2 point1, glm::vec2 point2, glm::vec3 color)
		{
			if (m_LineBatchVertexCount >= MAX_BATCH_VERTS)
			{
				NewLineBatch();
			}

			m_LastLineVertexPtr->position = point1;
			m_LastLineVertexPtr->color = color;

			m_LastLineVertexPtr++;

			m_LastLineVertexPtr->position = point2;
			m_LastLineVertexPtr->color = color;

			m_LastLineVertexPtr++;

			m_LineBatchVertexCount += 2;

		}

		void Renderer2D::DrawLineImpl(glm::vec2 point1, glm::vec2 point2, glm::vec3 color, glm::mat4& projection)
		{
			auto platform = Application::GetRenderPlatform();
			auto command = platform->GetRenderCommand();

			float verts[] = {
				point1.x, point1.y,
				point2.x, point2.y
			};
			m_SceneProps->SetData("sys_viewProjection", projection);
			m_LineShader->Bind();
			m_LineShaderProps->SetData("props_color", color);
			m_LineVB->SetSubData(0, &verts, sizeof(verts));
			m_LineVB->Bind();
			command->DrawArrays(PrimitiveType::LINE, 2);
		}

		void Renderer2D::StartBatch()
		{
			m_LastQuadVertexPtr = m_QuadBatchData;
			m_QuadBatchIndexCount = 0;
		}

		void Renderer2D::NewBatch()
		{
			FlushBatch();
			StartBatch();
		}

		void Renderer2D::DrawRectImpl(Rect rect, glm::vec3 color, bool filled)
		{
			DrawRectImpl(rect.GetMin(), rect.GetMax(), color, filled);
		}

		void Renderer2D::DrawRectImpl(Rect rect, glm::vec3 color, bool filled, glm::mat4 projection)
		{
			DrawRectImpl(rect.GetMin(), rect.GetMax(), color, filled, projection);
		}


		void Renderer2D::FlushBatch()
		{
			auto command = Application::GetRenderPlatform()->GetRenderCommand();
			m_ColorShader->Bind();

			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);

			uint32_t dataSize = (uint32_t)((uint8_t*)m_LastQuadVertexPtr - (uint8_t*)m_QuadBatchData);
			m_BatchVB->SetSubData(0, m_QuadBatchData, dataSize);
			m_BatchVB->Bind();
			m_BatchIB->Bind();
			command->DrawIndexed(PrimitiveType::TRIANGLE, m_QuadBatchIndexCount);
		}

		void Renderer2D::StartLineBatch()
		{
			m_LastLineVertexPtr = m_LineBatchData;
			m_LineBatchVertexCount = 0;
		}

		void Renderer2D::NewLineBatch()
		{
			FlushLineBatch();
			StartLineBatch();
		}

		void Renderer2D::FlushLineBatch()
		{
			auto platform = Application::GetRenderPlatform();
			auto command = platform->GetRenderCommand();

			m_SceneProps->SetData("sys_viewProjection", m_SceneCameraViewProjection);
			m_LineShader->Bind();
			m_LineVB->SetSubData(0, m_LineBatchData, m_LineBatchVertexCount * sizeof(LineVertex));
			m_LineVB->Bind();
			command->DrawArrays(PrimitiveType::LINE, m_LineBatchVertexCount);
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
				glm::vec3 color = uitext.GetColor();
				m_TexturedRectShaderProps->SetData("tint_color", color);
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
				auto lineShader = assetManager->GetShaderByName("r2d_lineShader");
				m_LineShader = platform->CreateShader(lineShader.absolutePath.c_str());

				UniformBufferLayout layout;
				layout.Push("props_color", ShaderDataType::FLOAT3);
				m_LineShaderProps = platform->CreateUniformBuffer(layout);
				m_LineShaderProps->SetName("line_shader_props");

				m_LineShader->SetUniformBuffer(m_SceneProps);
				m_LineShader->SetUniformBuffer(m_LineShaderProps);

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

