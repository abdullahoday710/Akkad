#include "Box2dWorld.h"

#include "Akkad/ECS/Scene.h"
#include "Akkad/ECS/Entity.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Graphics/Renderer2D.h"

namespace Akkad {

	Box2dWorld::Box2dWorld()
	{
		b2Vec2 gravity = { m_Gravity.x, m_Gravity.y };
		m_World = CreateSharedPtr<b2World>(gravity);

		if (Application::GetAssetManager() != nullptr)
		{
			if (Application::GetRenderPlatform() != nullptr)
			{
				static auto shader = Application::GetAssetManager()->GetShaderByName("PhysicsDebugShader");
				static SharedPtr<Graphics::Shader> debugShader = Application::GetRenderPlatform()->CreateShader(shader.absolutePath.c_str());

				Graphics::UniformBufferLayout layout;
				layout.Push("color", Graphics::ShaderDataType::FLOAT3);
				m_DebugShaderProps = Application::GetRenderPlatform()->CreateUniformBuffer(layout);
				m_DebugShaderProps->SetName("shader_props");

				m_DebugShader = debugShader;
				m_DebugShader->SetUniformBuffer(Graphics::Renderer2D::GetSystemUniforms());
				m_DebugShader->SetUniformBuffer(m_DebugShaderProps);
				
			}
		}
	}

	Box2dWorld::Box2dWorld(glm::vec2 gravity)
	{
		b2Vec2 b2gravity = {gravity.x, gravity.y};
		m_World = CreateSharedPtr<b2World>(b2gravity);
	}

	Box2dWorld::~Box2dWorld()
	{
	}

	Box2dBody Box2dWorld::CreateBody(BodySettings settings, Scene* scene, uint32_t entityid)
	{
		b2BodyDef bodyDef;
		Entity* entity = new Entity((entt::entity)entityid, scene);

		bodyDef.position.Set(settings.position.x, settings.position.y);
		bodyDef.angle = settings.rotation;
		bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);
		
		switch (settings.type)
		{
		case BodyType::STATIC:
			bodyDef.type = b2_staticBody;
			break;
		case BodyType::DYNAMIC:
			bodyDef.type = b2_dynamicBody;
			break;
		}

		b2Body* body = m_World->CreateBody(&bodyDef);
		b2FixtureDef fixtureDef;

		fixtureDef.density = settings.density;
		fixtureDef.friction = settings.friction;
		switch (settings.shape)
		{
		case BodyShape::POLYGON_SHAPE:
		{
			b2PolygonShape polygon;
			polygon.SetAsBox(settings.halfX, settings.halfY);
			fixtureDef.shape = &polygon;
			body->CreateFixture(&fixtureDef);
			break;
		}

		}

		return Box2dBody(body);
	}

	void Box2dWorld::SetContactListener(Box2dContactListener* listener)
	{
		m_World->SetContactListener(listener);
	}

	void Box2dWorld::Step()
	{
		float timeStep = 1.0f / 60.0f;
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		m_World->Step(timeStep, velocityIterations, positionIterations);
	}

	void Box2dWorld::Clear()
	{
		m_World.reset(new b2World({m_Gravity.x, m_Gravity.y}));
	}

	void Box2dWorld::DebugDrawBody(b2Body* body)
	{
		using namespace Graphics;
		if (body != nullptr)
		{
			for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
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
							b2Vec2 worldvertex = body->GetWorldPoint(polygonShape->m_vertices[i]);
							verts.push_back(worldvertex.x);
							verts.push_back(worldvertex.y);
						}

						auto platform = Application::GetRenderPlatform();
						auto command = platform->GetRenderCommand();

						m_DebugVertexBuffer = platform->CreateVertexBuffer();
						m_DebugVertexBuffer->SetLayout(bufferLayout);
						m_DebugVertexBuffer->SetData(verts.data(), verts.size() * 4);

						glm::vec3 color{ 1,0,0 };
						if (body->IsAwake())
						{
							color = { 0,1,0 };
						}

						m_DebugShaderProps->SetData("color", color);

						command->SetPolygonMode(PolygonMode::LINE);
						Renderer2D::Draw(m_DebugVertexBuffer, m_DebugShader, polygonShape->m_count);
						command->SetPolygonMode(PolygonMode::FILL);

					}
				}

			}
		}
	}
}