#pragma once

#include "Panel.h"
#include "Editor/EditorCamera.h"

#include "Akkad/core.h"

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
	private:
		static bool showPanel;
		float m_ViewportAspectRatio = 0.0f;
		EditorCamera m_EditorCamera;
		SharedPtr<Graphics::FrameBuffer> m_buffer;
		void OnScenePlay();
		void OnSceneStop();

		void RenderScene();

		friend class EditorLayer;
		friend class GameViewPanel;
	};
}

