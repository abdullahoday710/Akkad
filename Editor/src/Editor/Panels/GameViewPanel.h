#pragma once
#include "Panel.h"

#include "Akkad/core.h"
#include "Akkad/Graphics/Rect.h"
namespace Akkad {

	namespace Graphics {
		class FrameBuffer;
	}

	class GameViewPanel : public Panel
	{
	public:
		GameViewPanel();
		~GameViewPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };
		virtual std::string GetName() override { return "Game View"; }

		bool IsSelected = false;

	private:
		static bool showPanel;
		float m_AspectRatio;
		void RenderScene();
		SharedPtr<Graphics::FrameBuffer> m_buffer;
		Graphics::Rect m_ViewportRect;

		friend class EditorLayer;
	};

}

