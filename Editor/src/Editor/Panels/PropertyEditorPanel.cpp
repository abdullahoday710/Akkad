#include "PropertyEditorPanel.h"
#include "MaterialEditorPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/MaterialSerializer.h"

#include <Akkad/Application/Application.h>
#include <Akkad/Scripting/LoadedGameAssembly.h>
#include <Akkad/ECS/Components/Components.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/Graphics/SortingLayer2D.h>

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
		ImGui::Begin("Property Editor", &showPanel);

		ImGui::Text("Properties: ");
		if (m_ActiveEntity.IsValid())
		{
			DrawTagComponent();

			if (m_ActiveEntity.HasComponent<TransformComponent>())
			{
				DrawTransformComponent();
			}

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

			if (m_ActiveEntity.HasComponent<RigidBody2dComponent>())
			{
				DrawRigidBody2dComponent();
			}

			if (m_ActiveEntity.HasComponent<GUITextComponent>())
			{
				DrawGUITextComponent();
			}

			if (m_ActiveEntity.HasComponent<GUIContainerComponent>())
			{
				DrawGUIContainerComponent();
			}

			if (m_ActiveEntity.HasComponent<RectTransformComponent>())
			{
				DrawRectTransformComponent();
			}

			DrawAddComponent();
		}
		

		ImGui::End();
	}

	void PropertyEditorPanel::OnClose()
	{
		showPanel = false;
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

				if (ImGui::Button("Rigidbody 2D"))
				{
					if (!m_ActiveEntity.HasComponent<RigidBody2dComponent>())
					{
						m_ActiveEntity.AddComponent<RigidBody2dComponent>();
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
			ImGui::DragFloat3("Rotation", glm::value_ptr(transform.GetRotation()));
			ImGui::DragFloat3("Scale", glm::value_ptr(transform.GetScale()));
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

			auto& material = sprite.material;

			ImGui::Text("Material :");

			if (material.isValid())
			{
				if (ImGui::Button(sprite.material.GetName().c_str()))
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(sprite.materialID);
					MaterialEditorPanel::SetActiveMaterial(sprite.material, sprite.materialID);
					PanelManager::AddPanel(new MaterialEditorPanel());
				}

				
			}
			else
			{
				std::string buf;
				ImGui::InputText("Material", &buf, ImGuiInputTextFlags_ReadOnly);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::MATERIAL)
					{
						sprite.materialID = desc.assetID;
						sprite.material = Graphics::Material::LoadFile(desc.absolutePath);
					}
				}
				ImGui::EndDragDropTarget();
			}

			auto layers = SortingLayer2DHandler::GetRegisteredLayers();
			static int item_current_idx = 0;
			if (ImGui::BeginCombo("Sorting Layer",sprite.sortingLayer.c_str()))
			{
				for (int i = 0; i < layers.size(); i++)
				{
					const bool is_selected = (item_current_idx == i);
					if (ImGui::Selectable(layers[i].name.c_str(), is_selected))
					{
						item_current_idx = i;
						sprite.sortingLayer = layers[i].name.c_str();
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			
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

			auto gameAssembly = Application::GetGameAssembly();
			for (auto it : gameAssembly->GetScripts())
			{
				scriptNames.push_back(it);
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
						script.ScriptName = scriptNames[i];
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

	}

	std::string BodyTypeToStr(BodyType type)
	{
		switch (type)
		{
		case BodyType::STATIC:
			return "Static";
		case BodyType::DYNAMIC:
			return "Dynamic";
		}
	}
	void PropertyEditorPanel::DrawRigidBody2dComponent()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Rigid Body"))
		{
			if (DrawComponentContextMenu<RigidBody2dComponent>(m_ActiveEntity))
			{
				ImGui::TreePop();
				return;
			}
			auto& rigidBody = m_ActiveEntity.GetComponent<RigidBody2dComponent>();
			ImGui::Text("Rigid body :");

			const char* body_types[] = { "Static", "Dynamic" };
			std::string bodytypestr = BodyTypeToStr(rigidBody.type);
			static const char* current_body_type = bodytypestr.c_str();

			if (ImGui::BeginCombo("Body type", current_body_type))
			{
				for (int n = 0; n < IM_ARRAYSIZE(body_types); n++)
				{
					bool is_selected = (current_body_type == body_types[n]);
					if (ImGui::Selectable(body_types[n], is_selected))
					{
						current_body_type = body_types[n];
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (current_body_type == "Static")
			{
				rigidBody.type = BodyType::STATIC;
			}
			if (current_body_type == "Dynamic")
			{
				rigidBody.type = BodyType::DYNAMIC;
			}

			rigidBody.shape = BodyShape::POLYGON_SHAPE; // TODO : make the shapes selectable and add more shapes.

			ImGui::InputFloat("Density", &rigidBody.density);
			ImGui::InputFloat("Friction", &rigidBody.friction);

			ImGui::TreePop();
		}
	}

	void PropertyEditorPanel::DrawGUITextComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& textComponent = m_ActiveEntity.GetComponent<GUITextComponent>();
		if (ImGui::TreeNode("Text"))
		{
			if (ImGui::InputText("text", &textComponent.text.m_Text))
			{
				if (textComponent.text.IsValid())
				{
					textComponent.text.SetText(textComponent.text.m_Text);
				}
			}
			


			ImGui::ColorEdit3("color", glm::value_ptr(textComponent.textColor));

			static std::string fontname;
			static std::string fontPath;
			if (!textComponent.fontAssetID.empty())
			{
				auto desc = Application::GetAssetManager()->GetDescriptorByID(textComponent.fontAssetID);
				fontname = desc.assetName;
				fontPath = desc.absolutePath;
			}
			ImGui::InputText("Font", &fontname);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::FONT)
					{
						textComponent.text.SetFont(desc.absolutePath);
						textComponent.fontAssetID = desc.assetID;
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::InputScalar("Font Size", ImGuiDataType_U32, &textComponent.fontSize))
			{
				if (!textComponent.fontAssetID.empty())
				{
					textComponent.text.SetFont(fontPath, textComponent.fontSize);
				}
			}
		}
		ImGui::TreePop();
	}

	void PropertyEditorPanel::DrawGUIContainerComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& containerComponent = m_ActiveEntity.GetComponent<GUIContainerComponent>();
		glm::vec2 size = containerComponent.container.GetScreenSize();
		if (ImGui::TreeNode("container"))
		{
			ImGui::InputFloat2("size", glm::value_ptr(size));
		}
		ImGui::TreePop();
	}

	void PropertyEditorPanel::DrawRectTransformComponent()
	{
		ImGui::SetNextItemOpen(true);
		auto& rectTransformComp = m_ActiveEntity.GetComponent<RectTransformComponent>();

		if (ImGui::TreeNode("Rect Transform"))
		{
			glm::vec2 pos = rectTransformComp.GetRect().GetPosition();
			glm::vec2 size = { rectTransformComp.GetRect().GetWidth(), rectTransformComp.GetRect().GetHeight() };

			if (ImGui::DragFloat2("Position", glm::value_ptr(pos)))
			{
				rectTransformComp.GetRect().SetPosition(pos);
			}

			if (ImGui::DragFloat2("Size", glm::value_ptr(size)))
			{
				rectTransformComp.GetRect().SetWidth(size.x);
				rectTransformComp.GetRect().SetHeight(size.y);
			}
		}
		ImGui::TreePop();
	}

}