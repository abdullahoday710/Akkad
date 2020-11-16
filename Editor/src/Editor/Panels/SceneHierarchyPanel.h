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
		virtual void OnClose() override;

	private:
		SharedPtr<Scene> m_Scene;
		bool showPanel = true;
		void DrawHierarchyPanel();
	};
}


