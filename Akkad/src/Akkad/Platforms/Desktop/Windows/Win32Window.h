#pragma once
#include <Windows.h>
#include "Akkad/Application/IWindow.h"

namespace Akkad {

	class Win32Window : public Window
	{
	public:
		virtual int Init(WindowSettings settings) override;
		virtual void OnUpdate() override;
		virtual void SetEventCallback(std::function<void(Event&)> func) override { m_EventCallback = func; };
		virtual unsigned int GetWidth() { return m_Width; };
		virtual unsigned int GetHeight() { return m_Height; };
		virtual glm::vec2 GetWindowRectMin() override;
		virtual glm::vec2 GetWindowRectMax() override;
		virtual void* GetNativeWindow() override { return m_WindowHandle; };
		virtual void ToggleFullScreen() override;
		virtual bool IsFullScreen() { return m_FullScreen; };
		void ResetKeyStates();

		std::function<void(Event&)> m_EventCallback;

		bool isCursorTracked = false;

		void _SetSize(unsigned int width, unsigned int height)
		{
			m_Width = width;
			m_Height = height;
		}
		int m_MouseStates[3] = {};
	private:
		bool m_IsClosed = true;
		bool m_FullScreen = false;
		HWND m_WindowHandle = nullptr;
		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		bool m_VsyncEnabled = true;
		Graphics::RenderAPI m_RenderAPI;

	};

}

