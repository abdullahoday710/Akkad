#pragma once
#include "Panel.h"

#include <Akkad/core.h>
namespace Akkad {

	namespace Graphics {
		class Texture;
	}

	class TexturePreviewPanel : public Panel
	{
	public:
		TexturePreviewPanel() {};
		~TexturePreviewPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		static void SetActiveTexture(std::string assetID);

		virtual std::string GetName() override { return "TexturePreviewPanel"; }

	private:
		static bool showPanel;
		static std::string m_TextureAssetID;
		static SharedPtr<Graphics::Texture> m_Texture;
	};
}

