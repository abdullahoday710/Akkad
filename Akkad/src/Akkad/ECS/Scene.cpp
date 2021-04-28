#include "Scene.h"
#include "Entity.h"
#include "SceneSerializer.h"

#include "Akkad/Logging.h"
#include "Akkad/Application/Application.h"
#include "Akkad/Graphics/Renderer2D.h"
#include "Akkad/Asset/AssetManager.h"
#include "Akkad/Scripting/LoadedGameAssembly.h"
#include "Akkad/Graphics/SortingLayer2D.h"

#include "Components/Components.h"

namespace Akkad {
	using namespace Graphics;
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Start()
	{
		// Initialize physics
		{
			m_PhysicsWorld2D.Clear();
			auto view = m_Registry.view<TransformComponent,RigidBody2dComponent>();

			for (auto entity : view)
			{
				auto& rigidbody2dcomp = view.get<RigidBody2dComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);

				BodySettings settings;
				settings.density = rigidbody2dcomp.density;
				settings.friction = rigidbody2dcomp.friction;
				settings.shape = rigidbody2dcomp.shape;
				settings.type = rigidbody2dcomp.type;

				settings.position = { transform.GetPosition().x, transform.GetPosition().y };
				settings.rotation = { transform.GetRotation().z };

				settings.halfX = transform.GetScale().x / 2;
				settings.halfY = transform.GetScale().y / 2;

				rigidbody2dcomp.body = m_PhysicsWorld2D.CreateBody(settings);
			}
			
		}
		
		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);


				if (script.Instance == nullptr)
				{
					auto gameAssembly = Application::GetGameAssembly();
					script.Instance = gameAssembly->InstantiateScript(script.ScriptName.c_str());
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
		command->Clear();

		for (auto it : SortingLayer2DHandler::GetRegisteredLayers())
		{
			for (auto entity : view)
			{
				auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);

				if (spriteRenderer.sortingLayer == it.name)
				{
					auto& transform = view.get<TransformComponent>(entity);

					Renderer2D::DrawQuad(spriteRenderer.material, transform.GetTransformMatrix());
					transform.RecalculateTransformMatrix();
					if (m_Registry.has<RigidBody2dComponent>(entity))
					{
						auto& rigidbody2dcomponent = m_Registry.get<RigidBody2dComponent>(entity);
						rigidbody2dcomponent.body.DrawBoundingBox();
					}
				}

			}
		}

	}

	void Scene::RenderPickingBuffer2D()
	{
		auto command = Application::GetRenderPlatform()->GetRenderCommand();

		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		command->Clear();

		for (auto it : SortingLayer2DHandler::GetRegisteredLayers())
		{
			for (auto entity : view)
			{
				auto& spriteRenderer = view.get<SpriteRendererComponent>(entity);

				if (spriteRenderer.sortingLayer == it.name)
				{
					auto& transform = view.get<TransformComponent>(entity);

					uint32_t entityID = (uint32_t)entity;

					entityID += 1;

					glm::vec3 color = {entityID , entityID , entityID};
					Renderer2D::DrawQuad(color, transform.GetTransformMatrix());
				}

			}
		}
		
	}

	void Scene::RenderGUI()
	{
		auto containerView = m_Registry.view<GUIContainerComponent>();
		glm::mat4 projection(1.0f);
		for (auto entity : containerView)
		{
			auto& container = containerView.get<GUIContainerComponent>(entity);
			container.container.SetScreenSize(m_ViewportSize);
			projection = container.container.GetProjection();
			break;
		}
		auto view = m_Registry.view<TransformComponent, GUITextComponent>();

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& guitext = view.get<GUITextComponent>(entity);
			guitext.text.SetPosition({ transform.GetPosition().x, transform.GetPosition().y });
			Renderer2D::RenderText(guitext.text, guitext.text.GetPosition(), 1.0f, guitext.textColor, projection);
		}
	}

	void Scene::BeginRenderer2D(float aspectRatio)
	{
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		bool foundCamera = false;

		for (auto entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& camera = view.get<CameraComponent>(entity);

			if (camera.isActive)
			{
				transform.RecalculateTransformMatrix();
				camera.camera.SetAspectRatio(aspectRatio);
				Renderer2D::BeginScene(camera.camera, transform.GetTransformMatrix());
				break;
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
				if (script.Instance != nullptr)
				{
					script.Instance->OnUpdate();
				}
				
			}
		}

		// Update physics
		{
			m_PhysicsWorld2D.Step();

			auto view = m_Registry.view<TransformComponent, RigidBody2dComponent>();

			for (auto entity : view)
			{
				auto& transform = view.get<TransformComponent>(entity);
				auto& rigidbody2dcomponent = view.get<RigidBody2dComponent>(entity);

				glm::vec2 position = rigidbody2dcomponent.body.GetPosition();
				float rotation = rigidbody2dcomponent.body.GetRotation();
				transform.SetPostion({position.x, position.y, 0.0f});
				transform.SetRotation({ 0, 0, rotation });

			}
		}
		
	}

	void Scene::Stop()
	{

		{
			m_PhysicsWorld2D.Clear();
			auto view = m_Registry.view<RigidBody2dComponent>();
			for (auto entity : view)
			{
				auto& rigidbody2dcomponent = view.get<RigidBody2dComponent>(entity);

				rigidbody2dcomponent.body = Box2dBody();
			}
		}

		{
			auto view = m_Registry.view<ScriptComponent>();

			for (auto entity : view)
			{
				auto& script = view.get<ScriptComponent>(entity);

				if (script.Instance != nullptr)
				{
					delete script.Instance;
					script.Instance = nullptr;
				}

			}
		}
		
	}

	void Scene::SetViewportSize(glm::vec2 size)
	{
		m_ViewportSize = size;
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

