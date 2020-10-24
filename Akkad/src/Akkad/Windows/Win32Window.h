#pragma once
#include <Windows.h>
#include "Akkad/Application/IWindow.h"

namespace Akkad {

	class Win32Window : public Window
	{
	public:
		virtual int Init(WindowSettings settings) override;
		virtual void OnUpdate() override;
		virtual bool IsCloseRequested() override;
		virtual void SetEventCallback(std::function<void(Event&)> func) override { m_EventCallback = func; };

		std::function<void(Event&)> m_EventCallback;
	private:
		bool m_IsClosed = true;
	};

}

