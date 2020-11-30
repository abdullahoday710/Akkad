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
		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);


				if (script.Instance == nullptr)
				{
					script.Instance = ScriptFactory::GetInstance().createObject(script.ScriptName);
					Entity e(entity, this);
					script.Instance->m_Entity = e;
					script.Instance->OnStart();
				}
			}
		}
	}

	void Scene::Update()
	{
		// Update scripts
		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);

				script.Instance->OnUpdate();
			}

		}

		// Begin scene
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& camera = view.get<CameraComponent>(entity);

				Renderer2D::BeginScene(camera.camera, transform.GetTransformMatrix());
			}
		}

		// Render
		{
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
		
		
	}

	void Scene::Stop()
	{
		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);

				delete script.Instance;
				script.Instance = nullptr;
			}
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

