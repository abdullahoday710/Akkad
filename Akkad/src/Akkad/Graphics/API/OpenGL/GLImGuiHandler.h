#pragma once
#include "Akkad/Graphics/ImGuiHandler.h"
namespace Akkad {
	namespace Graphics {

		class GLImGuiHandler : public ImGuiHandler
		{
		public:
			virtual void Init() override;
			virtual void NewFrame() override;
			virtual void Render() override;
			virtual void ShutDown() override;

			virtual void UpdateRenderPlatforms() override;
		};
	}
}


