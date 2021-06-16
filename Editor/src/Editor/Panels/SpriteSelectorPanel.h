#pragma once
#include "Panel.h"

#include <Akkad/ECS/Entity.h>

namespace Akkad {

	class SpriteSelectorPanel : public Panel
	{
	public:
		SpriteSelectorPanel();
		~SpriteSelectorPanel() {}

		static void SetActiveEntity(Entity entity);

		virtual void DrawImGui() override;
		virtual void OnOpen() override { showPanel = true; }
		virtual void OnClose() override;
		virtual bool IsOpen() override { return showPanel; };

		virtual std::string GetName() override { return "SpriteSelectorPanel"; };

	private:
		static bool showPanel;
		static Entity m_ActiveEntity;
	};
}


