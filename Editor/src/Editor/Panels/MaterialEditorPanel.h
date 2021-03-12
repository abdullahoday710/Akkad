#pragma once
#include "Panel.h"

namespace Akkad {

	namespace Graphics {
		class Material;
	}

	class MaterialEditorPanel : public Panel
	{
	public:
		MaterialEditorPanel();
		~MaterialEditorPanel() {}

		static void SetActiveMaterial(Graphics::Material& mat, std::string assetID);

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "MaterialEditorPanel"; };

	private:
		static bool showPanel;
		static Graphics::Material& m_Material;
		static std::string m_MaterialAssetID;

		friend class EditorLayer;
	};
}


