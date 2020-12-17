#include "PropertyEditorPanel.h"
#include "../EditorLayer.h"

#include <Akkad/ECS/Components/Components.h>

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Akkad {
	bool PropertyEditorPanel::showPanel;

	bool AddComponentsOpen = false;
	Entity PropertyEditorPanel::m_ActiveEntity;

	PropertyEditorPanel::PropertyEditorPanel()
	{
	}

	PropertyEditorPanel::~PropertyEditorPanel()
	{
	}

	template <typename T>
	bool DrawComponentContextMenu(Entity e)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Delete"))
			{
				e.RemoveComponent<T>();
				ImGui::EndPopup();
				return true;
			}

			ImGui::EndPopup();
			return false;
		}
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

			if (ImGui::Button("Script"))
			{
				if (!m_ActiveEntity.HasComponent<ScriptComponent>())
				{
					m_ActiveEntity.AddComponent<ScriptComponent>("");
				}
			}
		
		}
	}

	void PropertyEditorPanel::DrawTagComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Tag"))
		{
			auto& tag = m_ActiveEntity.GetComponent<TagComponent>();
			std::string& str = tag.Tag;
			ImGui::InputText("Tag", &str);

			ImGui::TreePop();
		}
		
	}

	void PropertyEditorPanel::DrawTransformComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Transform"))
		{
			auto& transform = m_ActiveEntity.GetComponent<TransformComponent>();
			ImGui::DragFloat3("Position", glm::value_ptr(transform.GetPosition()));
			transform.RecalculateTransformMatrix();
			ImGui::TreePop();
		}


	}

	void PropertyEditorPanel::DrawSpriteRendererComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Sprite Renderer"))
		{
			if (DrawComponentContextMenu<SpriteRendererComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& sprite = m_ActiveEntity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorPicker3("Color", glm::value_ptr(sprite.color));
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawCameraComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Camera"))
		{
			if (DrawComponentContextMenu<CameraComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& camera = m_ActiveEntity.GetComponent<CameraComponent>();
			ImGui::Text("Camera :");
			ImGui::Checkbox("Active", &camera.isActive);
			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawScriptComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Script"))
		{
			if (DrawComponentContextMenu<ScriptComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			std::vector<std::string> scriptNames;
			for (auto it = ScriptFactory::GetInstance().objectmap.begin(); it != ScriptFactory::GetInstance().objectmap.end(); it++)
			{
				scriptNames.push_back(it->first);
			}
			static int item_current_idx = 0;
			const char* combo_label = scriptNames[item_current_idx].c_str();

			auto& script = m_ActiveEntity.GetComponent<ScriptComponent>();
			if (script.ScriptName.empty())
			{
				script.ScriptName = scriptNames[item_current_idx];
			}

			else
			{
				ptrdiff_t pos = std::find(scriptNames.begin(), scriptNames.end(), script.ScriptName) - scriptNames.begin();
				if (pos <= scriptNames.size())
				{
					item_current_idx = pos;
				}
			}

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
			ImGui::TreePop();
		}

	}

}