#pragma once
#include "Akkad/Application/IWindow.h"
namespace Akkad {

	class WebWindow : public Window
	{
	public:
		virtual int Init(WindowSettings settings);
		virtual void OnUpdate();
		virtual void SetEventCallback(std::function<void(Event&)> func);
		virtual unsigned int GetWidth();
		virtual unsigned int GetHeight();
		virtual glm::vec2 GetWindowRectMin();
		virtual glm::vec2 GetWindowRectMax();
		virtual void* GetNativeWindow();
		virtual void ToggleFullScreen();
		virtual bool IsFullScreen();
	};
}


