#include "InstantiableEntityPreviewPanel.h"
#include "Editor/EditorLayer.h"

#include <Akkad/ECS/Serializers/SceneSerializer.h>
#include <Akkad/Application/Application.h>
#include <Akkad/Asset/AssetManager.h>
#include <Akkad/ECS/Serializers/InstantiableEntitySerializer.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <fstream>
namespace Akkad {
	bool InstantiableEntityPreviewPanel::showPanel;
	std::string InstantiableEntityPreviewPanel::m_FileAssetID;
	nlohmann::ordered_json InstantiableEntityPreviewPanel::m_Data;

	void InstantiableEntityPreviewPanel::DrawImGui()
	{
		// TODO : show more information about the entity here.
		ImGui::Begin("Instantiable Entity Preview", &showPanel);
		ImGui::Text("Drag an entity from the hierarchy and drop it below to");
		ImGui::Text("save it as an instantiable entity");

		std::string name = m_Data["Scene"]["Name"];
		ImGui::InputText("Entity", &name, ImGuiInputTextFlags_ReadOnly);

		if (ImGui::BeginDragDropTarget())
		{
			auto scene = EditorLayer::GetActiveScene();
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG_DROP"))
			{
				size_t* entity_id = (size_t*)payload->Data;

				Entity e = Entity((entt::entity)*entity_id, scene.get());

				if (e.IsValid())
				{
					auto desc = Application::GetAssetManager()->GetDescriptorByID(m_FileAssetID);
					InstantiableEntitySerializer::Serialize(e, desc.absolutePath);

					SetEntityFile(m_FileAssetID); // reload file after saving
				}

			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}

	void InstantiableEntityPreviewPanel::OnOpen()
	{
		showPanel = true;
	}

	void InstantiableEntityPreviewPanel::OnClose()
	{
	}

	void InstantiableEntityPreviewPanel::SetEntityFile(std::string assetID)
	{
		m_FileAssetID = assetID;
		auto desc = Application::GetAssetManager()->GetDescriptorByID(assetID);
		std::ifstream file;
		file.open(desc.absolutePath);

		file >> m_Data;
	}
}