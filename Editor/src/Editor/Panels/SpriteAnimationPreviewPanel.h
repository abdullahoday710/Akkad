#pragma once
#include "Panel.h"

#include <Akkad/core.h>
namespace Akkad {

	namespace Graphics {
		class SpriteAnimation;
		class Texture;
	}

	class SpriteAnimationPreviewPanel : public Panel
	{
	public:
		SpriteAnimationPreviewPanel() {};
		~SpriteAnimationPreviewPanel() {};

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		static void SetActiveAnimation(SharedPtr<Graphics::SpriteAnimation> animation, std::string assetID);

		virtual std::string GetName() override { return "SpriteAnimationPreviewPanel"; }

	private:
		static bool showPanel;
		static std::string m_AnimationAssetID;
		static SharedPtr<Graphics::SpriteAnimation> m_Animation;
		static SharedPtr<Graphics::Texture> m_AnimationSpriteSheet;
	};
}

