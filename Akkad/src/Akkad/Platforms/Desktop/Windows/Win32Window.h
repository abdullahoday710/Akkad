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
		virtual void CreateContext(Graphics::RenderAPI api) override;
		virtual unsigned int GetWidth() { return m_Width; };
		virtual unsigned int GetHeight() { return m_Height; };
		virtual void* GetNativeWindow() override { return m_WindowHandle; };
		std::function<void(Event&)> m_EventCallback;
	private:
		bool m_IsClosed = true;
		HWND m_WindowHandle;
		HDC m_DeviceContext = nullptr;
		unsigned int m_Width;
		unsigned int m_Height;
	};

}

