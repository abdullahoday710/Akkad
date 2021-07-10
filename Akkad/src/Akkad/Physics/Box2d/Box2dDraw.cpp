#include "Box2dDraw.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/Renderer2D.h"

#include <vector>
namespace Akkad {

	Box2dDraw::Box2dDraw()
	{
		if (Application::GetAssetManager() != nullptr)
		{
			if (Application::GetRenderPlatform() != nullptr)
			{
				using namespace Graphics;
				auto platform = Application::GetRenderPlatform();
				static auto shader = Application::GetAssetManager()->GetShaderByName("PhysicsDebugShader");
				static SharedPtr<Graphics::Shader> debugShader = platform->CreateShader(shader.absolutePath.c_str());

				Graphics::UniformBufferLayout layout;
				layout.Push("color", Graphics::ShaderDataType::FLOAT3);
				m_DebugShaderProps = Application::GetRenderPlatform()->CreateUniformBuffer(layout);
				m_DebugShaderProps->SetName("shader_props");

				m_DebugShader = debugShader;
				m_DebugShader->SetUniformBuffer(Graphics::Renderer2D::GetSystemUniforms());
				m_DebugShader->SetUniformBuffer(m_DebugShaderProps);

				// creating the polygon vertex buffer;
				{
					VertexBufferLayout bufferLayout;
					bufferLayout.Push(ShaderDataType::FLOAT, 2);
					bufferLayout.isDynamic = true;
					m_PolygonVB = platform->CreateVertexBuffer();
					m_PolygonVB->SetLayout(bufferLayout);
				}
			}
		}
	}

	void Box2dDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygonImp(vertices, vertexCount, color);
	}

	void Box2dDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		DrawPolygonImp(vertices, vertexCount, color);
	}

	void Box2dDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
	}

	void Box2dDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
	}

	void Box2dDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
	}

	void Box2dDraw::DrawTransform(const b2Transform& xf)
	{
	}

	void Box2dDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
	}

	void Box2dDraw::DrawPolygonImp(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		using namespace Graphics;

		auto platfrom = Application::GetRenderPlatform();

		glm::vec3 pass_color = { color.r,color.g,color.b };

		std::vector<float> verts;
		for (int i = 0; i < vertexCount; i++)
		{
			b2Vec2 worldvertex = vertices[i];
			verts.push_back(worldvertex.x);
			verts.push_back(worldvertex.y);
		}

		m_PolygonVB->SetData(verts.data(), verts.size() * 4);

		m_DebugShaderProps->SetData("color", pass_color);

		auto command = platfrom->GetRenderCommand();

		command->SetPolygonMode(PolygonMode::LINE);
		Renderer2D::Draw(m_PolygonVB, m_DebugShader, vertexCount);
		command->SetPolygonMode(PolygonMode::FILL);

	}
}