#pragma once
#include "Akkad/Graphics/Rect.h"
#include "Akkad/Physics/Box2d/Box2dWorld.h"
#include <entt/entt.hpp>

namespace Akkad {

	namespace Graphics {
		class FrameBuffer;
	}

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
		void SetViewportRect(Graphics::Rect rect) { m_ViewportRect = rect; }

		Graphics::Rect GetViewportRect() { return m_ViewportRect; }
		SharedPtr<Graphics::FrameBuffer> GetPickingBuffer() { return m_PickingBuffer; }

		Entity AddEntity(std::string tag = "Entity");
		void AssignEntityToParent(Entity parent, Entity child);
		bool EntityHasChild(Entity parent, Entity child);
		bool EntityHasHierarchyChild(Entity parent, Entity child);

		void RemoveEntity(Entity entity);
		std::string GetName() { return m_Name; }

	private:

		void BeginRenderer2D(float aspectRatio);
		void Render2D();
		void RenderPickingBuffer2D();

		void UpdateGUIPositions();
		void RenderGUIElement(Entity parent, bool pickingPhase);
		void RenderGUI(bool pickingPhase = false);

		Entity GetEntity(entt::entity handle);
		Entity GetGuiContainer();
		Entity AddGuiContainer();

		entt::registry m_Registry;
		std::string m_Name = "Scene";
		glm::vec2 m_ViewportSize = { 0,0 };
		Box2dWorld m_PhysicsWorld2D;
		SharedPtr<Graphics::FrameBuffer> m_PickingBuffer;
		Graphics::Rect m_ViewportRect;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertyEditorPanel;
		friend class EditorLayer;
		friend class SceneSerializer;
		friend class GameViewPanel;
		friend class ViewPortPanel;
		friend class MaterialEditorPanel;
		friend class TexturePreviewPanel;
		friend class SpriteAnimationPreviewPanel;
		friend class SceneManager;
		friend class GUIFactory;

	};

}