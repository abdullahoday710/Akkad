#include "Box2dBody.h"
#include "Akkad/Graphics/Renderer2D.h"
#include "Akkad/Application/Application.h"
namespace Akkad {
	SharedPtr<Graphics::Shader> Box2dBody::m_DebugShader;

	Box2dBody::Box2dBody(b2Body* body)
	{

		static SharedPtr<Graphics::Shader> debugShader = Application::GetRenderPlatform()->CreateShader("res/shaders/PhysicsDebugShader.shaderdesc");
		m_Body = body;
		m_DebugShader = debugShader;
		m_DebugShader->SetUniformBuffer(Graphics::Renderer2D::GetSystemUniforms());
	}
	bool Box2dBody::IsValid()
	{
		if (m_Body != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	glm::vec2 Box2dBody::GetPosition()
	{
		AK_ASSERT(IsValid(), "invalid body !");
		b2Vec2 pos = m_Body->GetPosition();

		return {pos.x, pos.y};
	}

	float Box2dBody::GetRotation()
	{
		return m_Body->GetAngle();
	}

	void Box2dBody::DrawBoundingBox()
	{
		using namespace Graphics;

		if (IsValid())
		{
			for (b2Fixture* f = m_Body->GetFixtureList(); f; f = f->GetNext())
			{
				if (f != nullptr)
				{
					b2Shape::Type shapeType = f->GetType();
					if (shapeType == b2Shape::e_circle)
					{
						b2CircleShape* circleShape = (b2CircleShape*)f->GetShape();
					}

					else if (shapeType == b2Shape::e_polygon)
					{
						VertexBufferLayout bufferLayout;

						b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
						std::vector<float> verts;
						bufferLayout.Push(ShaderDataType::FLOAT, 2);
						for (int i = 0; i < polygonShape->m_count; i++)
						{
							b2Vec2 worldvertex = m_Body->GetWorldPoint(polygonShape->m_vertices[i]);
							verts.push_back(worldvertex.x);
							verts.push_back(worldvertex.y);
						}

						auto platform = Application::GetRenderPlatform();
						auto command = platform->GetRenderCommand();

						m_VertexBuffer = platform->CreateVertexBuffer();
						m_VertexBuffer->SetLayout(bufferLayout);
						m_VertexBuffer->SetData(verts.data(), verts.size() * 4);

						glm::vec3 trans = { m_Body->GetTransform().p.x, m_Body->GetTransform().p.y, 1.0f };

						command->SetPolygonMode(PolygonMode::LINE);
						Renderer2D::Draw(m_VertexBuffer, m_DebugShader, polygonShape->m_count);
						command->SetPolygonMode(PolygonMode::FILL);

					}
				}

			}
		}
		
	}
}