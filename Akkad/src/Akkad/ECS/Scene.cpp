#include "Scene.h"
#include "Entity.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Renderer2D.h"

#include "Components/Components.h"

namespace Akkad {
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Start()
	{
	}

	void Scene::Update()
	{
		auto camView = m_Registry.view<TransformComponent, CameraComponent>();

		for (auto entity : camView)
		{
			auto& transform = camView.get<TransformComponent>(entity);
			auto& camera = camView.get<CameraComponent>(entity);

			Renderer2D::BeginScene(camera.camera, transform.GetTransformMatrix());
		}

		auto command = Application::GetRenderPlatform()->GetRenderCommand();
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		command->Clear();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);
			
			Renderer2D::DrawQuad(spriteRenderer.color, transform.GetTransformMatrix());
		}
		
	}

	Entity Scene::AddEntity(std::string tag)
	{
		Entity entity = { m_Registry.create(), this };
		// any entity must have a tag and transform component by default !
		auto& tag_comp = entity.AddComponent<TagComponent>();
		auto& transform_comp = entity.AddComponent<TransformComponent>();
		tag_comp.Tag = tag;

		return entity;
	}

}

