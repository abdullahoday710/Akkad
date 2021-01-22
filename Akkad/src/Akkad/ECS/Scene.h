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

		std::string GetName() { return m_Name; }

	private:
		entt::registry m_Registry;
		std::string m_Name = "Scene";
		void BeginRenderer2D(float aspectRatio);
		void Render2D();
		Entity GetEntity(entt::entity handle);

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertyEditorPanel;
		friend class EditorLayer;
		friend class SceneSerializer;
		friend class GameViewPanel;
		friend class ViewPortPanel;
		friend class SceneManager;

	};

}