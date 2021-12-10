#pragma once
#include "Panel.h"

#include <Akkad/core.h>
#include <Akkad/ECS/Entity.h>
#include <Akkad/ECS/Scene.h>
namespace Akkad {

	class PropertyEditorPanel : public Panel
	{
	public:
		PropertyEditorPanel();
		~PropertyEditorPanel();
		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };
	private:
		static bool showPanel;
		friend class SceneHierarchyPanel;
		friend class NewScenePanel;
		friend class EditorLayer;
		friend class ViewPortPanel;

		static Entity m_ActiveEntity;

		static void SetActiveEntity(Entity e) { m_ActiveEntity = e; }

		void DrawAddComponent();

		void DrawTagComponent();
		void DrawTransformComponent();
		void DrawSpriteRendererComponent();
		void DrawCameraComponent();
		void DrawScriptComponent();
		void DrawRigidBody2dComponent();
		void DrawGUITextComponent();
		void DrawGUIContainerComponent();
		void DrawRectTransformComponent();
		void DrawGUIButtonComponent();
		void DrawAnimatedSpriteRendererComponent();
		void DrawHingeJoint2DComponent();
		void DrawLineRendererComponent();
		void DrawColoredSpriteRendererComponent();

	};
}


