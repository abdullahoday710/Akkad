#include "SceneHierarchyPanel.h"
#include "PropertyEditorPanel.h"
#include "ViewPortPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/ECS/SceneManager.h>
#include <Akkad/ECS/Components/TagComponent.h>

#include <imgui.h>
#include <imgui_internal.h>
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
		showPanel = false;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		if (entity.IsValid())
		{
			auto entity_tag = entity.GetComponent<TagComponent>();
			auto entity_relation = entity.GetComponent<RelationShipComponent>();

			bool node_open = ImGui::TreeNode((void*)entity.m_Handle, entity_tag.Tag.c_str());

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::Text(entity_tag.Tag.c_str());
				uint32_t entity_id = (uint32_t)entity.m_Handle;
				ImGui::SetDragDropPayload("HIERARCHY_DRAG_DROP", &entity_id, sizeof(size_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto scene = EditorLayer::GetActiveScene();
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_DROP"))
				{
					uint32_t* entity_id = (uint32_t*)payload->Data;

					if ((entt::entity)*entity_id != entity.m_Handle)
					{
						Entity dragged_child = EditorLayer::GetActiveScene()->GetEntity((entt::entity)*entity_id);
						scene->AssignEntityToParent(entity, dragged_child);
						
					}

				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (!PropertyEditorPanel::showPanel)
				{
					PropertyEditorPanel* panel = new PropertyEditorPanel();
					PanelManager::AddPanel(panel);
				}

				PropertyEditorPanel::SetActiveEntity(entity);

				ViewPortPanel* viewport = (ViewPortPanel*)PanelManager::GetPanel("viewport");
				if (viewport != nullptr)
				{
					viewport->SetSelectedEntity(entity);
				}
				
			}
			DrawEntityContextMenu(entity);
			if (node_open)
			{
				Entity current_child = entity_relation.first_child;
				for (size_t i = 0; i < entity_relation.children; i++)
				{
					if (current_child.IsValid())
					{
						DrawEntityNode(current_child);
						if (current_child.IsValid())
						{
							auto current_child_relation = current_child.GetComponent<RelationShipComponent>();
							current_child = current_child_relation.next;
						}
					}
				}

				ImGui::TreePop();
			}
			
		}
	}

	void SceneHierarchyPanel::DrawEntityContextMenu(Entity entity)
	{
		entt::entity entityID = entity.m_Handle;

		std::string entity_id_str = std::to_string((size_t)entity.m_Handle);

		if(ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Delete"))
			{
				ViewPortPanel* viewport = (ViewPortPanel*)PanelManager::GetPanel("viewport");
				if (viewport->IsPlaying)
				{
					Application::GetSceneManager()->GetActiveScene()->RemoveEntity(entity);
				}
				else
				{
					EditorLayer::GetActiveScene()->RemoveEntity(entity);
				}
			}

			if (ImGui::Button("Add child entity"))
			{
				auto scene = EditorLayer::GetActiveScene();
				Entity parent = scene->GetEntity(entityID);
				Entity child = scene->AddEntity("child");

				scene->AssignEntityToParent(parent, child);
			}
			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DrawHierarchyPanel()
	{
		ImGui::Begin("Hierarchy", &showPanel);
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();
		ImRect windowrect = { min, max };
		
		if (ImGui::BeginDragDropTargetCustom(windowrect, ImGui::GetID("Hierarchy")))
		{
			auto scene = EditorLayer::GetActiveScene();
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_DROP"))
			{
				size_t* entity_id = (size_t*)payload->Data;

				Entity dragged_child = EditorLayer::GetActiveScene()->GetEntity((entt::entity) * entity_id);
				scene->AssignEntityToParent({}, dragged_child);


			}
			ImGui::EndDragDropTarget();
		}

		SharedPtr<Scene> scene;
		ViewPortPanel* viewport = (ViewPortPanel*)PanelManager::GetPanel("viewport");


		if (viewport->IsPlaying)
		{
			scene = Application::GetSceneManager()->GetActiveScene();
		}

		else
		{
			scene = EditorLayer::GetActiveScene();
		}

		auto view = scene->m_Registry.view<TagComponent, RelationShipComponent>();

		unsigned int count = 0;
		for (auto entity : view)
		{
			Entity parentless = scene->GetEntity(entity);

			auto& parentless_relation = parentless.GetComponent<RelationShipComponent>();

			if (!parentless_relation.parent.IsValid())
			{
				DrawEntityNode(parentless);
			}
			count++;

		}

		ImGui::End();
	}

}