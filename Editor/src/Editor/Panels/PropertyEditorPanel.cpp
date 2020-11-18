#include "PropertyEditorPanel.h"

#include <Akkad/ECS/Components/TagComponent.h>
#include <Akkad/ECS/Components/TransformComponent.h>
#include <Akkad/ECS/Components/SpriteRendererComponent.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.cpp>

namespace Akkad {
	bool PropertyEditorPanel::showPanel;

	bool AddComponentsOpen = false;
	Entity PropertyEditorPanel::m_ActiveEntity;

	PropertyEditorPanel::PropertyEditorPanel(Scene* scene)
	{
		m_Scene = scene;
	}

	PropertyEditorPanel::~PropertyEditorPanel()
	{
	}

	void PropertyEditorPanel::DrawImGui()
	{
		if (!showPanel)
		{
			OnClose();
			return;
		}

		if (!ImGui::Begin("Property Editor", &showPanel))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Properties: ");
		if (m_ActiveEntity.IsValid())
		{
			DrawTagComponent();
			DrawTransformComponent();

			if (m_ActiveEntity.HasComponent<SpriteRendererComponent>())
			{
				DrawSpriteRendererComponent();
			}

			DrawAddComponent();
		}
		

		ImGui::End();
	}
	

	void PropertyEditorPanel::OnClose()
	{
		PanelManager::RemovePanel(this);
	}

	void PropertyEditorPanel::DrawAddComponent()
	{
		if (ImGui::Button("Add component"))
		{
			AddComponentsOpen = !AddComponentsOpen;
		}

		if (AddComponentsOpen)
		{
			ImGui::Separator();
			if (ImGui::TreeNode("2D"))
			{
				if (ImGui::Button("Sprite Renderer"))
				{
					if (!m_ActiveEntity.HasComponent<SpriteRendererComponent>())
					{
						m_ActiveEntity.AddComponent<SpriteRendererComponent>();
					}
					
				}
				ImGui::TreePop();
			}
		
		}
	}

	void PropertyEditorPanel::DrawTagComponent()
	{
		auto& tag = m_ActiveEntity.GetComponent<TagComponent>();
		std::string& str = tag.Tag;
		ImGui::InputText("Tag", &str);
	}

	void PropertyEditorPanel::DrawTransformComponent()
	{
		auto& transform = m_ActiveEntity.GetComponent<TransformComponent>();
		ImGui::InputFloat3("Transform", glm::value_ptr(transform.GetPosition()));
		transform.RecalculateTransformMatrix();

	}

	void PropertyEditorPanel::DrawSpriteRendererComponent()
	{
		auto& sprite = m_ActiveEntity.GetComponent<SpriteRendererComponent>();
		ImGui::InputFloat3("Color", glm::value_ptr(sprite.color));
	}

}