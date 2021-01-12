#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Renderer2D.h"
#include "Akkad/Scripting/ScriptFactory.h"

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

	void Scene::Render2D()
	{
		auto command = Application::GetRenderPlatform()->GetRenderCommand();
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		auto assetManager = Application::GetAssetManager();
		command->Clear();
		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);
			
			auto texture = assetManager->GetTexture(spriteRenderer.textureID);
			// TODO : support custom shaders
			if (spriteRenderer.shaderID.empty())
			{
				Renderer2D::DrawQuad(texture, transform.GetTransformMatrix());
			}
			transform.RecalculateTransformMatrix();
		}
	}

	void Scene::BeginRenderer2D()
	{
		auto view = m_Registry.view<TransformComponent, CameraComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& camera = view.get<CameraComponent>(entity);
			transform.RecalculateTransformMatrix();
			Renderer2D::BeginScene(camera.camera, transform.GetTransformMatrix());
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

	Entity Scene::GetEntity(entt::entity handle)
	{
		Entity entity = { handle, this };
		if (entity.IsValid())
		{
			return entity;
		}
		else
		{
			AK_ASSERT(false, "Invalid Entity");
		}
	}

}

