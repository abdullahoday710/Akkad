#pragma once
#include <functional>
#include "Event.h"
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
	};
}
