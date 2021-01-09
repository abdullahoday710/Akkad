#pragma once

#include <Akkad/Graphics/RenderPlatform.h>
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

		Entity AddEntity(std::string tag = "Entity");

	private:
		entt::registry m_Registry;
		std::string m_Name = "Scene";
		void BeginRenderer2D();
		void Render2D();
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertyEditorPanel;
		friend class EditorLayer;
		friend class SceneSerializer;
		friend class GameViewPanel;

	};

}