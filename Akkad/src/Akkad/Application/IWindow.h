#pragma once
#include <functional>
#include "Event.h"
#include "Akkad/Graphics/RenderPlatform.h"
namespace Akkad {
	
	struct WindowSettings {
		const char* title;
		unsigned int width;
		unsigned int height;
	};

	class Window {

	public:
		virtual int Init(WindowSettings settings) = 0;
		virtual void OnUpdate() = 0;
		virtual bool IsCloseRequested() = 0;
		virtual void SetEventCallback(std::function<void(Event&)> func) = 0;
		virtual void CreateContext(Graphics::RenderAPI api) = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
	};
}
