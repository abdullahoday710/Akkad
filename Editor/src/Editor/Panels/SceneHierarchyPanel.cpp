#include "SceneHierarchyPanel.h"
#include <Akkad/ECS/Components/TagComponent.h>
#include <imgui.h>
#include <entt/entt.hpp>
namespace Akkad {

	SceneHierarchyPanel::SceneHierarchyPanel(SharedPtr<Scene> scene)
	{
		m_Scene = scene;
	}

	void SceneHierarchyPanel::DrawImGui()
	{
		DrawHierarchyPanel();
	}

	void SceneHierarchyPanel::OnClose()
	{
		PanelManager::RemovePanel(this);
	}

	void SceneHierarchyPanel::DrawHierarchyPanel()
	{
		if (!showPanel)
		{
			OnClose();
			return;
		}

		if (!ImGui::Begin("Hierarchy", &showPanel))
		{
			ImGui::End();
			return;
		}
		auto view = m_Scene->m_Registry.view<TagComponent>();
		unsigned int count = 0;

		for (auto entity : view)
		{
			auto& tag = view.get<TagComponent>(entity);
			if (ImGui::TreeNode((void*)count, tag.Tag.c_str()))
			{
				if (ImGui::IsItemToggledOpen())
				{
					// TODO : open a property editor
				}
				ImGui::TreePop();
			}
			count++;

		}

		ImGui::End();
	}
}