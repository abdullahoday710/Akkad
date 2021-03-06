#include "MaterialEditorPanel.h"

#include "Editor/EditorLayer.h"
#include "Editor/Serializers/MaterialSerializer.h"

#include "Akkad/Graphics/Material.h"
#include "Akkad/Application/Application.h"
#include "Akkad/ECS/Scene.h"
#include "Akkad/ECS/Components/SpriteRendererComponent.h"
#include "Akkad/Asset/AssetManager.h"

#include <imgui.h>
#include <entt/entt.hpp>
#include <misc/cpp/imgui_stdlib.h>

namespace Akkad {
	bool MaterialEditorPanel::showPanel;
	std::string MaterialEditorPanel::m_MaterialAssetID;

	Graphics::Material& MaterialEditorPanel::m_Material = Graphics::Material();

	MaterialEditorPanel::MaterialEditorPanel()
	{
	}

	void MaterialEditorPanel::SetActiveMaterial(Graphics::Material& mat, std::string assetID)
	{
		m_Material = mat;
		m_MaterialAssetID = assetID;
	}

	void MaterialEditorPanel::DrawImGui()
	{
		std::string label = "Material Editor : " + m_Material.m_Name;
		ImGui::Begin(label.c_str(), &showPanel);

		auto shaders = Application::GetAssetManager()->GetAllShaders();

		if (m_Material.m_Shader == nullptr)
		{
			if (ImGui::BeginCombo("Shader", "Select a shader"))
			{
				for (int i = 0; i < shaders.size(); i++)
				{

					if (ImGui::Selectable(shaders[i].assetName.c_str()))
					{
						m_Material.SetShader(shaders[i].assetID);
					}
				}

				ImGui::EndCombo();
			}
		}

		else
		{
			static int item_current_idx = 0;
			std::string shaderID = m_Material.m_ShaderID;

			ptrdiff_t pos = std::find_if(shaders.begin(), shaders.end(), [&shaderID](const AssetDescriptor& obj) {return obj.assetID == shaderID; }) - shaders.begin();
			if (pos <= shaders.size())
			{
				item_current_idx = pos;
			}
			const char* combo_label = shaders[item_current_idx].assetName.c_str();

			if (ImGui::BeginCombo("Shader", combo_label))
			{
				for (int i = 0; i < shaders.size(); i++)
				{
					const bool is_selected = (item_current_idx == i);
					if (ImGui::Selectable(shaders[i].assetName.c_str(), is_selected))
					{
						item_current_idx = i;
						if (shaders[i].absolutePath != "null")
						{

							m_Material.SetShader(shaders[i].assetID);
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		}
		ImGui::Text("Textures:");
		for (auto& textureProp : m_Material.m_Textures)
		{
			if (textureProp.assetID.empty())
			{
				std::string buf = textureProp.assetID;
				ImGui::InputText(textureProp.samplerName.c_str(), &buf, ImGuiInputTextFlags_ReadOnly);
			}
			else
			{
				auto texture = Application::GetAssetManager()->GetTexture(textureProp.assetID);
				ImVec2 size{ 100, 100 };
				ImGui::Text(textureProp.samplerName.c_str());
				ImGui::Image((void*)texture->GetID(), size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG_DROP"))
				{
					const char* id = (const char*)payload->Data;
					AssetDescriptor desc = Application::GetAssetManager()->GetDescriptorByID(id);

					if (desc.assetType == AssetType::TEXTURE)
					{
						textureProp.assetID = id;
					}

				}
				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::Button("Save material"))
		{
			std::string path = EditorLayer::GetActiveProject().GetAssetsPath().string();

			path += m_Material.m_Name + ".mat";
			MaterialSerializer::Serialize(m_Material, path);

			auto scene = EditorLayer::GetActiveScene();
			
			auto view = scene->m_Registry.view<SpriteRendererComponent>();

			for (auto entity : view)
			{
				auto& sprite = view.get<SpriteRendererComponent>(entity);
				if (sprite.materialID == m_MaterialAssetID)
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(m_MaterialAssetID);

					sprite.material = Graphics::Material::LoadFile(desc.absolutePath);
				}
			}
		}

		ImGui::End();
	}

	void MaterialEditorPanel::OnClose()
	{
		showPanel = false;
	}
}