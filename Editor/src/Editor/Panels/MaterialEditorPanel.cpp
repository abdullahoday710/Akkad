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

	SharedPtr<Graphics::Material> MaterialEditorPanel::m_Material;

	MaterialEditorPanel::MaterialEditorPanel()
	{
	}

	void MaterialEditorPanel::SetActiveMaterial(SharedPtr<Graphics::Material> mat, std::string assetID)
	{
		m_Material = mat;
		m_MaterialAssetID = assetID;
	}

	void MaterialEditorPanel::DrawImGui()
	{
		ImGui::Begin("Material Editor", &showPanel);

		std::string label = "Material Name : " + m_Material->GetName();
		ImGui::Text(label.c_str());
		ImGui::Separator();

		auto shaders = Application::GetAssetManager()->GetAllShaders();

		if (m_Material->m_Shader == nullptr)
		{
			if (ImGui::BeginCombo("Shader", "Select a shader"))
			{
				for (int i = 0; i < shaders.size(); i++)
				{

					if (ImGui::Selectable(shaders[i].assetName.c_str()))
					{
						m_Material->SetShader(shaders[i].assetID);
						m_Material->SerializeShader();
					}
				}

				ImGui::EndCombo();
			}
		}

		else
		{
			static int item_current_idx = 0;
			std::string shaderID = m_Material->m_ShaderID;

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

							m_Material->SetShader(shaders[i].assetID);
							m_Material->SerializeShader();
						}
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		}
		ImGui::Text("Textures:");
		for (auto& it : m_Material->m_Textures)
		{
			auto& textureProp = it.second;
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

		ImGui::Text("Properties");
		if (m_Material->m_PropertyBuffer != nullptr)
		{
			for (auto element : m_Material->m_PropertyBuffer->GetLayout().GetElements())
			{
				std::string elementName = element.first;
				Graphics::ShaderDataType type;
				type = element.second.GetType();

				switch (type)
				{

				case Graphics::ShaderDataType::FLOAT:
				{
					float bufferValue = m_Material->m_PropertyBuffer->GetData<float>(elementName);
					ImGui::InputFloat(elementName.c_str(), &bufferValue);
					m_Material->m_PropertyBuffer->SetData(elementName, bufferValue);
					break;
				}

				case Graphics::ShaderDataType::FLOAT2:
				{
					glm::vec2 bufferValue = m_Material->m_PropertyBuffer->GetData<glm::vec2>(elementName);
					ImGui::InputFloat2(elementName.c_str(), glm::value_ptr(bufferValue));
					m_Material->m_PropertyBuffer->SetData(elementName, bufferValue);
					break;
				}

				case Graphics::ShaderDataType::FLOAT3:
				{
					glm::vec3 bufferValue = m_Material->m_PropertyBuffer->GetData<glm::vec3>(elementName);

					if (elementName.rfind("color_", 0) == 0) {
						ImGui::ColorEdit3(elementName.c_str(), glm::value_ptr(bufferValue));
					}
					else
					{
						ImGui::InputFloat3(elementName.c_str(), glm::value_ptr(bufferValue));
					}
					m_Material->m_PropertyBuffer->SetData(elementName, bufferValue);
					break;
				}

				case Graphics::ShaderDataType::FLOAT4:
				{
					glm::vec4 bufferValue = m_Material->m_PropertyBuffer->GetData<glm::vec4>(elementName);
					ImGui::InputFloat4(elementName.c_str(), glm::value_ptr(bufferValue));
					m_Material->m_PropertyBuffer->SetData(elementName, bufferValue);
					break;
				}

				case Graphics::ShaderDataType::UNISGNED_INT:
				{
					unsigned int bufferValue = m_Material->m_PropertyBuffer->GetData<unsigned int>(elementName);
					ImGui::InputScalar(elementName.c_str(), ImGuiDataType_U32, &bufferValue);
					m_Material->m_PropertyBuffer->SetData(elementName, bufferValue);
					break;
				}

				}

			}
		}

		ImGui::Text("Render flags");
		{
			using namespace Graphics;

			unsigned int flags = m_Material->GetRenderFlags();

			bool blend_enable = false;
			bool blend_inverse_alpha = false;
			bool blend_alpha_add = false;

			if (flags & Material::RenderFlags::BLEND_ENABLE)
			{
				blend_enable = true;
			}

			if (flags & Material::RenderFlags::BLEND_INVERSE_ALPHA && !(flags & Material::RenderFlags::BLEND_ALPHA_ADD))
			{
				blend_inverse_alpha = true;
			}

			if (flags & Material::RenderFlags::BLEND_ALPHA_ADD && !(flags & Material::RenderFlags::BLEND_INVERSE_ALPHA))
			{
				blend_alpha_add = true;
			}

			if (ImGui::Checkbox("Enable blending", &blend_enable))
			{
				if (blend_enable)
				{
					flags |= Material::RenderFlags::BLEND_ENABLE;
				}
				else
				{
					flags &= ~Material::RenderFlags::BLEND_ENABLE;
				}
				m_Material->SetRenderFlags(flags);
			}

			if (blend_enable)
			{
				if (ImGui::Checkbox("Blend : inverse alpha", &blend_inverse_alpha))
				{
					if (!blend_alpha_add && blend_inverse_alpha)
					{
						flags |= Material::RenderFlags::BLEND_INVERSE_ALPHA;
					}
					else
					{
						flags &= ~Material::RenderFlags::BLEND_INVERSE_ALPHA;
					}
					m_Material->SetRenderFlags(flags);
				}

				if (ImGui::Checkbox("Blend : alpha add", &blend_alpha_add))
				{
					if (blend_alpha_add && !blend_inverse_alpha)
					{
						flags |= Material::RenderFlags::BLEND_ALPHA_ADD;
					}
					else
					{
						flags &= ~Material::RenderFlags::BLEND_ALPHA_ADD;
					}
					m_Material->SetRenderFlags(flags);
				}
			}


		}

		if (ImGui::Button("Save material"))
		{
			std::string path = EditorLayer::GetActiveProject().GetAssetsPath().string();

			path += m_Material->m_Name + ".mat";
			MaterialSerializer::Serialize(m_Material, path);

			auto scene = EditorLayer::GetActiveScene();

			auto view = scene->m_Registry.view<SpriteRendererComponent>();

			for (auto entity : view)
			{
				auto& sprite = view.get<SpriteRendererComponent>(entity);
				if (sprite.materialID == m_MaterialAssetID)
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(m_MaterialAssetID);

					sprite.sprite.SetMaterial(desc.absolutePath);
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