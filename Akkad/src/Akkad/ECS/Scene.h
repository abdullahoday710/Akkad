#pragma once
#include "Akkad/Physics/Box2d/Box2dWorld.h"
#include <entt/entt.hpp>

namespace Akkad {

	class Entity;

	class Scene {

	public:
		Scene();
		Scene(std::string& name) : m_Name(name) {}
		~Scene();

		void Start();
		void Update();
		void Stop();
		void SetViewportSize(glm::vec2 size);
		Entity AddEntity(std::string tag = "Entity");
		Entity AddEntityToParent(Entity parent, Entity child, std::string tag = "Child");
		void RemoveEntity(Entity entity);
		std::string GetName() { return m_Name; }

	private:
		entt::registry m_Registry;
		std::string m_Name = "Scene";
		void BeginRenderer2D(float aspectRatio);
		void Render2D();
		void RenderPickingBuffer2D();
		void RenderGUI();
		glm::vec2 m_ViewportSize;
		Entity GetEntity(entt::entity handle);
		Box2dWorld m_PhysicsWorld2D;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertyEditorPanel;
		friend class EditorLayer;
		friend class SceneSerializer;
		friend class GameViewPanel;
		friend class ViewPortPanel;
		friend class MaterialEditorPanel;
		friend class SceneManager;

	};

}