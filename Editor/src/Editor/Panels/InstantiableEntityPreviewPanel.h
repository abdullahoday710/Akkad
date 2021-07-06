#pragma once
#include "Panel.h"

#include <Akkad/ECS/Entity.h>
#include <Akkad/ECS/Scene.h>

#include <json.hpp>

namespace Akkad {

	class InstantiableEntityPreviewPanel : public Panel
	{
	public:
		InstantiableEntityPreviewPanel() {};
		~InstantiableEntityPreviewPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override;
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "InstantiableEntityPreviewPanel"; }
		static void SetEntityFile(std::string assetID);

	private:
		static bool showPanel;
		static std::string m_FileAssetID;
		static nlohmann::ordered_json m_Data;
	};
}


