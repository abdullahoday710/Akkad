#pragma once
#include "RenderPlatform.h"
namespace Akkad {
	namespace Graphics {

		class ImGuiHandler {
		public:
			virtual void Init() = 0;
			virtual void NewFrame() = 0;
			virtual void Render() = 0;
			virtual void ShutDown() = 0;

			static ImGuiHandler* create(RenderAPI api);
		};

		class ImGuiWindowHandler {
		public:
			static void Init();
			static void NewFrame();
			static void ShutDown();
		};
	}
}