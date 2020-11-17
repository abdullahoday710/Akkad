#pragma once
#include "Panel.h"
#include <Akkad/core.h>
#include <Akkad/ECS/Scene.h>

namespace Akkad {

	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(SharedPtr<Scene> scene);
		~SceneHierarchyPanel() {}

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

	private:
		SharedPtr<Scene> m_Scene;
		static bool showPanel;
		void DrawHierarchyPanel();
	};
}


