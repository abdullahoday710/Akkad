#include "SceneHierarchyPanel.h"
#include "PropertyEditorPanel.h"
#include "Editor/EditorLayer.h"

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

	}

	void SceneHierarchyPanel::DrawHierarchyPanel()
	{
		ImGui::Begin("Hierarchy", &showPanel);

		auto view = EditorLayer::GetActiveScene()->m_Registry.view<TagComponent>();
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
						Entity e(entity, EditorLayer::GetActiveScene());
						PropertyEditorPanel::SetActiveEntity(e);
					}
					Entity e(entity, EditorLayer::GetActiveScene());
					PropertyEditorPanel::SetActiveEntity(e);
				}
				ImGui::TreePop();
			}

			if (ImGui::BeginPopupContextItem(tag.Tag.c_str()))
			{
				if (ImGui::Button("Delete"))
				{
					EditorLayer::GetActiveScene()->m_Registry.destroy(entity);
				}
				ImGui::EndPopup();
			}
			count++;

		}

		ImGui::End();
	}
}