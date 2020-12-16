#include "SceneHierarchyPanel.h"
#include "PropertyEditorPanel.h"
#include "../EditorLayer.h"

#include <Akkad/ECS/Components/TagComponent.h>
#include <imgui.h>
#include <entt/entt.hpp>
namespace Akkad {
	bool SceneHierarchyPanel::showPanel;

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
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

		auto view = EditorLayer::s_ActiveScene->m_Registry.view<TagComponent>();
		unsigned int count = 0;

		for (auto entity : view)
		{
			auto& tag = view.get<TagComponent>(entity);

			if (ImGui::TreeNode((void*)count, tag.Tag.c_str()))
			{
				if (ImGui::IsItemToggledOpen())
				{
					if (!PropertyEditorPanel::showPanel)
					{
						PropertyEditorPanel* panel = new PropertyEditorPanel();
						PanelManager::AddPanel(panel);
						Entity e(entity, EditorLayer::s_ActiveScene);
						PropertyEditorPanel::SetActiveEntity(e);
					}
					Entity e(entity, EditorLayer::s_ActiveScene);
					PropertyEditorPanel::SetActiveEntity(e);
				}
				ImGui::TreePop();
			}

			if (ImGui::BeginPopupContextItem(tag.Tag.c_str()))
			{
				if (ImGui::Button("Delete"))
				{
					EditorLayer::s_ActiveScene->m_Registry.destroy(entity);
				}
				ImGui::EndPopup();
			}
			count++;

		}

		ImGui::End();
	}
}