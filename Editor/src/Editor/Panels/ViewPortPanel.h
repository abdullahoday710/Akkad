#pragma once

#include "Panel.h"
#include "Editor/EditorCamera.h"

#include "Akkad/core.h"
#include "Akkad/Graphics/Rect.h"
#include "Akkad/ECS/Entity.h"

namespace Akkad {
	namespace Graphics {
		class FrameBuffer;
	}
	class ViewPortPanel : public Panel
	{
	public:
		ViewPortPanel();
		virtual ~ViewPortPanel();

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() { return "viewport"; }

		bool IsPlaying = false;

		void SetSelectedEntity(Entity selectedEntity)
		{
			m_SelectedEntity = selectedEntity;
		}
	private:
		static bool showPanel;
		float m_ViewportAspectRatio = 0.0f;
		EditorCamera m_EditorCamera;
		SharedPtr<Graphics::FrameBuffer> m_buffer;
		Graphics::Rect m_ViewportRect;
		Entity m_SelectedEntity;

		void OnScenePlay();
		void OnSceneStop();

		void RenderScene();

		friend class EditorLayer;
		friend class GameViewPanel;
	};
}

