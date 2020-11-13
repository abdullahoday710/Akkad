#pragma once

#include "Akkad/Graphics/RenderContext.h"

#include <Windows.h>

namespace Akkad {
	namespace Graphics {

		class Win32RenderContext : public RenderContext {
		public:
			virtual void Init(RenderAPI api) override;
			virtual void SwapWindowBuffers() override;
			virtual void SetVsync(bool status) override;

		private:
			friend class ImGuiWindowHandler;
			HGLRC m_GLContext = nullptr;
			HDC m_DeviceContext = nullptr;
			RenderAPI m_API;
			bool m_VsyncEnabled = false;
		};
	}
}