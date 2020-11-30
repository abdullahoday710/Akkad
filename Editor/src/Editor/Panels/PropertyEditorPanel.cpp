#include "PropertyEditorPanel.h"
#include "../EditorLayer.h"

#include <Akkad/ECS/Components/Components.h>

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

			if (m_ActiveEntity.HasComponent<CameraComponent>())
			{
				DrawCameraComponent();
			}

			if (m_ActiveEntity.HasComponent<ScriptComponent>())
			{
				DrawScriptComponent();
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

				if (ImGui::Button("Camera"))
				{

					if (!m_ActiveEntity.HasComponent<CameraComponent>())
					{
						m_ActiveEntity.AddComponent<CameraComponent>(CameraProjection::Orthographic);
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Script"))
			{
				if (!m_ActiveEntity.HasComponent<ScriptComponent>())
				{
					m_ActiveEntity.AddComponent<ScriptComponent>("Null");
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
		//ImGui::InputFloat3("Color", glm::value_ptr(sprite.color));
		ImGui::ColorPicker3("Color", glm::value_ptr(sprite.color));
	}

	void PropertyEditorPanel::DrawCameraComponent()
	{
		auto& camera = m_ActiveEntity.GetComponent<CameraComponent>();
		ImGui::Text("Camera :");
		ImGui::Checkbox("Active", &camera.isActive);
	}

	void PropertyEditorPanel::DrawScriptComponent()
	{
		std::vector<std::string> scriptNames;
		for (auto it = ScriptFactory::GetInstance().objectmap.begin(); it != ScriptFactory::GetInstance().objectmap.end(); it++)
		{
			scriptNames.push_back(it->first);
		}
		static int item_current_idx = 0;                   
		const char* combo_label = scriptNames[item_current_idx].c_str();

		auto& script = m_ActiveEntity.GetComponent<ScriptComponent>();
		script.ScriptName = scriptNames[item_current_idx];

		if (ImGui::BeginCombo("Script", combo_label))
		{
			for (int i = 0; i < scriptNames.size(); i++)
			{
				const bool is_selected = (item_current_idx == i);
				if (ImGui::Selectable(scriptNames[i].c_str(), is_selected))
				{
					item_current_idx = i;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			
			ImGui::EndCombo();
		}
	}

}