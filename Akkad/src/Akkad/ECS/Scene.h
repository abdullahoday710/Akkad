#pragma once

#include <Akkad/Graphics/RenderPlatform.h>
#include <entt/entt.hpp>

namespace Akkad {

	class Entity;

	class Scene {

	public:
		Scene();
		~Scene();

		void Start();

		void Update();

		void Stop();

		void Serialize();
		void Deserialize();
		Entity AddEntity(std::string tag = "Entity");

	private:
		entt::registry m_Registry;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertyEditorPanel;
		friend class EditorLayer;
		friend class SceneSerializer;

	};

}