#include "GUIText.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/RenderPlatform.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Camera.h"
#include "Akkad/Graphics/Renderer2D.h"

#include <glad/glad.h>
namespace Akkad {
	namespace GUI {
		/*
		void GUIText::Render(float x, float y, float scale, glm::vec3 color, glm::mat4 projection)
		{
			using namespace Graphics;
			auto renderCommand = Application::GetRenderPlatform()->GetRenderCommand();

			VertexBufferLayout layout;
			layout.Push(ShaderDataType::FLOAT, 4);

			static auto shaderasset = Application::GetAssetManager()->GetShaderByName("r2d_guitext");
			static auto shader = Application::GetAssetManager()->GetShader(shaderasset.assetID);
			UniformBufferLayout ublayout;
			ublayout.Push("projection", ShaderDataType::MAT4);
			ublayout.Push("text_color", ShaderDataType::FLOAT3);
			static auto ub = Application::GetRenderPlatform()->CreateUniformBuffer(ublayout);
			ub->SetName("shader_props");
			ub->SetData("projection", projection);
			ub->SetData("text_color", color);
			shader->Bind();
			shader->SetUniformBuffer(ub);

			std::string::const_iterator c;
			renderCommand->EnableBlending();
			renderCommand->SetBlendState(BlendSourceFactor::ALPHA, BlendDestFactor::INVERSE_SRC_ALPHA);
			for (c = m_Text.begin(); c != m_Text.end(); c++)
			{
				auto VertexBuffer = Application::GetRenderPlatform()->CreateVertexBuffer();
				VertexBuffer->SetLayout(layout);

				Font::FontCharacter ch = m_Font->GetCharacter(*c);

				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;

				auto fontAtlasSize = m_Font->GetTextureAtlasSize();

				float vertices[6][4] = {
					{ xpos,     ypos + h,   ch.xOffset / fontAtlasSize.x, 0.0f },
					{ xpos,     ypos,       ch.xOffset / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },
					{ xpos + w, ypos,       (ch.xOffset + ch.Size.x) / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },

					{ xpos,     ypos + h,   ch.xOffset / fontAtlasSize.x, 0.0f },
					{ xpos + w, ypos,       (ch.xOffset + ch.Size.x) / fontAtlasSize.x, ch.Size.y / fontAtlasSize.y },
					{ xpos + w, ypos + h,   (ch.xOffset + ch.Size.x) / fontAtlasSize.x, 0.0f }
				};

				VertexBuffer->SetData(vertices, sizeof(vertices));
				VertexBuffer->Bind();
				m_Font->GetAtlas()->Bind(0);
				Application::GetRenderPlatform()->GetRenderCommand()->DrawArrays(PrimitiveType::TRIANGLE, 6);
				x += (ch.Advance >> 6) * scale;
			}
			renderCommand->DisableBlending();
				
		}*/
	}
}

