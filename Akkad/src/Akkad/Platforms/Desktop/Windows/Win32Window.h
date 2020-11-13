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
		virtual unsigned int GetWidth() { return m_Width; };
		virtual unsigned int GetHeight() { return m_Height; };
		virtual void* GetNativeWindow() override { return m_WindowHandle; };
		virtual void SetVsync(bool status) override {};
		std::function<void(Event&)> m_EventCallback;
	private:
		bool m_IsClosed = true;
		HWND m_WindowHandle = nullptr;
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		bool m_VsyncEnabled = true;
		Graphics::RenderAPI m_RenderAPI;
	};

}

