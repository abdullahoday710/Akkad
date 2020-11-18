#include "Scene.h"
#include "Entity.h"

#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Renderer2D.h"

#include "Components/TagComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"

namespace Akkad {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Init()
	{
		auto platform = Application::GetRenderPlatform();
		m_colorShader = platform->CreateShader("res/shaders/colorShader.glsl");
	}

	void Scene::Update()
	{
		auto command = Application::GetRenderPlatform()->GetRenderCommand();
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		command->Clear();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);

			m_colorShader->Bind();
			m_colorShader->SetMat4("transform", transform.GetTransformMatrix());
			Renderer2D::DrawQuad(m_colorShader, spriteRenderer.color);
		}
	}

	Entity Scene::AddEntity(std::string tag)
	{
		Entity entity = { m_Registry.create(), this };
		// any entity must have a tag and transform component by default !
		auto& tag_comp = entity.AddComponent<TagComponent>();
		auto& transform_comp = entity.AddComponent<TransformComponent>();
		auto& spriterRenderer_comp = entity.AddComponent<SpriteRendererComponent>();
		tag_comp.Tag = tag;

		return entity;
	}

}

